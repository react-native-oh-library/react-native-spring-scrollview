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

#include "SpringScrollViewArkTsMessageHandler.h"
#include "SpringScrollViewNode.h"
#include <EventBus/EventHandler.h>
#include "EventBus/EventBus.h"
#include <SpringScrollViewEvent.h>

namespace rnoh {
void SpringScrollViewArkTSMessageHandler::handleArkTSMessage(const Context &ctx) {
    auto rnInstance = ctx.rnInstance.lock();
    if (!rnInstance) {
        return;
    }

    DLOG(INFO) << "SpringScrollViewArkTSMessageHandler handleArkTSMessage:" << ctx.messageName << ctx.messagePayload;

    if (ctx.messageName == "onUpdate") {
        auto event = std::make_shared<SpringScrollViewEvent>(5);
        event->setAnimationValue(ctx.messagePayload["value"].asDouble());
        event->setEventType(ctx.messagePayload["type"].asString());
        event->setEventDirections(ctx.messagePayload["directions"].asBool());
        event->setMessageType("onUpdate");
        auto baseEvent = std::static_pointer_cast<EventBus::Event>(event);
        EventBus::EventBus::getInstance()->fire(baseEvent);
    }

    if (ctx.messageName == "onEnd") {
        auto event = std::make_shared<SpringScrollViewEvent>(5);
        event->setEventType(ctx.messagePayload["type"].asString());
        event->setEventDirections(ctx.messagePayload["directions"].asBool());
        event->setAnimationValue(ctx.messagePayload["value"].asDouble());
        event->setMessageType("onEnd");
        auto baseEvent = std::static_pointer_cast<EventBus::Event>(event);
        EventBus::EventBus::getInstance()->fire(baseEvent);
    }
}
} // namespace rnoh