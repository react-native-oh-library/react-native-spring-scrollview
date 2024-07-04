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

public:
    SpringScrollViewComponentInstance(Context context);
    void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
    void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;
    SpringScrollViewNode &getLocalRootArkUINode() override;
    void onPropsChanged(SharedConcreteProps const &props) override;
    void onRefresh() override;
    void onLoading() override;
    void onTouchBegin() override;
    void onTouchEnd() override;
    void onMomentumScrollBegin() override;
    void onMomentumScrollEnd() override;
    void onScroll(const facebook::react::RNCSpringScrollViewEventEmitter::OnScroll &onScroll) override;
    void callArkTSInnerAnimationStart(float from, float to, long duration) override;
    void callArkTSOuterAnimationStart(float from, float to, int duration) override;
    void callArkTSInnerHorizontalAnimationStart(float from, float to, long duration) override;
    void callArkTSOuterHorizontalAnimationStart(float from, float to, long duration) override;
    void callArkTSReboundAnimationStart(float from, float to, long duration) override;
    void callArkTSHorizontalReboundAnimationStart(float from, float to, long duration) override;
    void callArkTSEndRefreshStart(float from, float to, long duration) override;
    void callArkTSEndLoadingStart(float from, float to, long duration) override;
    void callArkTSScrollXStart(float from, float to, long duration) override;
    void callArkTSScrollYStart(float from, float to, long duration) override;
    void callArkTSAnimationCancel() override;
    void handleCommand(std::string const &commandName, folly::dynamic const &args) override;
    void setLayout(facebook::react::LayoutMetrics layoutMetrics) override;
    facebook::react::Size getLayoutSize() override;
    void onStateChanged(SharedConcreteState const &state) override;
    bool isHandlingTouches() const override;
    void setSwiperStatus(bool swiperStatus) override;
    void finalizeUpdates() override;
};
} // namespace rnoh
#endif // HARMONY_SpringScrollViewCOMPONENTINSTANCE_H