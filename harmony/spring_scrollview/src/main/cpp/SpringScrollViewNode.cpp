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

#include "SpringScrollViewNode.h"
#include "EventBus/include/EventBus/EventBus.h"
#include "RNOH/arkui/NativeNodeApi.h"
#include "SpringScrollViewComponentInstance.h"
#include <arkui/ui_input_event.h>
#include <cmath>

namespace rnoh {
SpringScrollViewNode::SpringScrollViewNode()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_STACK)),
      m_stackArkUINodeHandle(nullptr) {}
SpringScrollViewNode::~SpringScrollViewNode() {
    this->isDestory = true;
    m_scrollNodeDelegate->callArkTSAnimationCancel();
    this->isInitialContentOffset = false;
    this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
    this->recordEventModel->setRefreshStatus("waiting");
    this->recordEventModel->setLoadingStatus("waiting");
    this->recordEventModel->setEventBounces(false);
    this->recordEventModel->setEventContentOffset({0, 0});
    this->recordEventModel->setEventSize({0, 0});
    this->recordEventModel->setEventContentSize({0, 0});
    this->recordEventModel->setEventContentInsets({0, 0, 0, 0});
    this->recordEventModel->setEventBeginPoint({0, 0});
    this->recordEventModel->setEventLastPoint({0, 0});
    auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
    EventBus::EventBus::getInstance()->setEvent(baseEvent);
}

void SpringScrollViewNode::setSpringScrollViewNodeDelegate(SpringScrollViewNodeDelegate *springScrollViewNodeDelegate) {
    m_scrollNodeDelegate = springScrollViewNodeDelegate;
}

void SpringScrollViewNode::regsiteEventBus() {
    auto handler = std::make_shared<SpringScrollViewNode>();
    auto baseHandler = std::static_pointer_cast<EventBus::EventHandlerBase>(handler);
    EventBus::EventBus::getInstance()->registerHandler(baseHandler);
}

void SpringScrollViewNode::insertChild(ArkUINode &child, std::size_t index) {
    if (index == 0) {
        this->regsiteEventBus();
        m_stackArkUINodeHandle = NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_COLUMN);
        NativeNodeApi::getInstance()->addChild(m_nodeHandle, m_stackArkUINodeHandle);
        NativeNodeApi::getInstance()->insertChildAt(m_stackArkUINodeHandle, child.getArkUINodeHandle(), index);
        this->setChildWidth(m_scrollNodeDelegate->getLayoutSize().width);
        this->setChildHeight(m_scrollNodeDelegate->getLayoutSize().height);
        this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
        this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
        this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
        this->recordEventModel->setRefreshStatus(refreshStatus);
        this->recordEventModel->setLoadingStatus(loadingStatus);
        this->recordEventModel->setEventBounces(bounces);
        this->recordEventModel->setEventContentOffset(contentOffset);
        this->recordEventModel->setEventSize(size);
        this->recordEventModel->setEventContentSize(contentSize);
        this->recordEventModel->setEventContentInsets(contentInsets);
        this->recordEventModel->setEventBeginPoint(beginPoint);
        this->recordEventModel->setEventLastPoint(lastPoint);
        auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
        EventBus::EventBus::getInstance()->setEvent(baseEvent);
    }
}

void SpringScrollViewNode::removeChild(ArkUINode &child) {
    maybeThrow(NativeNodeApi::getInstance()->removeChild(m_stackArkUINodeHandle, child.getArkUINodeHandle()));
}

void SpringScrollViewNode::init() {
    if (this->isInitialContentOffset)
        return;
    this->registerPanGesture();
    if (this->initialContentOffset.y != 0) {
        this->isInitialContentOffset = true;
        this->setContentOffset(this->initialContentOffset.x, this->initialContentOffset.y);
    }
}

void SpringScrollViewNode::registerPanGesture() {
    auto anyGestureApi = OH_ArkUI_QueryModuleInterfaceByName(ARKUI_NATIVE_GESTURE, "ArkUI_NativeGestureAPI_1");
    this->panGestureApi = reinterpret_cast<ArkUI_NativeGestureAPI_1 *>(anyGestureApi);
    this->scrollPanGesture =
        this->panGestureApi->createPanGesture(1, GESTURE_DIRECTION_VERTICAL | GESTURE_DIRECTION_HORIZONTAL, 0);
    auto onPanActionCallBack = [](ArkUI_GestureEvent *event, void *extraParam) {
        SpringScrollViewNode *springScrollViewNode = (SpringScrollViewNode *)extraParam;
        ArkUI_GestureEventActionType actionType = OH_ArkUI_GestureEvent_GetActionType(event);
        DLOG(INFO) << "SpringScrollViewNode ArkUI_GestureEventActionType " << actionType;

        float x = OH_ArkUI_PanGesture_GetOffsetX(event);
        float y = OH_ArkUI_PanGesture_GetOffsetY(event);

        if (actionType == GESTURE_EVENT_ACTION_ACCEPT) {
                DLOG(INFO) << "SpringScrollViewNode GESTURE_EVENT_ACTION_ACCEPT x:" << x << " y:" << y;
                springScrollViewNode->downX = x;
                springScrollViewNode->downY = y;
                springScrollViewNode->onDown(event);
        } else if (actionType == GESTURE_EVENT_ACTION_UPDATE) {
                DLOG(INFO) << "SpringScrollViewNode GESTURE_EVENT_ACTION_UPDATE x:" << x << " y:" << y;
                springScrollViewNode->moveX = x;
                springScrollViewNode->moveY = y;
                if ((springScrollViewNode->moveX - springScrollViewNode->downX > 1.5) ||
                    (springScrollViewNode->moveX - springScrollViewNode->downX < -1.5)) {
                    springScrollViewNode->isMove = true;
                    springScrollViewNode->m_scrollNodeDelegate->setSwiperStatus(true);
                }
                if ((springScrollViewNode->moveY - springScrollViewNode->downY > 1.5) ||
                    (springScrollViewNode->moveY - springScrollViewNode->downY < -1.5)) {
                    springScrollViewNode->isMove = true;
                    springScrollViewNode->m_scrollNodeDelegate->setSwiperStatus(true);
                }
                springScrollViewNode->onMove(event);
        } else if (actionType == GESTURE_EVENT_ACTION_END) {
                springScrollViewNode->m_scrollNodeDelegate->setSwiperStatus(false);
                DLOG(INFO) << "SpringScrollViewNode onUp GESTURE_EVENT_ACTION_END";
                springScrollViewNode->onUp(event);
                springScrollViewNode->isMove = false;         
        }
    };
    this->panGestureApi->setGestureEventTarget(
        this->scrollPanGesture, GESTURE_EVENT_ACTION_ACCEPT | GESTURE_EVENT_ACTION_UPDATE | GESTURE_EVENT_ACTION_END,
        this, onPanActionCallBack);
    this->panGestureApi->addGestureToNode(this->m_nodeHandle, this->scrollPanGesture, PARALLEL, NORMAL_GESTURE_MASK);
}

