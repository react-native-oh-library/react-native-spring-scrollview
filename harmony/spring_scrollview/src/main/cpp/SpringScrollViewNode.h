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
#pragma once

#include "Props.h"
#include "RNOH/arkui/ArkUINode.h"
#include "RNOH/arkui/StackNode.h"
#include "EventEmitters.h"
#include "Types.h"
#include "RNOH/EventDispatcher.h"
#include <arkui/native_gesture.h>
#include <memory>
#include <EventBus/EventHandler.h>
#include <EventBus/EventBus.h>
#include <SpringScrollViewEvent.h>

namespace rnoh {
class SpringScrollViewNodeDelegate {
public:
    virtual ~SpringScrollViewNodeDelegate() = default;
    virtual void onRefresh(){};
    virtual void onLoading(){};
    virtual void onScroll(const facebook::react::RNCSpringScrollViewEventEmitter::OnScroll &onScroll){};
    virtual void onSizeChange(const float &displayedScrollHeight){};
    virtual void onContentSizeChange(const float &displayedScrollHeight){};
    virtual bool isComponentTop(){};
    virtual void callArkTSAnimationCancel(){};
    virtual facebook::react::Size getLayoutSize(){};
    virtual void setSwiperStatus(bool swiperStatus){};
    virtual void onCustomScrollBeginDrag(){};
    virtual void onCustomScrollEndDrag(){};
    virtual void onCustomTouchBegin(){};
    virtual void onCustomTouchEnd(){};
    virtual void onCustomMomentumScrollBegin(){};
    virtual void onCustomMomentumScrollEnd(){};

    virtual void callArkTSInnerStart(float f,  float v0,  float d, float lower, float upper, bool pagingEnabled, float pageSize,bool isVertical){};
    virtual void callArkTSOuterStart(float f, float v0, float d,bool isVertical){};
    virtual void callArkTSReboundStart(float f, float t, long d,bool isVertical){};
    virtual void sendEventAnimationsOnScroll(facebook::react::RNCSpringScrollViewEventEmitter::OnScroll onScroll){};
};

class SpringScrollViewNode : public ArkUINode, public EventBus::EventHandler<SpringScrollViewEvent> {
protected:
    SpringScrollViewNodeDelegate *m_scrollNodeDelegate;

public:
    SpringScrollViewNode();
    ~SpringScrollViewNode() override;

   void insertChild(ArkUINode &child, std::size_t index);
    void removeChild(ArkUINode &child);
    void setSpringScrollViewNodeDelegate(SpringScrollViewNodeDelegate *springScrollViewNodeDelegate);
    void init();
    void registerPanGesture();
    void setInitialContentOffset(float x, float y);
    void setRefreshHeaderHeight(float height);
    void setLoadingFooterHeight(float height);
    void setDecelerationRate(float rate);
    void setBounces(bool bounces);
    void setScrollEnabled(bool scrollEnabled);
    void setInverted(bool inverted);
    void setAllLoaded(bool allLoaded);
    void setDirectionalLockEnabled(bool directionalLockEnabled);
    void setContentOffset(float x, float y);
    void endLoading(bool rebound);
    void endRefresh();
    void scrollTo(float x, float y, bool animated);
    void onEvent(std::shared_ptr<SpringScrollViewEvent> &event) override;
    void regsiteEventBus();
    void setContentSize(Types::Size contentSize);
    void setScreenSize(Types::Size size);
    void setChildHeight(float height);
    void setChildWidth(float width);
    void setContentHeight(float height);
    void setPagingEnabled(bool pagingEnabled);
    void setPageSize(float width, float height);
    
private:
    Types::Offset contentOffset{0.0f, 0.0f};
    Types::Offset initialContentOffset{0.0f, 0.0f};
    Types::Size size{0.0f, 0.0f};
    Types::Size contentSize{0.0f, 0.0f};
    Types::Size pageSize{0.0f, 0.0f};
    Types::Point lastPoint{0.0f, 0.0f};
    Types::Point beginPoint{0.0f, 0.0f};
    Types::EdgeInsets contentInsets{0.0f, 0.0f, 0.0f, 0.0f};

    EventDispatcher events;
    float refreshHeaderHeight;
    float loadingFooterHeight;
    float decelerationRate;
    bool bounces;
    bool scrollEnabled;
    bool inverted;
    bool allLoaded;
    bool directionalLockEnabled;
    bool momentumScrolling;
    bool dragging;
    std::string draggingDirection;
    std::string refreshStatus = "waiting";
    std::string loadingStatus = "waiting";
    std::chrono::high_resolution_clock::time_point m_innerAnimationStart;
    std::chrono::high_resolution_clock::time_point m_innerHorizontalAnimationStart;
    std::shared_ptr<SpringScrollViewEvent> recordEventModel;
    float m_initialVelocity;
    float m_dampingCoefficient;
    ArkUI_NativeGestureAPI_1 *panGestureApi;
    ArkUI_GestureRecognizer *scrollPanGesture;
    ArkUI_NodeHandle m_stackArkUINodeHandle;
    ArkUI_NodeHandle m_scrollArkUINodeHandle;
    float recordSwipeY = 0;
    bool isOnloading = false;
    float downY = 0;
    float moveY = 0;
    float downX = 0;
    float moveX = 0;
    float contentHeight;
    bool isInitialContentOffset = false;
    bool isMove = false;
    bool isDestory = false;
    float recordKeyBoardPositioY = 0;
    bool recordKeyBoardShow = false;
    std::chrono::high_resolution_clock::time_point keyboardHideLastTime;
    const int DISMINATE_KEYBOARD_SHOW_HIDE = 99999;
    const int BEGIN_REFRESH = 10000;
    bool scrollBeginDrag = false;
    bool pagingEnabled = false;
    bool m_Directions;
    double m_AnimationValue;
    bool cancelAllAnimations();
    void onMove(ArkUI_GestureEvent *evt);
    void onDown(ArkUI_GestureEvent *evt);
    void onUp(ArkUI_GestureEvent *evt);
    void drag(float x, float y);
    float getYDampingCoefficient();
    float getXDampingCoefficient();
    void moveToOffset(float x, float y);
    float getPageWidth();
    float getPageHeight();
    bool overshootVertical();
    bool overshootHead();
    bool overshootFooter();
    bool overshootLoading();
    bool overshootRefresh();
    bool overshootLeft();
    bool overshootRight();
    bool overshootHorizontal();
    bool shouldPulling();
    bool shouldPullingEnough();
    bool shouldRefresh();
    bool shouldPullingCancel();
    bool shouldWaiting();
    bool shouldDrag(ArkUI_UIInputEvent *evt, bool child);
    bool shouldDragging();
    bool shouldDraggingEnough();
    bool shouldLoad();
    bool shouldDraggingCancel();
    bool shouldFooterWaiting();
    bool canHorizontalScroll();
    void onHorizontalAnimationEnd();
    void onVerticalAnimationEnd();
    void beginRefresh();
    void setRecordEventModel();
};
} // namespace rnoh