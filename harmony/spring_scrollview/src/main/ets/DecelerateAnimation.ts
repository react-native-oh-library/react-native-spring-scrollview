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

import animator, { AnimatorResult } from '@ohos.animator';
import { AnimationCallBack } from './AnimationCallBack'

export class DecelerateAnimation {
  private mAnimator: AnimatorResult | undefined = undefined
  private animating: boolean = false;
  private mDuration:number = 0;
  private mFrom:number = 0;
  private mTo:number = 0;
  constructor(from: number, to: number, duration: number) {
    this.mDuration = duration;
    this.mFrom = from;
    this.mTo = to;
    this.mAnimator = animator.create({
      duration: duration,
      easing: "ease",
      delay: 0,
      fill: "forwards",
      direction: "normal",
      iterations: 1,
      begin: from,
      end: to
    })
  }

  start(callBack: AnimationCallBack) {
    if (!this.mAnimator) {
      return
    }
    this.animating = true;
    this.mAnimator.onFinish = () => {
      this.animating = false;
      if (callBack) {
        callBack.onEnd();
      }
    }
    this.mAnimator.onRepeat = () => {
    }
    this.mAnimator.onCancel = () => {
    }

    this.mAnimator.onFrame = (value: number) => {
      if (callBack) {
        callBack.onUpdate(value, value);
      }
    }
    this.mAnimator.play();
  }

  cancel(): boolean {
    if (!this.mAnimator) {
      return false;
    }
    this.mAnimator.cancel();
    return this.animating;
  }
}