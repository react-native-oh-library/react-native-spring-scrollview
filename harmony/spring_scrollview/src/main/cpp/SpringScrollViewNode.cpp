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
    this->recordEventModel->setEventDirections(true);
    this->recordEventModel->setEventIsOnloading(false);
    this->recordEventModel->setEventRecordSwipeY(0);
    this->recordEventModel->setEventDampingCoefficient(0);
    this->recordEventModel->setEventMomentumScrolling(false);
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
        this->m_scrollNodeDelegate->onCustomScrollBeginDrag();
    }
}

void SpringScrollViewNode::onDown(ArkUI_GestureEvent *evt) {
    auto recordEvent =
            std::static_pointer_cast<SpringScrollViewEvent>(EventBus::EventBus::getInstance()->getEvent());
    this->contentOffset = recordEvent->getEventContentOffset();
    this->recordSwipeY = recordEvent->getEventRecordSwipeY();
    this->isOnloading = recordEvent->getEventIsOnloading();
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
         this->setRecordEventModel();
    }
    this->m_scrollNodeDelegate->onCustomTouchBegin();
}

void SpringScrollViewNode::onUp(ArkUI_GestureEvent *evt) {
    this->scrollBeginDrag = false;
    if(!isMove) return;
    this->onMove(evt);
    dragging = false;
    float vy = OH_ArkUI_PanGesture_GetVelocityY(evt)/1000;
    float vx = OH_ArkUI_PanGesture_GetVelocityX(evt)/1000;
    this->m_scrollNodeDelegate->onCustomTouchEnd();
    this->m_scrollNodeDelegate->onCustomScrollEndDrag();
    if (!momentumScrolling) {
        momentumScrolling = true;
        this->m_scrollNodeDelegate->onCustomMomentumScrollBegin();
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
    this->m_dampingCoefficient = pagingEnabled ? 0.99f : decelerationRate;
    this->setRecordEventModel();
    float maxY = contentSize.height + contentInsets.bottom - size.height;
    draggingDirection = "";
        this->m_scrollNodeDelegate->callArkTSInnerStart(contentOffset.y, -vy, pagingEnabled ? 0.99f :
        decelerationRate, -contentInsets.top,
                                     maxY, pagingEnabled, getPageHeight(),true);

    if (contentSize.width <= size.width) {
        return;
    }    
    float maxX = contentSize.width - size.width + contentInsets.right;
    this->m_scrollNodeDelegate->callArkTSInnerStart(contentOffset.x, -vx, pagingEnabled ? 0.99f : decelerationRate, -contentInsets.left,
                                   maxX, pagingEnabled, getPageWidth(),false);
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
    DLOG(INFO) << " SpringScrollViewNode drag isOnloading " << isOnloading << " recordSwipeY " << recordSwipeY;
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
    return overshootVertical() ? 0.5f : 1.0f;
}

float SpringScrollViewNode::getXDampingCoefficient() {
    return overshootLeft() || overshootRight() ? 0.5f : 1.0f;
}

void SpringScrollViewNode ::setContentOffset(float x, float y) {
    if(this->isDestory || !m_stackArkUINodeHandle || !m_scrollNodeDelegate) return;
    this->contentOffset.x = x;
    this->contentOffset.y = y;
      this->setRecordEventModel();  
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
    momentumScrolling = false;
    this->setRecordEventModel();
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

void SpringScrollViewNode ::endLoading(bool rebound) {
    this->isOnloading = true;
    if (loadingStatus != "loading") {
        return;
    }
    loadingStatus = rebound ? "rebound" : "waiting";
    this->recordSwipeY = contentOffset.y;
    this->setRecordEventModel();
    this->m_scrollNodeDelegate->callArkTSAnimationCancel();
    m_scrollNodeDelegate->callArkTSReboundStart(contentOffset.y, contentSize.height - size.height + (rebound ? 0 : contentInsets.bottom), 500,true);
    contentInsets.bottom = 0;
}

void SpringScrollViewNode ::endRefresh() {
    if (refreshStatus != "refreshing") {
        return;
    }
    refreshStatus = "rebound";
    this->setRecordEventModel();
    contentInsets.top = 0;
    m_scrollNodeDelegate->callArkTSReboundStart(contentOffset.y, 0, 500,true);
    contentOffset.y = 0;
    contentOffset.x = 0;
}

  void SpringScrollViewNode ::scrollTo(float x, float y, bool animated) {
       DLOG(INFO) << "SpringScrollViewNode scrollTo x:" << x << " y " << y << " animated " << animated; 
        this->setRecordEventModel();
        if(x == BEGIN_REFRESH){
            if (!this->bounces) return;
            this->beginRefresh();
            m_scrollNodeDelegate->callArkTSReboundStart(0, y, 500,true);
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
        m_scrollNodeDelegate->callArkTSReboundStart(contentOffset.y, y, 500,true);
        if (x != contentOffset.x && x!=DISMINATE_KEYBOARD_SHOW_HIDE) {
            m_scrollNodeDelegate->callArkTSReboundStart(contentOffset.x, x, 500,false);
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
    this->m_dampingCoefficient = recordEvent->getEventDampingCoefficient();
    this->m_initialVelocity = recordEvent->getEventInitialVelocity();
    this->contentSize = recordEvent->getEventContentSize();
    this->contentOffset = recordEvent->getEventContentOffset();
    this->m_Directions = event->getEventDirections();
    DLOG(INFO) << "SpringScrollViewNode onEvent " << event->getMessageType() << " getEventType "
               << event->getEventType() << " getAnimationValue "<< event->getAnimationValue() << " x " <<  recordEvent->getEventContentOffset().x  << " y " << recordEvent->getEventContentOffset().y << " isVertical " << event->getEventDirections() ;
    if (event->getMessageType() == "onUpdate" && event->getEventType() == "OuterAnimation") {
        if (this->m_Directions) {
               setContentOffset(recordEvent->getEventContentOffset().x, event->getAnimationValue());
            } else {
               setContentOffset(event->getAnimationValue(), recordEvent->getEventContentOffset().y);
        }
    }
    
    if (event->getMessageType() == "onUpdate" && event->getEventType() == "ReboundAnimation") {
        if (this->m_Directions) {
               setContentOffset(recordEvent->getEventContentOffset().x, event->getAnimationValue());
        } else {
               setContentOffset(event->getAnimationValue(), recordEvent->getEventContentOffset().y);
        }
    }

    if (event->getMessageType() == "onUpdate" && event->getEventType() == "InnerAnimation") {
        if(this->m_Directions){
            setContentOffset(recordEvent->getEventContentOffset().x, event->getAnimationValue());
        }
        else{
            setContentOffset(event->getAnimationValue(),recordEvent->getEventContentOffset().y);
        }
    }

    if (event->getMessageType() == "onEnd" && event->getEventType() == "OuterAnimation") {
        this->setRecordEventModel();

        if (this->m_Directions) {
            if (shouldRefresh()) {
                refreshStatus = "refreshing";
                contentInsets.top = refreshHeaderHeight;
            }
            float to = contentOffset.y < -contentInsets.top ? -contentInsets.top
                                                            : contentSize.height - size.height + contentInsets.bottom;
            this->m_scrollNodeDelegate->callArkTSReboundStart(contentOffset.y, to, 500,true);
        } else {
            float to = contentOffset.x < -contentInsets.left ? -contentInsets.left
                                                             : contentSize.width - size.width + contentInsets.right;
            this->m_scrollNodeDelegate->callArkTSReboundStart(contentOffset.x, to, 500,false);
        }
    }
    
    if (event->getMessageType() == "onEnd" && event->getEventType() == "ReboundAnimation") {
        this->setRecordEventModel();
        if (this->m_Directions) {
            onVerticalAnimationEnd();
         }
        else{
            onHorizontalAnimationEnd();
        }
    }

    if (event->getMessageType() == "onEnd" && event->getEventType() == "InnerAnimation") {
        if(this->m_Directions){
            if (bounces && overshootVertical()) {
                this->m_scrollNodeDelegate->callArkTSOuterStart(contentOffset.y, recordEvent->getAnimationValue(), 0.9f,true);
            } else {
                onVerticalAnimationEnd();
            }
        }
        else{
            if (bounces && overshootHorizontal()) {
                this->m_scrollNodeDelegate->callArkTSOuterStart(contentOffset.x, recordEvent->getAnimationValue(), 0.9f,false);
            } else {
                onHorizontalAnimationEnd();
            }
        }

        this->setRecordEventModel();
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
    refreshStatus = "refreshing";
    contentInsets.top = refreshHeaderHeight;
    this->setRecordEventModel();
}


void SpringScrollViewNode::setRecordEventModel() {
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
    this->recordEventModel->setEventDirections(m_Directions);
    this->recordEventModel->setEventRecordSwipeY(recordSwipeY);
    this->recordEventModel->setEventDampingCoefficient(m_dampingCoefficient);
    this->recordEventModel->setEventInitialVelocity(m_initialVelocity);
    this->recordEventModel->setEventIsOnloading(isOnloading);
    auto baseEvent = std::static_pointer_cast<EventBus::Event>(this->recordEventModel);
    EventBus::EventBus::getInstance()->setEvent(baseEvent);
}

void SpringScrollViewNode::setPagingEnabled(bool  pagingEnabled) { this->pagingEnabled = pagingEnabled; }

void SpringScrollViewNode::setPageSize(float width, float height) {
    this->pageSize.width = width;
    this->pageSize.height = height;
}

void SpringScrollViewNode::onHorizontalAnimationEnd() {
    if (momentumScrolling) {
        momentumScrolling = false;
        this->m_scrollNodeDelegate->onCustomMomentumScrollEnd();
    }
}


void SpringScrollViewNode::onVerticalAnimationEnd() {
    if (momentumScrolling) {
        momentumScrolling = false;
        this->m_scrollNodeDelegate->onCustomMomentumScrollEnd();
    }
}



} // namespace rnoh