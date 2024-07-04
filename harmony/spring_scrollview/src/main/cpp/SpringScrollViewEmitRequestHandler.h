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
#ifndef SPRING_SRC_MAIN_CPP_RNCSpringScrollViewEventEmitter_H
#define SPRING_SRC_MAIN_CPP_RNCSpringScrollViewEventEmitter_H

#include <glog/logging.h>
#include "EventEmitters.h"
#include "RNOH/ArkJS.h"
#include "RNOH/EventEmitRequestHandler.h"

namespace rnoh {
class SpringScrollViewEmitRequestHandler : public EventEmitRequestHandler {
public:
    void handleEvent(EventEmitRequestHandler::Context const &ctx) override {
        auto eventName = ctx.eventName;
        auto eventEmitter =
            ctx.shadowViewRegistry->getEventEmitter<facebook::react::RNCSpringScrollViewEventEmitter>(ctx.tag);
        if (eventEmitter == nullptr) {
            return;
        }
        if (eventName == "onRefresh") {
            eventEmitter->onRefresh({});
        } else if (eventName == "onLoading") {
            eventEmitter->onLoading({});
        } else if (eventName == "onTouchBegin") {
            eventEmitter->onTouchBegin({});
        } else if (eventName == "onTouchEnd") {
            eventEmitter->onTouchEnd({});
        } else if (eventName == "onMomentumScrollBegin") {
            eventEmitter->onMomentumScrollBegin({});
        } else if (eventName == "onMomentumScrollEnd") {
            eventEmitter->onMomentumScrollEnd({});
        } else if (eventName == "onScroll") {
            eventEmitter->onScroll({});
        } else if (eventName == "onSizeChange") {
            eventEmitter->onSizeChange({});
        } else if (eventName == "onContentSizeChange") {
            eventEmitter->onContentSizeChange({});
        }
    }
};
} // namespace rnoh
#endif