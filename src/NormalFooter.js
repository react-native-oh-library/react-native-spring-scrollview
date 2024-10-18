/**
 * Author: Shi(bolan0000@icloud.com)
 * Date: 2019/1/18
 * Copyright (c) 2018, AoTang, Inc.
 *
 * Description:
 */

import React from "react";
import { LoadingFooter, FooterStatus } from "./LoadingFooter";
import {
  ActivityIndicator,
  Animated,
  View,
  StyleSheet,
  Text,
  Easing

} from "react-native";

export class NormalFooter extends LoadingFooter {
  static height = 80;

  static style = "stickyContent";

  render() {
    if (this.state.status === "allLoaded")
      return (
        <View style={styles.container}>
          <Text style={styles.text}>{this.getTitle()}</Text>
        </View>
      );
    return (
      <View style={styles.container}>
        {this._renderIcon()}
        <View style={styles.rContainer}>
          <Text style={styles.text}>{this.getTitle()}</Text>
          {this.renderContent()}
        </View>
      </View>
    );
  }

  startLoadAnimation = () => {
    this.loadingAnimation.setValue(0);
    Animated.loop(
      Animated.timing(this.loadingAnimation, {
        toValue: 1,
        duration: 1000,
        easing: Easing.linear,
        useNativeDriver: true
      }),
      { iterations: -1 },
    ).start();
  }

  _renderIcon() {
    const s = this.state.status;
    if (Platform.OS === "harmony") {
      this.startLoadAnimation();
      const rotateValue = this.loadingAnimation.interpolate({
        inputRange: [0, 1],
        outputRange: ["0deg", "360deg"]
      })
      if (s === "loading" || s === "cancelLoading" || s === "rebound") {
        return <Animated.Image
          source={require("./Customize/res/icon_load.png")}
          style={{
            transform: [
              {
                rotate: rotateValue
              }
            ]
          }}
        />;
      }
      const { maxHeight, offset, bottomOffset } = this.props;
      return (
        <Animated.Image
          source={require("./Customize/res/arrow.png")}
          style={{
            transform: [
              {
                rotate: offset.interpolate({
                  inputRange: [
                    bottomOffset - 1 + 45,
                    bottomOffset + 45,
                    bottomOffset + maxHeight,
                    bottomOffset + maxHeight + 1
                  ],
                  outputRange: ["180deg", "180deg", "0deg", "0deg"]
                })
              }
            ]
          }}
        />
      );
    }
    else {
      if (s === "loading" || s === "cancelLoading" || s === "rebound") {
        return <ActivityIndicator color={"gray"} />;
      }
      const { maxHeight, offset, bottomOffset } = this.props;
      return (
        <Animated.Image
          source={require("./Customize/res/arrow.png")}
          style={{
            transform: [
              {
                rotate: offset.interpolate({
                  inputRange: [
                    bottomOffset - 1 + 45,
                    bottomOffset + 45,
                    bottomOffset + maxHeight,
                    bottomOffset + maxHeight + 1
                  ],
                  outputRange: ["180deg", "180deg", "0deg", "0deg"]
                })
              }
            ]
          }}
        />
      );
    }
  }

  renderContent() {
    return null;
  }

  getTitle() {
    const s = this.state.status;
    if (s === "dragging" || s === "waiting") {
      return "Drag up to load";
    } else if (s === "draggingEnough") {
      return "Release to load";
    } else if (s === "loading") {
      return "Loading ...";
    } else if (s === "draggingCancel") {
      return "Give up loading";
    } else if (s === "rebound") {
      return "Load completed";
    } else if (s === "allLoaded") {
      return "No more data";
    }
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: "center",
    justifyContent: "center",
    flexDirection: "row"
  },
  rContainer: {
    marginLeft: 20
  },
  text: {
    marginVertical: 5,
    fontSize: 15,
    color: "#666",
    textAlign: "center",
    width: 140
  }
});