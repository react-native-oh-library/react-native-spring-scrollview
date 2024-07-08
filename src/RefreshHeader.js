/**
 * Author: Shi(bolan0000@icloud.com)
 * Date: 2019/1/18
 * Copyright (c) 2018, AoTang, Inc.
 *
 * Description:
 */

import React from "react";
import { ActivityIndicator, Animated, Image, Text, View } from "react-native";

export class RefreshHeader extends React.Component<
  HeaderPropType,
  HeaderStateType
> {

  static height = 80;

  static style = "stickyContent";

  constructor(props: HeaderPropType) {
    super(props);
    this.state = { status: "waiting",rotateY:0};
    this.loadingAnimation=new Animated.Value(0);
  }

  changeToY(value) {
    if(this.state.status=="pulling" || this.state.status=="pullingEnough"){
      if(-value*2.5 >= 180){
         value = -72; 
      }
      this.setState({ rotateY: value });
    }
  }

  changeToState(newStatus: HeaderStatus) {
    this.state.status !== newStatus &&
      this.onStateChange(this.state.status, newStatus);
  }

  onStateChange(oldStatus: HeaderStatus, newStatus: HeaderStatus) {
    this.setState({ status: newStatus });
  }

  render() {
    return (
      <View style={{ flex: 1, justifyContent: "center", alignItems: "center" }}>
        <Text style={{ fontSize:18 }}>{this.state.status}</Text>
      </View>
    );
  }
}

export type HeaderStatus =
  | "waiting"
  | "pulling"
  | "pullingEnough"
  | "pullingCancel"
  | "refreshing"
  | "rebound";

interface HeaderPropType {
  offset?: Animated.Value;
  maxHeight?: number;
  bottomOffset?: number;
}

interface HeaderStateType {
  status?: HeaderStatus;
}