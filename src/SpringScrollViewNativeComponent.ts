import * as React from "react";
import codegenNativeComponent from 'react-native/Libraries/Utilities/codegenNativeComponent';
import type { HostComponent, ViewProps,Animated,ViewStyle } from 'react-native';

export interface Offset {
  x: number;
  y: number;
}

export interface Size {
  width:number;
  height:number;
}

export interface NativeContentOffset {
  x?: Animated.Value;
  y?: Animated.Value;
}

export type RefreshStyle = "topping" | "stickyScrollView" | "stickyContent";

export type LoadingStyle = "bottoming" | "stickyScrollView" | "stickyContent";

export interface ScrollEvent {
  nativeEvent: {
    contentOffset: {
      x: number;
      y: number;
    };
  };
}

export type HeaderStatus =
  | "waiting"
  | "pulling"
  | "pullingEnough"
  | "pullingCancel"
  | "refreshing"
  | "rebound";

export interface RefreshHeaderPropType {
  maxHeight: number;
  offset: Animated.Value;
}
export interface RefreshHeaderStateType {
  status: HeaderStatus;
}
export class RefreshHeader extends React.Component<
  RefreshHeaderPropType,
  RefreshHeaderStateType
> {}

export class NormalHeader extends RefreshHeader {}

export type FooterStatus =
  | "waiting"
  | "dragging"
  | "draggingEnough"
  | "draggingCancel"
  | "releaseRebound"
  | "loading"
  | "rebound"
  | "allLoaded";

export interface LoadingFooterPropType {
  maxHeight: number;
  offset: Animated.Value;
  bottomOffset: number;
}

export interface LoadingFooterStateType {
  status: FooterStatus;
}

export class LoadingFooter extends React.Component<
  LoadingFooterPropType,
  LoadingFooterStateType
> {}

export class NormalFooter extends LoadingFooter {}


export interface NativeProps extends ViewProps {
  contentStyle?: ViewStyle;
  bounces?: boolean;
  scrollEnabled?: boolean;
  pagingEnabled?: boolean;
  pageSize?: Size;
  decelerationRate?: number;
  directionalLockEnabled?: boolean;
  initialContentOffset?: Offset;
  showsVerticalScrollIndicator?: boolean;
  showsHorizontalScrollIndicator?: boolean;
  refreshHeader?: React.ComponentClass<RefreshHeaderPropType, RefreshHeaderStateType>;
  loadingFooter?: React.ComponentClass<LoadingFooterPropType, LoadingFooterStateType>;
  onRefresh?: () => any;
  onLoading?: () => any;
  allLoaded?: boolean;
  textInputRefs?: any[];
  inputToolBarHeight?: number;
  tapToHideKeyboard?: boolean;
  dragToHideKeyboard?: boolean;
  onTouchBegin?: () => any;
  onTouchEnd?: () => any;
  inverted?: boolean;
  onMomentumScrollBegin?: () => any;
  onMomentumScrollEnd?: () => any;
  onScroll?: (evt: ScrollEvent) => any;
  onNativeContentOffsetExtract?: NativeContentOffset;
  onSizeChange?: (size:Size) => any;
  onContentSizeChange?: (size: Size) => any;
  onScrollBeginDrag?: () => any;
  scrollTo(offset: Offset, animated?: boolean): Promise<void>;
  scroll(offset: Offset, animated?: boolean): Promise<void>;
  scrollToBegin(animated?: boolean): Promise<void>;
  scrollToEnd(animated?: boolean): Promise<void>;
  endRefresh(): void;
  beginRefresh(): Promise<any>;
  endLoading(rebound: boolean): void;
  }
  
  
  export default codegenNativeComponent<NativeProps>(
    'SpringScrollView'
  ) as HostComponent<NativeProps>;