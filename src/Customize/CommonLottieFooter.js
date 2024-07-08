/*
 *
 * Created by Stone
 * https://github.com/bolan9999
 * Email: shanshang130@gmail.com
 * Date: 2019/2/18
 *
 */

import React from "react";
import { RefreshHeader } from "../RefreshHeader";
import { View, Animated, Easing } from "react-native";

import LottieView from 'lottie-react-native'

export class CommonLottieFooter extends RefreshHeader {
  static height: number = 100;

  constructor(props) {
    super(props);
  }

  render() {
    if (this.state.status === "allLoaded") return null;
    if(Platform.OS === "harmony") {
    return (
      <View style={{ flex: 1, marginBottom: 20, alignItems: "center" }}>
        <LottieView
          source={
            require("./res/loading.json")
          }
          style={{
            height: 80,
            width: 80,
            display: this.state.status == "loading" ? 'none' : 'flex'
          }}
          progress={this.state.rotateY * 0.0138}
          autoPlay={true}
          loop={false}
        />

        <LottieView
          source={
            require("./res/loading.json")
          }
          style={{
            height: 80,
            width: 80,
            display: this.state.status == "loading" ? 'flex' : 'none'
          }}
          autoPlay={true}
          loop={true}
        />
      </View>
    );
  }
  else {
    const { offset, bottomOffset } = this.props;
    let progress = offset.interpolate({
      inputRange: [
        bottomOffset + 50,
        bottomOffset + 500
      ],
      outputRange: [0, 1]
    });
    if (this.state.status === "loading") {
      progress = undefined;
    }
    return (
      <View style={{ flex: 1, marginBottom: 20 }}>
        <LottieView
          key={this.state.status === "loading"}
          source={require("./res/loading.json")}
          progress={progress}
          autoPlay={this.state.status === "loading"}
          loop={this.state.status === "loading"}
          speed={2}
        />
      </View>
    );
  }

  }
}