void SpringScrollViewNode::onMove(ArkUI_GestureEvent *evt) {
    if (!this->scrollEnabled || !isMove) {
            return;
    }
    float x = OH_ArkUI_PanGesture_GetOffsetX(evt);
    float y = OH_ArkUI_PanGesture_GetOffsetY(evt);
    drag(lastPoint.x - x, lastPoint.y - y);
    lastPoint.x = x;
    lastPoint.y = y;
    if (this->m_scrollNodeDelegate && !this->scrollBeginDrag) {
        this->scrollBeginDrag = true;
        this->m_scrollNodeDelegate->onScrollBeginDrag();
    }
}

void SpringScrollViewNode::onDown(ArkUI_GestureEvent *evt) {
    auto recordEvent =
            std::static_pointer_cast<SpringScrollViewEvent>(EventBus::EventBus::getInstance()->getEvent());
    this->contentOffset = recordEvent->getEventContentOffset();
    this->recordSwipeY = recordEvent->getEventRecordSwipeY();
    refreshStatus = "waiting";
    if (recordEvent->getLoadingStatus() != "allLoaded") {
        loadingStatus = "waiting";
    }
    float x = OH_ArkUI_PanGesture_GetOffsetX(evt);
    float y = OH_ArkUI_PanGesture_GetOffsetY(evt);
    beginPoint.x = lastPoint.x = x;
    beginPoint.y = lastPoint.y = y;

    if (this->contentHeight <= m_scrollNodeDelegate->getLayoutSize().height) {
        contentOffset.y = 0;
        contentOffset.x = 0;
        contentInsets.top = 0;
    }
    if (cancelAllAnimations()) {
        dragging = true;
    }
    if (momentumScrolling) {
        momentumScrolling = false;
        this->m_scrollNodeDelegate->onMomentumScrollEnd();
        this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
        this->recordEventModel->setEventMomentumScrolling(false);
        this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
        this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
        this->recordEventModel->setRefreshStatus(refreshStatus);
        this->recordEventModel->setLoadingStatus(loadingStatus);
        this->recordEventModel->setEventBounces(bounces);
        this->recordEventModel->setEventContentOffset(contentOffset);
        this->recordEventModel->setEventSize(size);
        this->recordEventModel->setEventContentSize(contentSize);
        this->recordEventModel->setEventContentInsets(contentInsets);
        this->recordEventModel->setEventBeginPoint(beginPoint);
        this->recordEventModel->setEventLastPoint(lastPoint);
        this->recordEventModel->setEventRecordSwipeY(this->recordSwipeY);
        auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
        EventBus::EventBus::getInstance()->setEvent(baseEvent);
    }
    this->m_scrollNodeDelegate->onTouchBegin();
}

void SpringScrollViewNode::onUp(ArkUI_GestureEvent *evt) {
    this->scrollBeginDrag = false;
    if(!isMove) return;
    this->onMove(evt);
    dragging = false;
    float vy = 0.020;
    float vx = 0.020;
    draggingDirection = "";
    this->m_scrollNodeDelegate->onTouchEnd();
    if (!momentumScrolling) {
        momentumScrolling = true;
        this->m_scrollNodeDelegate->onMomentumScrollBegin();
    }
    if (shouldRefresh()) {
        refreshStatus = "refreshing";
        contentInsets.top = refreshHeaderHeight;
    }
    if (shouldLoad()) {
        loadingStatus = "loading";
        contentInsets.bottom = loadingFooterHeight;
    }
    if (!scrollEnabled) {
        return;
    }
    this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
    this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
    this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
    this->recordEventModel->setRefreshStatus(refreshStatus);
    this->recordEventModel->setLoadingStatus(loadingStatus);
    this->recordEventModel->setEventBounces(bounces);
    this->recordEventModel->setEventContentOffset(contentOffset);
    this->recordEventModel->setEventSize(size);
    this->recordEventModel->setEventContentSize(contentSize);
    this->recordEventModel->setEventContentInsets(contentInsets);
    this->recordEventModel->setEventBeginPoint(beginPoint);
    this->recordEventModel->setEventLastPoint(lastPoint);
    this->recordEventModel->setEventMomentumScrolling(momentumScrolling);
    auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
    EventBus::EventBus::getInstance()->setEvent(baseEvent);
    if (overshootVertical()) {
        this->beginOuterAnimation(vy);
    } else {
        this->beginInnerAnimation(vy);
    }
    if (contentSize.width <= size.width) {
        return;
    }
    if (overshootHorizontal()) {
        this->beginOuterHorizontalAnimation(vx);
    } else {
        this->beginInnerHorizontalAnimation(vx);
    }
}

