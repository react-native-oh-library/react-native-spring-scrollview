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

#include "Props.h"
#include <react/renderer/core/PropsParserContext.h>
#include <react/renderer/core/propsConversions.h>
#include "Types.h"

namespace facebook {
namespace react {

RNCSpringScrollViewProps::RNCSpringScrollViewProps(const PropsParserContext &context,
                                                   const RNCSpringScrollViewProps &sourceProps,
                                                   const RawProps &rawProps)

    : ViewProps(context, sourceProps, rawProps),

      initialContentOffset(convertRawProp(context, rawProps, "initialContentOffset", sourceProps.initialContentOffset,
                                          Types::Offset(0.0f, 0.0f))),
      refreshHeaderHeight(
          convertRawProp(context, rawProps, "refreshHeaderHeight", sourceProps.refreshHeaderHeight, 1.0f)),
      loadingFooterHeight(
          convertRawProp(context, rawProps, "loadingFooterHeight", sourceProps.loadingFooterHeight, 1.0f)),
      bounces(convertRawProp(context, rawProps, "bounces", sourceProps.bounces, false)),
      scrollEnabled(convertRawProp(context, rawProps, "scrollEnabled", sourceProps.scrollEnabled, false)),
      inverted(convertRawProp(context, rawProps, "inverted", sourceProps.inverted, false)),
      allLoaded(convertRawProp(context, rawProps, "allLoaded", sourceProps.allLoaded, false)),
      directionalLockEnabled(
          convertRawProp(context, rawProps, "directionalLockEnabled", sourceProps.directionalLockEnabled, false)) {}

void fromRawValue(const PropsParserContext &context, const RawValue &value, Types::Offset &result) {
    auto map = (butter::map<std::string, RawValue>)value;
    auto tmp_x = map.find("x");
    if (tmp_x != map.end()) {
        fromRawValue(context, tmp_x->second, result.x);
    }
    auto tmp_y = map.find("y");
    if (tmp_y != map.end()) {
        fromRawValue(context, tmp_y->second, result.y);
    }
}

void fromRawValue(const PropsParserContext &context, const RawValue &value, Types::Size &result) {
    auto map = (butter::map<std::string, RawValue>)value;
    auto tmp_width = map.find("width");
    if (tmp_width != map.end()) {
        fromRawValue(context, tmp_width->second, result.width);
    }
    auto tmp_height = map.find("height");
    if (tmp_height != map.end()) {
        fromRawValue(context, tmp_height->second, result.height);
    }
}

void fromRawValue(const PropsParserContext &context, const RawValue &value, Types::Point &result) {
    auto map = (butter::map<std::string, RawValue>)value;
    auto tmp_x = map.find("x");
    if (tmp_x != map.end()) {
        fromRawValue(context, tmp_x->second, result.x);
    }
    auto tmp_y = map.find("y");
    if (tmp_y != map.end()) {
        fromRawValue(context, tmp_y->second, result.y);
    }
}

void fromRawValue(const PropsParserContext &context, const RawValue &value, Types::EdgeInsets &result) {
    auto map = (butter::map<std::string, RawValue>)value;
    auto tmp_top = map.find("top");
    if (tmp_top != map.end()) {
        fromRawValue(context, tmp_top->second, result.top);
    }
    auto tmp_right = map.find("right");
    if (tmp_right != map.end()) {
        fromRawValue(context, tmp_right->second, result.right);
    }
    auto tmp_bottom = map.find("bottom");
    if (tmp_bottom != map.end()) {
        fromRawValue(context, tmp_bottom->second, result.bottom);
    }
    auto tmp_left = map.find("left");
    if (tmp_left != map.end()) {
        fromRawValue(context, tmp_left->second, result.left);
    }
}
} // namespace react
} // namespace facebook