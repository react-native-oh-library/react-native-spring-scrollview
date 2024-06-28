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

#ifndef HARMONY_SPRING_SCROLLVIEW_SRC_MAIN_CPP_SPRINGSCROLLVIEWJSIBINDER_H
#define HARMONY_SPRING_SCROLLVIEW_SRC_MAIN_CPP_SPRINGSCROLLVIEWJSIBINDER_H

#include "RNOHCorePackage/ComponentBinders/ViewComponentJSIBinder.h"

namespace rnoh {
class SpringScrollViewJSIBinder : public ViewComponentJSIBinder {
    facebook::jsi::Object createNativeProps(facebook::jsi::Runtime &rt) override {
        auto object = ViewComponentJSIBinder::createNativeProps(rt);
        object.setProperty(rt, "initialContentOffset", "string");
        object.setProperty(rt, "refreshHeaderHeight", "float");
        object.setProperty(rt, "loadingFooterHeight", "float");
        object.setProperty(rt, "bounces", "boolean");
        object.setProperty(rt, "scrollEnabled", "boolean");
        object.setProperty(rt, "inverted", "boolean");
        object.setProperty(rt, "directionalLockEnabled", "boolean");
        object.setProperty(rt, "allLoaded", "boolean");
        return object;
    }

    facebook::jsi::Object createBubblingEventTypes(facebook::jsi::Runtime &rt) override {
        return facebook::jsi::Object(rt);
    }

    facebook::jsi::Object createDirectEventTypes(facebook::jsi::Runtime &rt) override {
        facebook::jsi::Object events(rt);
        events.setProperty(rt, "topRefresh", createDirectEvent(rt, "onRefresh"));
        events.setProperty(rt, "topLoading", createDirectEvent(rt, "onLoading"));
        events.setProperty(rt, "topTouchBegin", createDirectEvent(rt, "onTouchBegin"));
        events.setProperty(rt, "topTouchFinish", createDirectEvent(rt, "onTouchFinish"));
        events.setProperty(rt, "topMomentumScrollBegin", createDirectEvent(rt, "onMomentumScrollBegin"));
        events.setProperty(rt, "topMomentumScrollEnd", createDirectEvent(rt, "onMomentumScrollEnd"));
        events.setProperty(rt, "topScroll", createDirectEvent(rt, "onScroll"));
        return events;
    }
};
} // namespace  rnoh
#endif