void SpringScrollViewNode::beginInnerHorizontalAnimation(float initialVelocity) {
    if (abs(initialVelocity) < 0.1f) {
        return;
    }
    this->m_innerHorizontalAnimationStart = std::chrono::high_resolution_clock::now();
    float base = contentOffset.x;
    float v = initialVelocity;
    float dampingCoefficient = 0.997f;
    int duration = 0;
    float displacement = 0;
    while (abs(v) > 0.1f) {
        displacement += v;
        v *= dampingCoefficient;
        duration++;
    }
    this->m_scrollNodeDelegate->callArkTSInnerHorizontalAnimationStart(base, base - displacement, duration);
}

void SpringScrollViewNode::beginHorizontalReboundAnimation() {
    if (!overshootHorizontal() || !bounces) {
        return;
    }
    float endValue;
    if (overshootLeft()) {
        endValue = -contentInsets.left;
    } else {
        endValue = contentSize.width - size.width + contentInsets.right;
    }
    this->m_scrollNodeDelegate->callArkTSHorizontalReboundAnimationStart(contentOffset.x, endValue, 500);
}

void SpringScrollViewNode::beginOuterHorizontalAnimation(float initialVelocity) {
    if (abs(initialVelocity) < 0.1f) {
        beginHorizontalReboundAnimation();
        return;
    }
    if (initialVelocity > 15)
        initialVelocity = 15;
    if (initialVelocity < -15)
        initialVelocity = -15;
    float base = contentOffset.x;
    float v = initialVelocity;
    float dampingCoefficient = 0.9f;
    int duration = 0;
    float displacement = 0;
    while (abs(v) > 0.1f) {
        displacement += v;
        v *= dampingCoefficient;
        duration++;
    }
    this->m_scrollNodeDelegate->callArkTSOuterHorizontalAnimationStart(base, base - displacement, duration);
}

void SpringScrollViewNode::beginOuterAnimation(float initialVelocity) {

    if (abs(initialVelocity) < 0.1f) {
        beginReboundAnimation();
        return;
    }
    if (initialVelocity > 15) {
        initialVelocity = 15;
    }
    if (initialVelocity < -15) {
        initialVelocity = -15;
    }
    float base = contentOffset.y;
    float v = initialVelocity;
    float dampingCoefficient = 0.9f;
    int duration = 0;
    float displacement = 0;
    while (abs(v) > 0.1f) {
        displacement += v;
        v *= dampingCoefficient;
        duration++;
    }
    this->m_scrollNodeDelegate->callArkTSOuterAnimationStart(base, base - displacement, duration);
}

void SpringScrollViewNode::beginInnerAnimation(float initialVelocity) {
    if (abs(initialVelocity) < 0.1f) {
        if (momentumScrolling) {
            momentumScrolling = false;
            this->m_scrollNodeDelegate->onMomentumScrollEnd();
            this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
            this->recordEventModel->setEventMomentumScrolling(false);
            this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
            this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
            this->recordEventModel->setRefreshStatus(refreshStatus);
            this->recordEventModel->setLoadingStatus(loadingStatus);
            this->recordEventModel->setEventBounces(bounces);
            this->recordEventModel->setEventContentOffset(contentOffset);
            this->recordEventModel->setEventSize(size);
            this->recordEventModel->setEventContentSize(contentSize);
            this->recordEventModel->setEventContentInsets(contentInsets);
            this->recordEventModel->setEventBeginPoint(beginPoint);
            this->recordEventModel->setEventLastPoint(lastPoint);
            auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
            EventBus::EventBus::getInstance()->setEvent(baseEvent);
        }
        return;
    }
    float base = contentOffset.y;
    float v = initialVelocity;
    float dampingCoefficient = 0.997f;
    int duration = 0;
    float displacement = 0;
    while (abs(v) > 0.1f) {
        displacement += v;
        v *= dampingCoefficient;
        duration++;
    }
    this->m_initialVelocity = initialVelocity;
    this->m_innerAnimationStart = std::chrono::high_resolution_clock::now();
    this->m_scrollNodeDelegate->callArkTSInnerAnimationStart(base, base - displacement, duration);
}

void SpringScrollViewNode::beginReboundAnimation() {
    if (!overshootVertical() || !bounces) {
        return;
    }
    float endValue;
    if (overshootHead()) {
        endValue = -contentInsets.top;
    } else {
        endValue = contentSize.height - size.height + contentInsets.bottom;
    }
    this->m_scrollNodeDelegate->callArkTSReboundAnimationStart(contentOffset.y, endValue, 500);
}

bool SpringScrollViewNode::cancelAllAnimations() {
    m_scrollNodeDelegate->callArkTSAnimationCancel();
    return true;
}

