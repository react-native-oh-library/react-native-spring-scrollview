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

#ifndef HARMONY_SPRING_SCROLLVIEW_SRC_MAIN_CPP_SPRING_SCROLLVIEWNAPIBINDER_H
#define HARMONY_SPRING_SCROLLVIEW_SRC_MAIN_CPP_SPRING_SCROLLVIEWNAPIBINDER_H
#include "RNOHCorePackage/ComponentBinders/ViewComponentNapiBinder.h"
#include "Props.h"

namespace rnoh {
class SpringScrollViewNapiBinder : public ViewComponentNapiBinder {
public:
    napi_value createProps(napi_env env, facebook::react::ShadowView const shadowView) override {
        napi_value napiViewProps = ViewComponentNapiBinder::createProps(env, shadowView);
        auto propsObjBuilder = ArkJS(env).getObjectBuilder(napiViewProps);

        if (auto props = std::dynamic_pointer_cast<const facebook::react::RNCSpringScrollViewProps>(shadowView.props)) {
            propsObjBuilder.addProperty("initialContentOffsetX", props->initialContentOffset.x);
            propsObjBuilder.addProperty("initialContentOffsetY", props->initialContentOffset.y);
            propsObjBuilder.addProperty("refreshHeaderHeight", props->refreshHeaderHeight);
            propsObjBuilder.addProperty("loadingFooterHeight", props->loadingFooterHeight);
            propsObjBuilder.addProperty("bounces", props->bounces);
            propsObjBuilder.addProperty("scrollEnabled", props->scrollEnabled);
            propsObjBuilder.addProperty("allLoaded", props->allLoaded);
            propsObjBuilder.addProperty("inverted", props->inverted);
            propsObjBuilder.addProperty("directionalLockEnabled", props->directionalLockEnabled);
            propsObjBuilder.addProperty("pageSizeW", props->pageSize.width);
            propsObjBuilder.addProperty("pageSizeH", props->pageSize.height);
            propsObjBuilder.addProperty("pagingEnabled", props->pagingEnabled);
            return propsObjBuilder.build();
        }
        return napiViewProps;
    };
};
} // namespace rnoh
#endif