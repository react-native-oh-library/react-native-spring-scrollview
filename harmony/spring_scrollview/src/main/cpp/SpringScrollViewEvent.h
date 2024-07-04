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

#ifndef HARMONY_EXAMPLEEVENT_H
#define HARMONY_EXAMPLEEVENT_H
#include <EventBus/Event.h>
#include <arkui/native_type.h>
#include <string>
#include "Types.h"

class SpringScrollViewEvent : public EventBus::Event {
public:
    explicit SpringScrollViewEvent(double animationValue) : animationValue(animationValue) {}
    
    virtual ~SpringScrollViewEvent() = default;

    const std::type_info &getType() override { return typeid(SpringScrollViewEvent); }

    void setAnimationValue(double animationValue) { this->animationValue = animationValue; }

    double getAnimationValue() const { return this->animationValue; }

    void setEventType(std::string eventType) { this->eventType = eventType; }

    std::string getEventType() const { return this->eventType; }

    void setMessageType(std::string messageType) { this->messageType = messageType; }

    std::string getMessageType() const { return this->messageType; }
    
    void setNodeHandle(ArkUI_NodeHandle nodeHandle) { this->nodeHandle = nodeHandle; }
    
    ArkUI_NodeHandle getNodeHandle() const { return this->nodeHandle; }

    void setEventSpringScrollViewNodeDelegate(void *eventSpringScrollViewNodeDelegate) {
        this->m_eventSpringScrollViewNodeDelegate = eventSpringScrollViewNodeDelegate;
    }
    
    void *getEventSpringScrollViewNodeDelegate() const { return this->m_eventSpringScrollViewNodeDelegate; }

    void setRefreshStatus(std::string refreshStatus) { this->refreshStatus = refreshStatus; }

    std::string getRefreshStatus() const { return this->refreshStatus; }

    void setLoadingStatus(std::string loadingStatus) { this->loadingStatus = loadingStatus; }

    std::string getLoadingStatus() const { return this->loadingStatus; }

    void setEventContentOffset(Types::Offset eventContentOffset) { this->eventContentOffset = eventContentOffset; }

    Types::Offset getEventContentOffset() const { return this->eventContentOffset; }

    void setEventBounces(bool eventBounces) { this->eventBounces = eventBounces; }

    bool getEventBounces() const { return this->eventBounces; }

    void setEventContentSize(Types::Size eventContentSize) { this->eventContentSize = eventContentSize; }

    void setEventSize(Types::Size eventSize) { this->eventSize = eventSize; }

    Types::Size getEventContentSize() { return this->eventContentSize; }

    Types::Size getEventSize() { return this->eventSize; }

    void setEventLastPoint(Types::Point eventLastPoint) { this->eventLastPoint = eventLastPoint; }

    Types::Point getEventLastPoint() { return this->eventLastPoint; }

    void setEventBeginPoint(Types::Point eventBeginPoint) { this->eventBeginPoint = eventBeginPoint; }

    Types::Point getEventBeginPoint() { return this->eventBeginPoint; }

    void setEventContentInsets(Types::EdgeInsets eventContentInsets) { this->eventContentInsets = eventContentInsets; }

    Types::EdgeInsets getEventContentInsets() { return this->eventContentInsets; }

    void setEventMomentumScrolling(bool eventMomentumScrolling) {
        this->eventMomentumScrolling = eventMomentumScrolling;
    }

    bool getEventMomentumScrolling() { return this->eventMomentumScrolling; }

protected:
    double animationValue;
    std::string eventType;
    std::string messageType;
    std::string refreshStatus;
    std::string loadingStatus;
    ArkUI_NodeHandle nodeHandle;
    void *m_eventSpringScrollViewNodeDelegate;
    Types::Offset eventContentOffset;
    bool eventBounces;
    Types::Size eventContentSize;
    Types::Size eventSize;
    Types::Point eventLastPoint;
    Types::Point eventBeginPoint;
    Types::EdgeInsets eventContentInsets;
    bool eventMomentumScrolling;
};
#endif // HARMONY_EXAMPLEEVENT_H