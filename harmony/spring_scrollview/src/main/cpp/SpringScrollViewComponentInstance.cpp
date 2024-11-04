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
        DLOG(INFO) << "SpringScrollViewComponentInstance::setProps refreshHeaderHeight: " << p->refreshHeaderHeight  
        << " loadingFooterHeight " << p->loadingFooterHeight << " bounces " << p->bounces << " allLoaded " << p->allLoaded 
        << " inverted " << p->inverted << " scrollEnabled " << p->scrollEnabled << " directionalLockEnabled " << p->directionalLockEnabled
        << " initialContentOffset.x " << p->initialContentOffset.x << " initialContentOffset.y " << initialContentOffset.y << " pagingEnabled " << p->pagingEnabled
        << " pageSize.width " << p->pageSize.width << " pageSize.height " << p->pageSize.height << " decelerationRate " << p->decelerationRate;
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
        this->getLocalRootArkUINode().setPagingEnabled(p->pagingEnabled);
        this->getLocalRootArkUINode().setPageSize(p->pageSize.width, p->pageSize.height);
        this->getLocalRootArkUINode().setDecelerationRate(p->decelerationRate);
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


void SpringScrollViewComponentInstance::onScroll(
    const facebook::react::RNCSpringScrollViewEventEmitter::OnScroll &onScroll) {
    DLOG(INFO) << "onScroll";
    m_eventEmitter->onScroll(onScroll);
};

void SpringScrollViewComponentInstance::onCustomScrollBeginDrag() {
    DLOG(INFO) << "onCustomScrollBeginDrag";
    m_eventEmitter->onCustomScrollBeginDrag();
}


void SpringScrollViewComponentInstance::onCustomTouchBegin() {
    DLOG(INFO) << "onCustomTouchBegin";
    m_eventEmitter->onCustomTouchBegin();
}

void SpringScrollViewComponentInstance::onCustomTouchEnd() {
    DLOG(INFO) << "onCustomTouchEnd";
    m_eventEmitter->onCustomTouchEnd();
}

void SpringScrollViewComponentInstance::onCustomScrollEndDrag() {
    DLOG(INFO) << "onCustomScrollEndDrag";
    m_eventEmitter->onCustomScrollEndDrag();
}

void SpringScrollViewComponentInstance::onCustomMomentumScrollBegin() {
    DLOG(INFO) << "onCustomMomentumScrollBegin";
    m_eventEmitter->onCustomMomentumScrollBegin();
}

void SpringScrollViewComponentInstance::onCustomMomentumScrollEnd() {
    DLOG(INFO) << "onCustomMomentumScrollEnd";
    m_eventEmitter->onCustomMomentumScrollEnd();
}

void SpringScrollViewComponentInstance::callArkTSAnimationCancel() {
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        arkTsTurboModule->callSync("cancelAnimation", {});
    }
}


void SpringScrollViewComponentInstance::callArkTSInnerStart(float f, float v0, float d, float lower, float upper,
                                                            bool pagingEnabled, float pageSize,bool isVertical) {
    DLOG(INFO) << "callArkTSInnerStart f " << f << " v0 " << v0 << " d " << d << " lower " << lower << " upper " << upper 
    << " pagingEnabled " << pagingEnabled << " pageSize " << pageSize << " isVertical " << isVertical;

    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(f);
        args.push_back(v0);
        args.push_back(d);
        args.push_back(lower);
        args.push_back(upper);
        args.push_back(pagingEnabled);
        args.push_back(pageSize);
        args.push_back(isVertical);
        arkTsTurboModule->callSync("startInner", args);
    }
}
void SpringScrollViewComponentInstance::callArkTSOuterStart(float f, float v0, float d,bool isVertical) {
    DLOG(INFO) << "callArkTSOuterStart f " << f << " v0 " << v0 << " d " << d;
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(f);
        args.push_back(v0);
        args.push_back(d);
        args.push_back(isVertical);
        arkTsTurboModule->callSync("startOuter", args);
    }
}
void SpringScrollViewComponentInstance::callArkTSReboundStart(float f, float t, long d ,bool isVertical) {
    DLOG(INFO) << "callArkTSReboundStart f " << f << " t " << t << " d " << d;
    auto rnInstancePtr = this->m_deps->rnInstance.lock();
    if (rnInstancePtr != nullptr) {
        auto turboModule = rnInstancePtr->getTurboModule("SpringScrollViewContext");
        auto arkTsTurboModule = std::dynamic_pointer_cast<rnoh::ArkTSTurboModule>(turboModule);
        std::vector<ArkJS::IntermediaryArg> args;
        args.push_back(f);
        args.push_back(t);
        args.push_back(d);
        args.push_back(isVertical);
        arkTsTurboModule->callSync("startRebound", args);
    }
}


