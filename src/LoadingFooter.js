/**
 * Author: Shi(bolan0000@icloud.com)
 * Date: 2019/1/18
 * Copyright (c) 2018, AoTang, Inc.
 *
 * Description:
 */

import React from "react";
import { Animated, Text } from "react-native";

export class LoadingFooter extends React.Component<
  FooterPropType,
  FooterStateType
  > {
  static height = 80;

  static style = "stickyContent";

  constructor(props: FooterPropType) {
    super(props);
    this.state = { status: props.allLoaded ? "allLoaded" : "waiting" ,rotateY:72};
    this.loadingAnimation=new Animated.Value(0);
  }

  componentWillReceiveProps(nextProps: FooterPropType) {
    if (nextProps.allLoaded) this.setState({ status: "allLoaded" });
  }

  changeToY(value) {
    if(this.state.status=="dragging" || this.state.status=="draggingEnough"){
      if(value >= 72){
        value = 72; 
      }
      this.setState({ rotateY: value });
    }
  }

  changeToState(newStatus: FooterStatus) {
    !this.props.allLoaded &&
    this.state.status !== newStatus &&
    this.onStateChange(this.state.status, newStatus);
  }

  onStateChange(oldStatus: FooterStatus, newStatus: FooterStatus) {
    // console.log("newStatus", newStatus);
    this.setState({ status: newStatus });
  }

  render() {
    return (
      <Text
        style={{
          flex: 1,
          alignSelf: "center",
          lineHeight: this.props.maxHeight,
          textAlign: "center"
        }}
      >
        {this.state.status}
      </Text>
    );
  }
}

export type FooterStatus =
  | "waiting"
  | "dragging"
  | "draggingEnough"
  | "draggingCancel"
  | "loading"
  | "rebound"
  | "allLoaded";

interface FooterPropType {
  offset?: Animated.Value,
  maxHeight?: number,
  allLoaded?: boolean,
  bottomOffset?: number;
}

interface FooterStateType {
  status?: FooterStatus
}