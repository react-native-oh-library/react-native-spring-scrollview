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
  private inner: DecelerateAnimation = null;
  private outer: DecelerateAnimation = null;
  private rebound: DecelerateAnimation = null;
  private initialVelocity: number;
  private dampingCoefficient: number;
  private from: number;
  private innerStopVelocity: number;


  startInner(f: number, v0: number, d: number, lower: number, upper: number, pagingEnabled: number, pageSize: number,
             isVertical: boolean) {
    this.from = f;
    this.initialVelocity = v0;
    this.dampingCoefficient = d;
    let displacement = 0;
    let v = this.initialVelocity;
    let mIsVertical = isVertical;
    let duration = 0;
    while ((typeof v ==="number") && Math.abs(v) > 0.10) {
      if(v == Infinity ) break;

      v *= this.dampingCoefficient;
      displacement = displacement + v;
      duration++;
      if (this.from + displacement >= upper) {
        displacement = upper - this.from;
        break;
      } else if (this.from + displacement <= lower) {
        displacement = lower - this.from;
        break;
      }
    }
    this.innerStopVelocity = v;
    if (pagingEnabled) {
      let end = Math.round((f + displacement) / pageSize) * pageSize;
      if (end > upper) {
        end = upper;
      }
      if (end < lower) {
        end = lower;
      }
      this.startRebound(f, end, 500,mIsVertical);
    } else {
      this.inner = new DecelerateAnimation(0, displacement, duration);
      let that = this;
      let callBack: AnimationCallBack = {
        onUpdate(value: number, currentPlayTime: number) {
          let v = that.initialVelocity;
          let displacement = 0;
          let currentTime = currentPlayTime;
          do {
            displacement = displacement + v;
            v *= that.dampingCoefficient;
            currentTime--;
          } while ((typeof currentTime ==="number") &&currentTime > 0);
          that.ctx.rnInstance.postMessageToCpp("onUpdate",
            { type: "InnerAnimation", value: that.from + displacement, directions: mIsVertical });
        },
        onEnd() {
          that.ctx.rnInstance.postMessageToCpp("onEnd",
            { type: "InnerAnimation", value: that.innerStopVelocity, directions: mIsVertical });
        }
      }
      if (duration > 0) {
        this.inner.start(callBack);
      } else {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "InnerAnimation", value: v, directions: mIsVertical });
      }
    }
  }

  startOuter(f: number, v0: number, d: number, isVertical: boolean) {
    this.from = f;
    this.initialVelocity = v0;
    this.dampingCoefficient = d;
    let displacement = 0;
    let v = this.initialVelocity;
    let duration = 0;
    let mIsVertical = isVertical;
    while ((typeof v ==="number") && Math.abs(v) > 0.10) {
      if(v == Infinity ) break;
      v *= this.dampingCoefficient;
      displacement = displacement + v;
      duration++;
    }
    this.outer = new DecelerateAnimation(this.from, this.from + displacement, duration);
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number, currentPlayTime: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate",
          { type: "OuterAnimation", value: value, directions: mIsVertical });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "OuterAnimation", value:-10000, directions: mIsVertical });
      }
    }
    this.outer.start(callBack);
  }


  startRebound(f: number, t: number, d: number, isVertical: boolean) {
    this.rebound = new DecelerateAnimation(f, t, d);
    let mIsVertical = isVertical;
    let that = this;
    let callBack: AnimationCallBack = {
      onUpdate(value: number, currentPlayTime: number) {
        that.ctx.rnInstance.postMessageToCpp("onUpdate",
          { type: "ReboundAnimation", value: value, directions: mIsVertical });
      },
      onEnd() {
        that.ctx.rnInstance.postMessageToCpp("onEnd", { type: "ReboundAnimation",value:-10000,  directions: mIsVertical });
      }
    }
    this.rebound.start(callBack)
  }

  cancelAnimation() {
    if (this.inner) {
      this.inner.cancel();
    }
    if (this.outer) {
      this.outer.cancel();
    }
    if (this.rebound) {
      this.rebound.cancel();
    }
  }
}