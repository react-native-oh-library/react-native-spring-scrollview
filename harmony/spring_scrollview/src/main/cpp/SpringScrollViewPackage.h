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

#ifndef HARMONY_SPRING_SCROLLVIEW_SRC_MAIN_CPP_SPRINGSCROLLVIEWPACKAGE_H
#define HARMONY_SPRING_SCROLLVIEW_SRC_MAIN_CPP_SPRINGSCROLLVIEWPACKAGE_H

#include "RNOH/Package.h"
#include "ComponentDescriptors.h"
#include "SpringScrollViewJSIBinder.h"
#include "SpringScrollViewNapiBinder.h"
#include "SpringScrollViewComponentInstance.h"
#include "SpringScrollViewEmitRequestHandler.h"
#include "SpringScrollViewTurboModule.h"
#include "SpringScrollViewArkTSMessageHandler.h"

namespace rnoh {
class SpringScrollViewPackageComponentInstanceFactoryDelegate : public ComponentInstanceFactoryDelegate {
public:
    using ComponentInstanceFactoryDelegate::ComponentInstanceFactoryDelegate;

    ComponentInstance::Shared create(ComponentInstance::Context ctx) override {
        if (ctx.componentName == "RNCSpringScrollView") {
            return std::make_shared<SpringScrollViewComponentInstance>(std::move(ctx));
        }
        return nullptr;
    }
};

class SpringScrollViewContextFactoryDelegate : public TurboModuleFactoryDelegate {
public:
    SharedTurboModule createTurboModule(Context ctx, const std::string &name) const override {
        if (name == "SpringScrollViewContext") {
            return std::make_shared<SpringScrollViewTurboModule>(ctx, name);
        }
        return nullptr;
    };
};

class SpringScrollViewPackage : public Package {
public:
    SpringScrollViewPackage(Package::Context ctx) : Package(ctx) {}

    ComponentInstanceFactoryDelegate::Shared createComponentInstanceFactoryDelegate() override {
        return std::make_shared<SpringScrollViewPackageComponentInstanceFactoryDelegate>();
    }

    std::vector<facebook::react::ComponentDescriptorProvider> createComponentDescriptorProviders() override {
        return {facebook::react::concreteComponentDescriptorProvider<
            facebook::react::RNCSpringScrollViewComponentDescriptor>()};
    }

    ComponentJSIBinderByString createComponentJSIBinderByName() override {
        return {
            {"RNCSpringScrollView", std::make_shared<SpringScrollViewJSIBinder>()},
        };
    }

    ComponentNapiBinderByString createComponentNapiBinderByName() override {
        return {
            {"RNCSpringScrollView", std::make_shared<SpringScrollViewNapiBinder>()},
        };
    }

    EventEmitRequestHandlers createEventEmitRequestHandlers() override {
        return {std::make_shared<SpringScrollViewEmitRequestHandler>()};
    }

    std::unique_ptr<TurboModuleFactoryDelegate> createTurboModuleFactoryDelegate() override {
        return std::make_unique<SpringScrollViewContextFactoryDelegate>();
    }

    std::vector<ArkTSMessageHandler::Shared> createArkTSMessageHandlers() override {
        return {std::make_shared<SpringScrollViewArkTSMessageHandler>()};
    }
};
} // namespace rnoh
#endif