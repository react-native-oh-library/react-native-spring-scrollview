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

import { TurboModule } from '@rnoh/react-native-openharmony/ts';
import { DecelerateAnimation } from './DecelerateAnimation';
import { AnimationCallBack } from './AnimationCallBack';

export class SpringScrollViewModule extends TurboModule {
  private mHorizontalAnimation: DecelerateAnimation = null;
  private mVerticalAnimation: DecelerateAnimation = null;

  startInnerAnimation(from: number, to: number, duration: number) {
    this.mVerticalAnimation = new DecelerateAnimation(from, to, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate", { type: "InnerAnimation", value: value });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "InnerAnimation" });
      }
    }
    this.mVerticalAnimation.start(callBack)
  }

  startOuterAnimation(from: number, to: number, duration: number) {
    this.mVerticalAnimation = new DecelerateAnimation(from, to, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate", { type: "OuterAnimation", value: value });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "OuterAnimation" });
      }
    }
    this.mVerticalAnimation.start(callBack)
  }

  startOuterHorizontalAnimation(from: number, to: number, duration: number) {
    this.mVerticalAnimation = new DecelerateAnimation(from, to, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate", { type: "OuterHorizontalAnimation", value: value });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "OuterHorizontalAnimation" });
      }
    }
    this.mVerticalAnimation.start(callBack)
  }

  startInnerHorizontalAnimation(from: number, to: number, duration: number) {
    this.mVerticalAnimation = new DecelerateAnimation(from, to, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate", { type: "InnerHorizontalAnimation", value: value });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "InnerHorizontalAnimation" });
      }
    }
    this.mVerticalAnimation.start(callBack)
  }

  startReboundAnimation(from: number, to: number, duration: number) {
    this.mVerticalAnimation = new DecelerateAnimation(from, to, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate", { type: "ReboundAnimation", value: value });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "ReboundAnimation" });
      }
    }
    this.mVerticalAnimation.start(callBack)
  }

  startHorizontalReboundAnimation(from: number, to: number, duration: number) {
    this.mVerticalAnimation = new DecelerateAnimation(from, to, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate", { type: "HorizontalReboundAnimation", value: value });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "HorizontalReboundAnimation" });
      }
    }
    this.mVerticalAnimation.start(callBack)
  }

  startEndRefresh(from: number, to: number, duration: number) {
    this.mVerticalAnimation = new DecelerateAnimation(from, to, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate", { type: "EndRefresh", value: value });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "EndRefresh" });
      }
    }
    this.mVerticalAnimation.start(callBack)
  }

  startEndLoading(from: number, to: number, duration: number) {
    this.mVerticalAnimation = new DecelerateAnimation(from, to, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate", { type: "EndLoading", value: value });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "EndLoading" });
      }
    }
    this.mVerticalAnimation.start(callBack)
  }

  startScrollX(from: number, to: number, duration: number) {
    this.mVerticalAnimation = new DecelerateAnimation(from, to, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate", { type: "ScrollX", value: value });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "ScrollX" });
      }
    }
    this.mVerticalAnimation.start(callBack)
  }

  startScrollY(from: number, to: number, duration: number) {
    this.mVerticalAnimation = new DecelerateAnimation(from, to, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate", { type: "ScrollY", value: value });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "ScrollY" });
      }
    }
    this.mVerticalAnimation.start(callBack)
  }

  cancelAnimation() {
    if (this.mHorizontalAnimation) {
      this.mHorizontalAnimation.cancel();
    }
    if (this.mVerticalAnimation) {
      this.mVerticalAnimation.cancel();
    }
  }
}