void SpringScrollViewNode::drag(float x, float y) {
    if (x == 0 && y == 0) {
        return;
    }
    y = this->getYDampingCoefficient() * y;
    x = this->getXDampingCoefficient() * x;
    if (directionalLockEnabled) {
        if (draggingDirection.empty()) {
            if (abs(x) > abs(y)) {
                draggingDirection = "h";
            } else {
                draggingDirection = "v";
            }
        }
        if (draggingDirection == "h")
            y = 0;
        if (draggingDirection == "v")
            x = 0;
    }
    if (this->isOnloading) {
        contentOffset.y = this->recordSwipeY;
        this->isOnloading = false;
    }
    moveToOffset(contentOffset.x + x, contentOffset.y + y);
}

void SpringScrollViewNode::moveToOffset(float x, float y) {
    if (!scrollEnabled) {
        return;
    }
    if (!bounces) {
        if (y < -contentInsets.top)
            y = -contentInsets.top;
        if (y > contentSize.height - size.height + contentInsets.bottom)
            y = contentSize.height - size.height + contentInsets.bottom;
    }
    if (contentSize.width <= size.width || !bounces) {
        if (x < -contentInsets.left)
            x = -contentInsets.left;
        if (x > contentSize.width - size.width + contentInsets.right)
            x = contentSize.width - size.width + contentInsets.right;
    }
    if (contentOffset.y == y && contentOffset.x == x) {
        return;
    }
    if (shouldPulling()) {
        refreshStatus = "pulling";
    } else if (shouldPullingEnough()) {
        refreshStatus = "pullingEnough";
    } else if (shouldRefresh()) {
        refreshStatus = "refreshing";
        contentInsets.top = refreshHeaderHeight;
    } else if (shouldPullingCancel()) {
        refreshStatus = "pullingCancel";
    } else if (shouldWaiting()) {
        refreshStatus = "waiting";
    }
    if (shouldDragging()) {
        loadingStatus = "dragging";
    } else if (shouldDraggingEnough()) {
        loadingStatus = "draggingEnough";
    } else if (shouldDraggingCancel()) {
        loadingStatus = "draggingCancel";
    } else if (shouldFooterWaiting()) {
        loadingStatus = "waiting";
    }
    setContentOffset(x, y);
}

float SpringScrollViewNode::getPageWidth() { return pageSize.width <= 0 ? size.width : pageSize.width; }

float SpringScrollViewNode::getPageHeight() { return pageSize.height <= 0 ? size.height : pageSize.height; }

bool SpringScrollViewNode::shouldPulling() {
    return refreshHeaderHeight > 0 && overshootHead() &&
           (refreshStatus == "waiting" || refreshStatus == "pullingCancel");
}

bool SpringScrollViewNode::shouldPullingEnough() {
    return refreshHeaderHeight > 0 && overshootRefresh() && refreshStatus == "pulling";
}

bool SpringScrollViewNode::shouldRefresh() {
    return !dragging && refreshHeaderHeight > 0 && overshootRefresh() && refreshStatus == "pullingEnough";
}

bool SpringScrollViewNode::shouldPullingCancel() {
    return refreshHeaderHeight > 0 && refreshStatus == "pullingEnough" && overshootHead() && !overshootRefresh();
}

bool SpringScrollViewNode::shouldWaiting() {
    return refreshHeaderHeight > 0 && !overshootHead() &&
           (refreshStatus == "rebound" || refreshStatus == "pullingCancel");
}

bool SpringScrollViewNode::shouldDragging() {
    return loadingFooterHeight > 0 && overshootFooter() &&
           (loadingStatus == "waiting" || loadingStatus == "draggingCancel");
}

bool SpringScrollViewNode::shouldDraggingEnough() {
    return loadingFooterHeight > 0 && overshootLoading() && loadingStatus == "dragging";
}

bool SpringScrollViewNode::shouldLoad() {
    return loadingFooterHeight > 0 && overshootLoading() && loadingStatus == "draggingEnough";
}

bool SpringScrollViewNode::shouldDraggingCancel() {
    return loadingFooterHeight > 0 && loadingStatus == "draggingEnough" && overshootFooter() && !overshootLoading();
}

bool SpringScrollViewNode::shouldFooterWaiting() {
    return loadingFooterHeight > 0 && !overshootFooter() &&
           (loadingStatus == ("rebound") || loadingStatus == ("draggingCancel"));
}

bool SpringScrollViewNode::canHorizontalScroll() { return scrollEnabled && contentSize.width > size.width; }

bool SpringScrollViewNode::overshootVertical() { return overshootHead() || overshootFooter(); }

bool SpringScrollViewNode::overshootHead() { return contentOffset.y < -contentInsets.top; }

bool SpringScrollViewNode::overshootFooter() { return contentOffset.y > contentSize.height - size.height; }

bool SpringScrollViewNode::overshootLoading() {
    return contentOffset.y > -size.height + contentSize.height + loadingFooterHeight;
}

bool SpringScrollViewNode::overshootRefresh() { return contentOffset.y < -contentInsets.top - refreshHeaderHeight; }

bool SpringScrollViewNode::overshootLeft() { return contentOffset.x < -contentInsets.left; }

bool SpringScrollViewNode::overshootRight() {
    return contentOffset.x > contentInsets.right + contentSize.width - size.width;
}

bool SpringScrollViewNode::overshootHorizontal() { return overshootLeft() || overshootRight(); }

