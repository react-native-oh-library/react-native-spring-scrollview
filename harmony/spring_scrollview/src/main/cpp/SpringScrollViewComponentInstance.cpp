/**
 * MIT License
 *
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "SpringScrollViewComponentInstance.h"
#include "Types.h"
#include <folly/dynamic.h>
#include "RNOH/arkui/ArkUINode.h"
#include "RNOH/arkui/ScrollNode.h"
namespace rnoh {
SpringScrollViewComponentInstance::SpringScrollViewComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {
    m_springStackNode.setSpringScrollViewNodeDelegate(this);
}


void SpringScrollViewComponentInstance::onChildInserted(ComponentInstance::Shared const &childComponentInstance,
                                                        std::size_t index) {
    CppComponentInstance::onChildInserted(childComponentInstance, index);
    m_springStackNode.insertChild(childComponentInstance->getLocalRootArkUINode(), index);
}
void SpringScrollViewComponentInstance::onChildRemoved(ComponentInstance::Shared const &childComponentInstance) {
    CppComponentInstance::onChildRemoved(childComponentInstance);
    m_springStackNode.removeChild(childComponentInstance->getLocalRootArkUINode());
};

SpringScrollViewNode &SpringScrollViewComponentInstance::getLocalRootArkUINode() { return m_springStackNode; }

void SpringScrollViewComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
    DLOG(INFO) << "SpringScrollViewComponentInstance::onPropsChanged";
    CppComponentInstance::onPropsChanged(props);
    if (props == nullptr) {
        return;
    }
    if (auto p = std::dynamic_pointer_cast<const facebook::react::RNCSpringScrollViewProps>(props)) {
        DLOG(INFO) << "<SpringScrollViewComponentInstance::setProps> refreshHeaderHeight: " << p->refreshHeaderHeight;
        DLOG(INFO) << "<SpringScrollViewComponentInstance::setProps> loadingFooterHeight: " << p->loadingFooterHeight;
        DLOG(INFO) << "<SpringScrollViewComponentInstance::setProps> bounces: " << p->bounces;
        DLOG(INFO) << "<SpringScrollViewComponentInstance::setProps> allLoaded: " << p->allLoaded;
        DLOG(INFO) << "<SpringScrollViewComponentInstance::setProps> inverted: " << p->inverted;
        DLOG(INFO) << "<SpringScrollViewComponentInstance::setProps> scrollEnabled: " << p->scrollEnabled;
        DLOG(INFO) << "<SpringScrollViewComponentInstance::setProps> directionalLockEnabled: "
                   << p->directionalLockEnabled;
        DLOG(INFO) << "<SpringScrollViewComponentInstance::setProps> initialContentOffset:"
                   << p->initialContentOffset.y;
        refreshHeaderHeight = p->refreshHeaderHeight;
        loadingFooterHeight = p->loadingFooterHeight;
        bounces = p->bounces;
        inverted = p->inverted;
        allLoaded = p->allLoaded;
        scrollEnabled = p->scrollEnabled;
        initialContentOffset.x = p->initialContentOffset.x;
        initialContentOffset.y = p->initialContentOffset.y;
        this->getLocalRootArkUINode().setInitialContentOffset(p->initialContentOffset.x, p->initialContentOffset.y);
        this->getLocalRootArkUINode().setRefreshHeaderHeight(p->refreshHeaderHeight);
        this->getLocalRootArkUINode().setLoadingFooterHeight(p->loadingFooterHeight);
        this->getLocalRootArkUINode().setBounces(p->bounces);
        this->getLocalRootArkUINode().setAllLoaded(p->allLoaded);
        this->getLocalRootArkUINode().setInverted(p->inverted);
        this->getLocalRootArkUINode().setScrollEnabled(p->scrollEnabled);
        this->getLocalRootArkUINode().setDirectionalLockEnabled(p->directionalLockEnabled);
    }
}

void SpringScrollViewComponentInstance::onRefresh() {
    DLOG(INFO) << "onRefresh";
    m_eventEmitter->onRefresh({});
};
void SpringScrollViewComponentInstance::onLoading() {
    DLOG(INFO) << "onLoading";
    m_eventEmitter->onLoading({});
};
void SpringScrollViewComponentInstance::onTouchBegin() {
    DLOG(INFO) << "onTouchBegin";
    m_eventEmitter->onTouchBegin({});
};
void SpringScrollViewComponentInstance::onTouchEnd() {
    DLOG(INFO) << "onTouchEnd";
    m_eventEmitter->onTouchFinish({});
};
void SpringScrollViewComponentInstance::onMomentumScrollBegin() {
    DLOG(INFO) << "onMomentumScrollBegin";
    m_eventEmitter->onMomentumScrollBegin({});
};
void SpringScrollViewComponentInstance::onMomentumScrollEnd() {
    DLOG(INFO) << "onMomentumScrollEnd";
    m_eventEmitter->onMomentumScrollEnd({});
};
void SpringScrollViewComponentInstance::onScroll(
    const facebook::react::RNCSpringScrollViewEventEmitter::OnScroll &onScroll) {
    DLOG(INFO) << "onScroll";
    m_eventEmitter->onScroll(onScroll);
};


void SpringScrollViewComponentInstance::callArkTSInnerAnimationStart(float from, float to, long duration) {
    DLOG(INFO) << "SpringScrollViewComponentInstance callArkTSInnerAnimationStart "
               << " from:" << from << " to:" << to << " duration:" << duration;
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(from);
        args.push_back(to);
        args.push_back(duration);
        arkTsTurboModule->callSync("startInnerAnimation", args);
    }
}

void SpringScrollViewComponentInstance::callArkTSOuterAnimationStart(float from, float to, int duration) {
    DLOG(INFO) << "SpringScrollViewComponentInstance callArkTSOuterAnimationStart "
               << " from:" << from << " to:" << to << " duration:" << duration;
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(from);
        args.push_back(to);
        args.push_back(duration);
        arkTsTurboModule->callSync("startOuterAnimation", args);
    }
}


void SpringScrollViewComponentInstance::callArkTSInnerHorizontalAnimationStart(float from, float to, long duration) {
    DLOG(INFO) << "SpringScrollViewComponentInstance callArkTSInnerHorizontalAnimationStart "
               << " from:" << from << " to:" << to << " duration:" << duration;
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(from);
        args.push_back(to);
        args.push_back(duration);
        arkTsTurboModule->callSync("startInnerHorizontalAnimation", args);
    }
}

void SpringScrollViewComponentInstance::callArkTSOuterHorizontalAnimationStart(float from, float to, long duration) {
    DLOG(INFO) << "SpringScrollViewComponentInstance callArkTSOuterHorizontalAnimationStart "
               << " from:" << from << " to:" << to << " duration:" << duration;
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(from);
        args.push_back(to);
        args.push_back(duration);
        arkTsTurboModule->callSync("startOuterHorizontalAnimation", args);
    }
}

void SpringScrollViewComponentInstance::callArkTSReboundAnimationStart(float from, float to, long duration) {
    DLOG(INFO) << "SpringScrollViewComponentInstance callArkTSReboundAnimationStart "
               << " from:" << from << " to:" << to << " duration:" << duration;
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(from);
        args.push_back(to);
        args.push_back(duration);
        arkTsTurboModule->callSync("startReboundAnimation", args);
    }
}

void SpringScrollViewComponentInstance::callArkTSHorizontalReboundAnimationStart(float from, float to, long duration) {
    DLOG(INFO) << "SpringScrollViewComponentInstance callArkTSHorizontalReboundAnimationStart "
               << " from:" << from << " to:" << to << " duration:" << duration;
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(from);
        args.push_back(to);
        args.push_back(duration);
        arkTsTurboModule->callSync("startHorizontalReboundAnimation", args);
    }
}

void SpringScrollViewComponentInstance::callArkTSEndRefreshStart(float from, float to, long duration) {
    DLOG(INFO) << "SpringScrollViewComponentInstance callArkTSEndRefreshStart "
               << " from:" << from << " to:" << to << " duration:" << duration;
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(from);
        args.push_back(to);
        args.push_back(duration);
        arkTsTurboModule->callSync("startEndRefresh", args);
    }
}

void SpringScrollViewComponentInstance::callArkTSEndLoadingStart(float from, float to, long duration) {
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(from);
        args.push_back(to);
        args.push_back(duration);
        arkTsTurboModule->callSync("startEndLoading", args);
    }
}

void SpringScrollViewComponentInstance::callArkTSScrollXStart(float from, float to, long duration) {
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(from);
        args.push_back(to);
        args.push_back(duration);
        arkTsTurboModule->callSync("startScrollX", args);
    }
}

void SpringScrollViewComponentInstance::callArkTSScrollYStart(float from, float to, long duration) {
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(from);
        args.push_back(to);
        args.push_back(duration);
        arkTsTurboModule->callSync("startScrollY", args);
    }
}

void SpringScrollViewComponentInstance::callArkTSAnimationCancel() {
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        arkTsTurboModule->callSync("cancelAnimation", {});
    }
}

void SpringScrollViewComponentInstance::handleCommand(std::string const &commandName, folly::dynamic const &args) {
    DLOG(INFO) << "SpringScrollViewComponentInstance handleCommand commandName:" << commandName;
    if (commandName == "10000") {
        this->getLocalRootArkUINode().endRefresh();
    } else if (commandName == "10001") {
        this->getLocalRootArkUINode().endLoading();
        this->firstLoad = false;
    } else if (commandName == "10002") {
        this->getLocalRootArkUINode().scrollTo(args[0].asDouble(), args[1].asDouble(), args[2].asBool());
    }
}

void SpringScrollViewComponentInstance::setLayout(facebook::react::LayoutMetrics layoutMetrics) {
    CppComponentInstance::setLayout(layoutMetrics);
    DLOG(INFO) << "SpringScrollViewComponentInstance setLayout w:" << layoutMetrics.frame.size.width
               << " h:" << layoutMetrics.frame.size.height;
}

facebook::react::Size SpringScrollViewComponentInstance::getLayoutSize() { return this->m_layoutMetrics.frame.size; }

void SpringScrollViewComponentInstance::onStateChanged(SharedConcreteState const &state) {
    CppComponentInstance::onStateChanged(state);
    auto stateData = state->getData();
    this->getLocalRootArkUINode().setScreenSize(
        {static_cast<float>(m_layoutMetrics.frame.size.width), static_cast<float>(m_layoutMetrics.frame.size.height)});
    this->getLocalRootArkUINode().setContentHeight(stateData.getContentSize().height);
    float tempHeight = (stateData.getContentSize().height < m_layoutMetrics.frame.size.height)
                           ? m_layoutMetrics.frame.size.height
                           : stateData.getContentSize().height;
    this->getLocalRootArkUINode().setContentSize(
        {static_cast<float>(stateData.getContentSize().width), static_cast<float>(tempHeight)});
    DLOG(INFO) << "SpringScrollViewComponentInstance onStateChanged h:" << tempHeight
               << " w:" << m_layoutMetrics.frame.size.width;
}

bool SpringScrollViewComponentInstance::isHandlingTouches() const { return this->swiperStatus; }

void SpringScrollViewComponentInstance::setSwiperStatus(bool swiperStatus) { this->swiperStatus = swiperStatus; }


void SpringScrollViewComponentInstance::finalizeUpdates() {
    LOG(INFO) << "SpringScrollViewComponentInstance finalizeUpdates";
    this->getLocalRootArkUINode().init();
}

} // namespace rnoh