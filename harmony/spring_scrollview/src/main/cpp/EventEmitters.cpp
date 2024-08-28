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

#include <glog/logging.h>
#include "EventEmitters.h"

namespace facebook {
namespace react {
void RNCSpringScrollViewEventEmitter::onRefresh(OnRefresh event) const {
    dispatchEvent("refresh", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}

void RNCSpringScrollViewEventEmitter::onLoading(OnLoading event) const {
    dispatchEvent("loading", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}

void RNCSpringScrollViewEventEmitter::onTouchBegin(OnTouchBegin event) const {
    dispatchEvent("touchBegin", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}

void RNCSpringScrollViewEventEmitter::onTouchFinish(OnTouchFinish event) const {
    dispatchEvent("touchFinish", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}

void RNCSpringScrollViewEventEmitter::onMomentumScrollBegin(OnMomentumScrollBegin event) const {
    dispatchEvent("momentumScrollBegin", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}

void RNCSpringScrollViewEventEmitter::onMomentumScrollEnd(OnMomentumScrollEnd event) const {
    dispatchEvent("momentumScrollEnd", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}

void RNCSpringScrollViewEventEmitter::onScroll(OnScroll event) const {
    dispatchEvent("scroll", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        auto contentOffset = jsi::Object(runtime);
        contentOffset.setProperty(runtime, "x", event.contentOffset.x);
        contentOffset.setProperty(runtime, "y", event.contentOffset.y);
        payload.setProperty(runtime, "contentOffset", contentOffset);
        payload.setProperty(runtime, "refreshStatus", event.refreshStatus);
        payload.setProperty(runtime, "loadingStatus", event.loadingStatus);
        return payload;
    });
}

void RNCSpringScrollViewEventEmitter::onSizeChange(OnSizeChange event) const {
    dispatchEvent("sizeChange", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        payload.setProperty(runtime, "width", event.width);
        payload.setProperty(runtime, "height", event.height);
        return payload;
    });
}

void RNCSpringScrollViewEventEmitter::onContentSizeChange(OnContentSizeChange event) const {
    dispatchEvent("contentSizeChange", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        payload.setProperty(runtime, "width", event.width);
        payload.setProperty(runtime, "height", event.height);
        return payload;
    });
}

void RNCSpringScrollViewEventEmitter::onScrollBeginDrag() const {
    dispatchEvent("scrollBeginDrag", [](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}


void RNCSpringScrollViewEventEmitter::onCustomScrollBeginDrag() const {
    dispatchEvent("customScrollBeginDrag", [](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}

void RNCSpringScrollViewEventEmitter::onCustomScrollEndDrag() const {
    dispatchEvent("customScrollEndDrag", [](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}


void RNCSpringScrollViewEventEmitter::onCustomTouchBegin() const {
    dispatchEvent("customTouchBegin", [](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}


void RNCSpringScrollViewEventEmitter::onCustomTouchEnd() const {
    dispatchEvent("customTouchEnd", [](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}


void RNCSpringScrollViewEventEmitter::onCustomMomentumScrollBegin() const {
    dispatchEvent("customMomentumScrollBegin", [](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}


void RNCSpringScrollViewEventEmitter::onCustomMomentumScrollEnd() const {
    dispatchEvent("customMomentumScrollEnd", [](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        return payload;
    });
}

} // namespace react
} // namespace facebook