float SpringScrollViewNode::getYDampingCoefficient() {
    if (!overshootVertical()) {
        return 1;
    }
    float overshoot = overshootHead() ? -contentOffset.y : contentOffset.y - contentSize.height + size.height;
    float c = 0.8f;
    return c / (size.height * size.height) * (overshoot * overshoot) - 2 * c / size.height * overshoot + c;
}

float SpringScrollViewNode::getXDampingCoefficient() {
    float overshoot;
    if (overshootLeft()) {
        overshoot = -contentOffset.x;
    } else if (overshootRight()) {
        overshoot = contentOffset.x - contentSize.width + size.width;
    } else {
        return 1;
    }
    float c = 0.8f;
    return c / (size.width * size.width) * (overshoot * overshoot) - 2 * c / size.width * overshoot + c;
}

void SpringScrollViewNode ::setContentOffset(float x, float y) {
    if(this->isDestory || !m_stackArkUINodeHandle || !m_scrollNodeDelegate) return;
    this->contentOffset.x = x;
    this->contentOffset.y = y;
    this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
    this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
    this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
    this->recordEventModel->setRefreshStatus(refreshStatus);
    this->recordEventModel->setLoadingStatus(loadingStatus);
    this->recordEventModel->setEventBounces(bounces);
    this->recordEventModel->setEventContentOffset(contentOffset);
    this->recordEventModel->setEventRecordSwipeY(contentOffset.y);
    auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
    EventBus::EventBus::getInstance()->setEvent(baseEvent);
    std::array<ArkUI_NumberValue, 3> translateValue = {
            ArkUI_NumberValue{.f32 = -this->contentOffset.x}, {.f32 = -this->contentOffset.y}, {.f32 = 0}};
        ArkUI_AttributeItem translateItem = {translateValue.data(), translateValue.size()};
    NativeNodeApi::getInstance()->setAttribute(m_stackArkUINodeHandle, NODE_TRANSLATE, &translateItem);
    facebook::react::RNCSpringScrollViewEventEmitter::OnScroll onScroll = {
        {contentOffset.x / 2, contentOffset.y / 2}, refreshStatus, loadingStatus};
    m_scrollNodeDelegate->onScroll(onScroll);
    DLOG(INFO) << "SpringScrollViewNode setContentOffset loadingStatus:" << loadingStatus
               << " refreshStatus:" << refreshStatus << " contentOffset.y " << contentOffset.y << " contentOffset.x "
               << contentOffset.x;
}

void SpringScrollViewNode ::setLoadingFooterHeight(float height) { this->loadingFooterHeight = height; }

void SpringScrollViewNode ::setAllLoaded(bool allLoaded) {
    loadingStatus = allLoaded ? "allLoaded" : "waiting";
    if (allLoaded) {
        contentInsets.bottom = 0;
    }
    this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
    this->recordEventModel->setEventMomentumScrolling(false);
    this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
    this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
    this->recordEventModel->setRefreshStatus(refreshStatus);
    this->recordEventModel->setLoadingStatus(loadingStatus);
    this->recordEventModel->setEventBounces(bounces);
    this->recordEventModel->setEventContentOffset(contentOffset);
    this->recordEventModel->setEventSize(size);
    this->recordEventModel->setEventContentSize(contentSize);
    this->recordEventModel->setEventContentInsets(contentInsets);
    this->recordEventModel->setEventBeginPoint(beginPoint);
    this->recordEventModel->setEventLastPoint(lastPoint);
    auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
    EventBus::EventBus::getInstance()->setEvent(baseEvent);
}

void SpringScrollViewNode ::setDecelerationRate(float rate) { decelerationRate = rate; }

void SpringScrollViewNode ::setInitialContentOffset(float x, float y) {
    initialContentOffset.x = x;
    initialContentOffset.y = y;
}

void SpringScrollViewNode ::setRefreshHeaderHeight(float v) { this->refreshHeaderHeight = v; }

void SpringScrollViewNode ::setBounces(bool v) { this->bounces = v; }

void SpringScrollViewNode ::setScrollEnabled(bool v) { this->scrollEnabled = v; }

void SpringScrollViewNode ::setInverted(bool v) { this->inverted = v; }

void SpringScrollViewNode ::setDirectionalLockEnabled(bool v) { this->directionalLockEnabled = v; }

void SpringScrollViewNode ::endLoading() {
    this->isOnloading = true;
    if (loadingStatus != "loading") {
        return;
    }
    loadingStatus = "rebound";
    this->recordSwipeY = contentOffset.y;
    auto recordEvent = std::static_pointer_cast<SpringScrollViewEvent>(EventBus::EventBus::getInstance()->getEvent());
    this->contentOffset = recordEvent->getEventContentOffset();
    this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
    this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
    this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
    this->recordEventModel->setRefreshStatus(refreshStatus);
    this->recordEventModel->setLoadingStatus(loadingStatus);
    this->recordEventModel->setEventBounces(bounces);
    this->recordEventModel->setEventContentOffset(contentOffset);
    this->recordEventModel->setEventSize(size);
    this->recordEventModel->setEventContentSize(contentSize);
    this->recordEventModel->setEventContentInsets(contentInsets);
    this->recordEventModel->setEventBeginPoint(beginPoint);
    this->recordEventModel->setEventLastPoint(lastPoint);
    this->recordEventModel->setEventRecordSwipeY(contentOffset.y);
    auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
    EventBus::EventBus::getInstance()->setEvent(baseEvent);
    this->m_scrollNodeDelegate->callArkTSAnimationCancel();
    contentInsets.bottom = 0;
    m_scrollNodeDelegate->callArkTSEndLoadingStart(contentOffset.y, contentSize.height - size.height, 500);
}

