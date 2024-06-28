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
#ifndef SPRING_SRC_MAIN_CPP_EVENTEMITTERS_H
#define SPRING_SRC_MAIN_CPP_EVENTEMITTERS_H

#include <react/renderer/components/view/ViewEventEmitter.h>
#include <jsi/jsi.h>
#include "RNOH/arkui/ScrollNode.h"

namespace facebook {
namespace react {
class JSI_EXPORT RNCSpringScrollViewEventEmitter : public ViewEventEmitter {
public:
    using ViewEventEmitter::ViewEventEmitter;
    rnoh::ScrollNode m_scrollNode;
    struct OnRefresh {};
    struct OnLoading {};
    struct OnTouchBegin {};
    struct OnTouchFinish {};
    struct OnMomentumScrollBegin {};
    struct OnMomentumScrollEnd {};
    struct ContentOffset {
        Float x;
        Float y;
    };
    struct OnScroll {
        ContentOffset contentOffset;
        std::string refreshStatus;
        std::string loadingStatus;
    };
    struct OnSizeChange {
        Float width;
        Float height;
    };
    struct OnContentSizeChange {
        Float width;
        Float height;
    };
    void onRefresh(OnRefresh value) const;
    void onLoading(OnLoading value) const;
    void onTouchBegin(OnTouchBegin value) const;
    void onTouchFinish(OnTouchFinish value) const;
    void onMomentumScrollBegin(OnMomentumScrollBegin value) const;
    void onMomentumScrollEnd(OnMomentumScrollEnd value) const;
    void onScroll(OnScroll value) const;
    void onSizeChange(OnSizeChange value) const;
    void onContentSizeChange(OnContentSizeChange value) const;
};

} // namespace react
} // namespace facebook
#endif