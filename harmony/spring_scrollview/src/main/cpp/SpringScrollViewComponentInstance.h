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

#ifndef HARMONY_SPRINGSCROLLVIEWCOMPONENTINSTANCE_H
#define HARMONY_SPRINGSCROLLVIEWCOMPONENTINSTANCE_H

#include "RNOH/CppComponentInstance.h"
#include "SpringScrollViewNode.h"
#include "Props.h"
#include "ShadowNodes.h"
#include "Types.h"
#include <react/renderer/core/LayoutContext.h>
#include "RNOHCorePackage/TurboModules/Animated/NativeAnimatedTurboModule.h"

namespace rnoh {
class SpringScrollViewComponentInstance : public CppComponentInstance<facebook::react::RNCSpringScrollViewShadowNode>,
                                          public SpringScrollViewNodeDelegate {
private:
    SpringScrollViewNode m_springStackNode;
    Types::Offset initialContentOffset;
    Types::Size pageSize;
    float refreshHeaderHeight = 0.0f;
    float loadingFooterHeight;
    float decelerationRate;
    bool bounces;
    bool scrollEnabled;
    bool inverted;
    bool allLoaded;
    bool directionalLockEnabled;
    bool pagingEnabled;
    facebook::react::Float height{0.0};
    facebook::react::Float y{0.0};
    bool firstLoad = true;
    bool swiperStatus = false;
    facebook::react::Size m_containerSize;
    facebook::react::Size m_contentSize;
    std::weak_ptr<NativeAnimatedTurboModule> m_springNativeAnimatedTurboModule{};
    bool firstShowUI = true;
public:
    SpringScrollViewComponentInstance(Context context);
    void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
    void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;
    SpringScrollViewNode &getLocalRootArkUINode() override;
    void onPropsChanged(SharedConcreteProps const &props) override;
    void onRefresh() override;
    void onLoading() override;
    void onScroll(const facebook::react::RNCSpringScrollViewEventEmitter::OnScroll &onScroll) override;
    void callArkTSAnimationCancel() override;
    void handleCommand(std::string const &commandName, folly::dynamic const &args) override;
    facebook::react::Size getLayoutSize() override;
    void onStateChanged(SharedConcreteState const &state) override;
    void setSwiperStatus(bool swiperStatus) override;
    void finalizeUpdates() override;
    void onCustomScrollBeginDrag() override;
    void onCustomTouchBegin() override;
    void onCustomTouchEnd() override;
    void onCustomScrollEndDrag() override;
    void onCustomMomentumScrollBegin() override;
    void onCustomMomentumScrollEnd() override;
    void callArkTSInnerStart(float f, float v0, float d, float lower, float upper, bool pagingEnabled,
                                     float pageSize,bool isVertical) override;
    void callArkTSOuterStart(float f, float v0, float d,bool isVertical) override;
    void callArkTSReboundStart(float f, float t, long d,bool isVertical) override;
    facebook::react::Point getCurrentOffset() const override;
    bool isHandlingTouches() const override;
    void sendEventAnimationsOnScroll(facebook::react::RNCSpringScrollViewEventEmitter::OnScroll onScroll) override;
};
} // namespace rnoh
#endif // HARMONY_SpringScrollViewCOMPONENTINSTANCE_H