void SpringScrollViewNode ::endRefresh() {
    if (refreshStatus != "refreshing") {
        return;
    }
    refreshStatus = "rebound";
    auto recordEvent = std::static_pointer_cast<SpringScrollViewEvent>(EventBus::EventBus::getInstance()->getEvent());
    this->contentOffset = recordEvent->getEventContentOffset();
    this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
    this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
    this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
    this->recordEventModel->setRefreshStatus(refreshStatus);
    this->recordEventModel->setLoadingStatus(loadingStatus);
    this->recordEventModel->setEventBounces(bounces);
    this->recordEventModel->setEventContentOffset(contentOffset);
    this->recordEventModel->setEventSize(size);
    this->recordEventModel->setEventContentSize(contentSize);
    this->recordEventModel->setEventContentInsets(contentInsets);
    this->recordEventModel->setEventBeginPoint(beginPoint);
    this->recordEventModel->setEventLastPoint(lastPoint);
    auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
    EventBus::EventBus::getInstance()->setEvent(baseEvent);
    contentInsets.top = 0;
    m_scrollNodeDelegate->callArkTSEndRefreshStart(contentOffset.y, 0, 500);
    contentOffset.y = 0;
    contentOffset.x = 0;
}

   void SpringScrollViewNode ::scrollTo(float x, float y, bool animated) {
        this->setRecordEventModel();
        if(x == BEGIN_REFRESH){
            if (!this->bounces) return;
            m_scrollNodeDelegate->callArkTSScrollYStart(0, y, 500);
            this->beginRefresh();
            return;
        }
        if( x == DISMINATE_KEYBOARD_SHOW_HIDE && this->recordKeyBoardShow) return;
        if(y == DISMINATE_KEYBOARD_SHOW_HIDE){
             this->recordKeyBoardShow = false;
             y = this->recordKeyBoardPositioY;
             this->keyboardHideLastTime = std::chrono::high_resolution_clock::now();
        }
        else if(x == DISMINATE_KEYBOARD_SHOW_HIDE){
             auto keyboardShowCurrentTime = std::chrono::high_resolution_clock::now();
             std::chrono::duration<double> duration = keyboardShowCurrentTime - this->keyboardHideLastTime;
             this->recordKeyBoardShow = true;
             if(duration.count() > 1.5) {
               this->recordKeyBoardPositioY = contentOffset.y;
            }
        }
        cancelAllAnimations();
        if (!animated) {
            moveToOffset(x, y);
            return;
        }
        m_scrollNodeDelegate->callArkTSScrollYStart(contentOffset.y, y, 500);
        if (x != contentOffset.x && x!=DISMINATE_KEYBOARD_SHOW_HIDE) {
            m_scrollNodeDelegate->callArkTSScrollXStart(contentOffset.x, x, 500);
        }
    }