facebook::react::Point SpringScrollViewComponentInstance::getCurrentOffset() const {
    auto recordEvent = std::static_pointer_cast<SpringScrollViewEvent>(EventBus::EventBus::getInstance()->getEvent());
    float scrollOffsetY = recordEvent->getEventContentOffset().y;
    return {0, scrollOffsetY};
}

void SpringScrollViewComponentInstance::handleCommand(std::string const &commandName, folly::dynamic const &args) {
    DLOG(INFO) << "SpringScrollViewComponentInstance handleCommand commandName:" << commandName;
    if (commandName == "10000") {
        this->getLocalRootArkUINode().endRefresh();
    } else if (commandName == "10001") {
        this->getLocalRootArkUINode().endLoading(args[0].asBool());
        this->firstLoad = false;
    } else if (commandName == "10002") {
        if (args[0] == nullptr) {
            this->getLocalRootArkUINode().scrollTo(0, args[1].asDouble(), args[2].asBool());
            return;
        }
        this->getLocalRootArkUINode().scrollTo(args[0].asDouble(), args[1].asDouble(), args[2].asBool());
    }
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
    DLOG(INFO) << "SpringScrollViewComponentInstance onStateChanged content_h:" << stateData.getContentSize().height
               << " content_w:" << stateData.getContentSize().width << " h:"<<m_layoutMetrics.frame.size.height << " w:" << m_layoutMetrics.frame.size.width;
    if(!this->firstShowUI){    
    this->getLocalRootArkUINode().setChildHeight(m_layoutMetrics.frame.size.height);
    this->getLocalRootArkUINode().setChildWidth(m_layoutMetrics.frame.size.width);
    }
    this->firstShowUI = false;
}

bool SpringScrollViewComponentInstance::isHandlingTouches() const { return this->swiperStatus; }

void SpringScrollViewComponentInstance::setSwiperStatus(bool swiperStatus) { this->swiperStatus = swiperStatus; }

void SpringScrollViewComponentInstance::finalizeUpdates() {
    DLOG(INFO) << "SpringScrollViewComponentInstance finalizeUpdates";
    this->getLocalRootArkUINode().init();
}

void SpringScrollViewComponentInstance::sendEventAnimationsOnScroll(
        facebook::react::RNCSpringScrollViewEventEmitter::OnScroll onScroll) {
        auto nativeAnimatedTurboModule = m_springNativeAnimatedTurboModule.lock();
        if (nativeAnimatedTurboModule == nullptr) {
            auto instance = m_deps->rnInstance.lock();
            if (instance == nullptr) {
                   return;
            }
            nativeAnimatedTurboModule =
                instance->getTurboModule<NativeAnimatedTurboModule>("NativeAnimatedTurboModule");
            m_springNativeAnimatedTurboModule = nativeAnimatedTurboModule;
        }
        if (nativeAnimatedTurboModule != nullptr) {
            using folly::dynamic;
            dynamic payload = dynamic::object("contentOffset",dynamic::object("x",onScroll.contentOffset.x)("y",onScroll.contentOffset.y)("refreshStatus",onScroll.refreshStatus)("loadingStatus",onScroll.loadingStatus));
            nativeAnimatedTurboModule->handleComponentEvent(m_tag, "onScroll",payload);
            DLOG(INFO) << "SpringScrollViewComponentInstance::sendEventAnimationsOnScroll y " << onScroll.contentOffset.y;
        }
}

} // namespace rnoh