void SpringScrollViewNode::onEvent(std::shared_ptr<SpringScrollViewEvent> &event) {
    if(this->isDestory) return;
    auto recordEvent = std::static_pointer_cast<SpringScrollViewEvent>(EventBus::EventBus::getInstance()->getEvent());
    this->m_stackArkUINodeHandle = recordEvent->getNodeHandle();
    this->m_scrollNodeDelegate = (SpringScrollViewNodeDelegate *)recordEvent->getEventSpringScrollViewNodeDelegate();
    this->bounces = recordEvent->getEventBounces();
    this->refreshStatus = recordEvent->getRefreshStatus();
    this->loadingStatus = recordEvent->getLoadingStatus();
    this->beginPoint = recordEvent->getEventBeginPoint();
    this->lastPoint = recordEvent->getEventLastPoint();
    this->contentInsets = recordEvent->getEventContentInsets();
    this->size = recordEvent->getEventSize();
    this->momentumScrolling = recordEvent->getEventMomentumScrolling();
    this->recordSwipeY = recordEvent->getEventRecordSwipeY();
    DLOG(INFO) << "SpringScrollViewNode onEvent " << event->getMessageType() << " getEventType "
               << event->getEventType();

    if (event->getMessageType() == "onUpdate" && event->getEventType() == "OuterAnimation") {
        double value = event->getAnimationValue();
        if (!bounces) {
            contentOffset.y = value;
            if (overshootHead()) {
                value = -contentInsets.top;
                m_scrollNodeDelegate->callArkTSAnimationCancel();
            } else if (overshootFooter()) {
                value = contentSize.height - size.height + contentInsets.bottom;
                m_scrollNodeDelegate->callArkTSAnimationCancel();
            }
        }
        contentOffset.x = recordEvent->getEventContentOffset().x;
        this->setContentOffset(contentOffset.x, value);
    }

    if (event->getMessageType() == "onUpdate" && event->getEventType() == "ReboundAnimation") {
        contentOffset.x = recordEvent->getEventContentOffset().x;
        this->setContentOffset(contentOffset.x, event->getAnimationValue());
    }

    if (event->getMessageType() == "onUpdate" && event->getEventType() == "InnerAnimation") {
        contentOffset.x = recordEvent->getEventContentOffset().x;
        setContentOffset(contentOffset.x, event->getAnimationValue());
        if (overshootHead() || overshootFooter()) {
            std::chrono::duration<double> duration =
                std::chrono::high_resolution_clock::now() - this->m_innerAnimationStart;
            long interval = duration.count();
            float v = this->m_initialVelocity;
            while (interval-- > 0) {
                v *= 0.997f;
            }
            m_scrollNodeDelegate->callArkTSAnimationCancel();
            beginOuterAnimation(v);
        }
    }
    
    if (event->getMessageType() == "onUpdate" && event->getEventType() == "OuterHorizontalAnimation") {
        float value = event->getAnimationValue();
        if (!bounces) {
            contentOffset.x = value;
            if (overshootLeft()) {
                value = -contentInsets.left;
                m_scrollNodeDelegate->callArkTSAnimationCancel();
            } else if (overshootRight()) {
                value = contentSize.width - size.width + contentInsets.right;
                m_scrollNodeDelegate->callArkTSAnimationCancel();
            }
        }
        contentOffset.y = recordEvent->getEventContentOffset().y;
        setContentOffset(value, contentOffset.y);
    }

    if (event->getMessageType() == "onUpdate" && event->getEventType() == "HorizontalReboundAnimation") {
        contentOffset.y = recordEvent->getEventContentOffset().y;
        setContentOffset(event->getAnimationValue(), contentOffset.y);
    }

    if (event->getMessageType() == "onUpdate" && event->getEventType() == "InnerHorizontalAnimation") {
        contentOffset.y = recordEvent->getEventContentOffset().y;
        setContentOffset(event->getAnimationValue(), contentOffset.y);
        if (overshootHorizontal()) {
            std::chrono::duration<double> duration =
                std::chrono::high_resolution_clock::now() - this->m_innerHorizontalAnimationStart;
            long interval = duration.count();
            float v = this->m_initialVelocity;
            while (interval-- > 0) {
                v *= 0.997f;
            }
            m_scrollNodeDelegate->callArkTSAnimationCancel();
            beginOuterHorizontalAnimation(v);
        }
    }

    if (event->getMessageType() == "onUpdate" && event->getEventType() == "EndRefresh") {
        contentOffset.x = recordEvent->getEventContentOffset().x;
        this->setContentOffset(contentOffset.x, event->getAnimationValue());
    }

    if (event->getMessageType() == "onUpdate" && event->getEventType() == "EndLoading") {
        contentOffset.x = recordEvent->getEventContentOffset().x;
        this->setContentOffset(contentOffset.x, event->getAnimationValue());
    }

    if (event->getMessageType() == "onEnd" && event->getEventType() == "OuterAnimation") {
        if (this->refreshStatus == "refreshing")
            return;
        this->beginReboundAnimation();
    }

    if (event->getMessageType() == "onUpdate" && event->getEventType() == "ScrollY") {
        contentOffset.x = recordEvent->getEventContentOffset().x;
        setContentOffset(contentOffset.x, event->getAnimationValue());
    }

    if (event->getMessageType() == "onUpdate" && event->getEventType() == "ScrollX") {
        contentOffset.y = recordEvent->getEventContentOffset().y;
        setContentOffset(event->getAnimationValue(), contentOffset.y);
    }

    if (event->getMessageType() == "onEnd" && event->getEventType() == "ReboundAnimation") {
        if (momentumScrolling) {
            momentumScrolling = false;
            m_scrollNodeDelegate->onMomentumScrollEnd();
            this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
            this->recordEventModel->setEventMomentumScrolling(false);
            this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
            this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
            this->recordEventModel->setRefreshStatus(refreshStatus);
            this->recordEventModel->setLoadingStatus(loadingStatus);
            this->recordEventModel->setEventBounces(bounces);
            this->recordEventModel->setEventContentOffset(contentOffset);
            this->recordEventModel->setEventSize(size);
            this->recordEventModel->setEventContentSize(contentSize);
            this->recordEventModel->setEventContentInsets(contentInsets);
            this->recordEventModel->setEventBeginPoint(beginPoint);
            this->recordEventModel->setEventLastPoint(lastPoint);
            auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
            EventBus::EventBus::getInstance()->setEvent(baseEvent);
        }
    }

    if (event->getMessageType() == "onEnd" && event->getEventType() == "InnerAnimation") {
        if (momentumScrolling) {
            momentumScrolling = false;
            m_scrollNodeDelegate->onMomentumScrollEnd();
            this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
            this->recordEventModel->setEventMomentumScrolling(false);
            this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
            this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
            this->recordEventModel->setRefreshStatus(refreshStatus);
            this->recordEventModel->setLoadingStatus(loadingStatus);
            this->recordEventModel->setEventBounces(bounces);
            this->recordEventModel->setEventContentOffset(contentOffset);
            this->recordEventModel->setEventSize(size);
            this->recordEventModel->setEventContentSize(contentSize);
            this->recordEventModel->setEventContentInsets(contentInsets);
            this->recordEventModel->setEventBeginPoint(beginPoint);
            this->recordEventModel->setEventLastPoint(lastPoint);
            auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
            EventBus::EventBus::getInstance()->setEvent(baseEvent);
        }
    }

    if (event->getMessageType() == "onEnd" && event->getEventType() == "OuterHorizontalAnimation") {
        this->beginHorizontalReboundAnimation();
    }

    if (event->getMessageType() == "onEnd" && event->getEventType() == "OuterHorizontalAnimation") {
        if (momentumScrolling) {
            momentumScrolling = false;
            m_scrollNodeDelegate->onMomentumScrollEnd();
            this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
            this->recordEventModel->setEventMomentumScrolling(false);
            this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
            this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
            this->recordEventModel->setRefreshStatus(refreshStatus);
            this->recordEventModel->setLoadingStatus(loadingStatus);
            this->recordEventModel->setEventBounces(bounces);
            this->recordEventModel->setEventContentOffset(contentOffset);
            this->recordEventModel->setEventSize(size);
            this->recordEventModel->setEventContentSize(contentSize);
            this->recordEventModel->setEventContentInsets(contentInsets);
            this->recordEventModel->setEventBeginPoint(beginPoint);
            this->recordEventModel->setEventLastPoint(lastPoint);
            auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
            EventBus::EventBus::getInstance()->setEvent(baseEvent);
        }
    }

    if (event->getMessageType() == "onEnd" && event->getEventType() == "InnerHorizontalAnimation") {
        if (momentumScrolling) {
            momentumScrolling = false;
            m_scrollNodeDelegate->onMomentumScrollEnd();
            this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
            this->recordEventModel->setEventMomentumScrolling(false);
            this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
            this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
            this->recordEventModel->setRefreshStatus(refreshStatus);
            this->recordEventModel->setLoadingStatus(loadingStatus);
            this->recordEventModel->setEventBounces(bounces);
            this->recordEventModel->setEventContentOffset(contentOffset);
            this->recordEventModel->setEventSize(size);
            this->recordEventModel->setEventContentSize(contentSize);
            this->recordEventModel->setEventContentInsets(contentInsets);
            this->recordEventModel->setEventBeginPoint(beginPoint);
            this->recordEventModel->setEventLastPoint(lastPoint);
            auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
            EventBus::EventBus::getInstance()->setEvent(baseEvent);
        }
    }

    if (event->getMessageType() == "onEnd" && event->getEventType() == "EndRefresh") {
        this->contentInsets.top = 0;
        this->contentOffset.y = 0;
        this->contentOffset.x = 0;
    }
}

void SpringScrollViewNode::setContentSize(Types::Size contentSize) { this->contentSize = contentSize; }

void SpringScrollViewNode::setScreenSize(Types::Size size) { this->size = size; }

void SpringScrollViewNode::setChildHeight(float height) {
    ArkUI_NumberValue heightValue[] = {{.f32 = height}};
    ArkUI_AttributeItem heightItem = {heightValue, sizeof(heightValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(this->m_stackArkUINodeHandle, NODE_HEIGHT, &heightItem));
}

void SpringScrollViewNode::setChildWidth(float width) {
    ArkUI_NumberValue widthValue[] = {{.f32 = width}};
    ArkUI_AttributeItem widthItem = {widthValue, sizeof(widthValue) / sizeof(ArkUI_NumberValue)};
    maybeThrow(NativeNodeApi::getInstance()->setAttribute(this->m_stackArkUINodeHandle, NODE_WIDTH, &widthItem));
}

void SpringScrollViewNode::setContentHeight(float height) { this->contentHeight = height; }

void SpringScrollViewNode::beginRefresh() {
    this->scrollBeginDrag = false;
    dragging = false;
    float vy = 0.020;
    draggingDirection = "";
    if (!momentumScrolling) {
        momentumScrolling = true;
        this->m_scrollNodeDelegate->onMomentumScrollBegin();
    }
    refreshStatus = "refreshing";
    contentInsets.top = refreshHeaderHeight;
    this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
    this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
    this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
    this->recordEventModel->setRefreshStatus(refreshStatus);
    this->recordEventModel->setLoadingStatus(loadingStatus);
    this->recordEventModel->setEventBounces(bounces);
    this->recordEventModel->setEventContentOffset(contentOffset);
    this->recordEventModel->setEventSize(size);
    this->recordEventModel->setEventContentSize(contentSize);
    this->recordEventModel->setEventContentInsets(contentInsets);
    this->recordEventModel->setEventBeginPoint(beginPoint);
    this->recordEventModel->setEventLastPoint(lastPoint);
    this->recordEventModel->setEventMomentumScrolling(momentumScrolling);
    auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
    EventBus::EventBus::getInstance()->setEvent(baseEvent);
    this->beginOuterAnimation(vy);
}


void SpringScrollViewNode::setRecordEventModel() {
    auto recordEvent = std::static_pointer_cast<SpringScrollViewEvent>(EventBus::EventBus::getInstance()->getEvent());
    this->contentOffset = recordEvent->getEventContentOffset();
    this->recordEventModel = std::make_shared<SpringScrollViewEvent>(5);
    this->recordEventModel->setNodeHandle(m_stackArkUINodeHandle);
    this->recordEventModel->setEventSpringScrollViewNodeDelegate(this->m_scrollNodeDelegate);
    this->recordEventModel->setRefreshStatus(refreshStatus);
    this->recordEventModel->setLoadingStatus(loadingStatus);
    this->recordEventModel->setEventBounces(bounces);
    this->recordEventModel->setEventContentOffset(contentOffset);
    this->recordEventModel->setEventSize(size);
    this->recordEventModel->setEventContentSize(contentSize);
    this->recordEventModel->setEventContentInsets(contentInsets);
    this->recordEventModel->setEventBeginPoint(beginPoint);
    this->recordEventModel->setEventLastPoint(lastPoint);
    this->recordEventModel->setEventMomentumScrolling(momentumScrolling);
    auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
    EventBus::EventBus::getInstance()->setEvent(baseEvent);
}

} // namespace rnoh