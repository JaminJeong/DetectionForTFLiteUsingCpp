/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
package com.continuumai.continuumaiandroidunitybuild;

import java.util.ArrayList;
import java.util.Comparator;

/** Result class for inference run on a single image. */
public class ClassificationResult {

  public class ClsInfo {
    public ClsInfo(String topKClass, Float topKProb, Integer htopKIndex) {
      this.topKClass = topKClass;
      this.topKProb = topKProb;
      this.htopKIndex = htopKIndex;
    }

    public String topKClass;
    public Float topKProb;
    public Integer htopKIndex;
  }

  /** Top K classes and probabilities. */
  public ArrayList<ClsInfo> info = null;

  /** Latency (ms). */
  public Long latency;

  ClassificationResult() {
    info = new ArrayList<>();
    latency = -1L;
  }

  public void add(String topKClass, Float topKProb, Integer htopKIndex) {
    info.add(new ClsInfo(topKClass, topKProb, htopKIndex));
  }
  public void add(ClsInfo input) {
    if(input != null) {
      info.add(input);
    }
    else {
      add("unknown", 0.0f, 0);
    }
  }

  public void sort() {
    info.sort(new Comparator<ClsInfo>() {
      @Override
      public int compare(ClassificationResult.ClsInfo o1, ClassificationResult.ClsInfo o2) {
        return (o1.topKProb < o2.topKProb)? 1 : -1;
      }
    });
  }

  @Override
  public String toString() {
    String textToShow = latency + "ms";
    for (int k = 0; k < info.size(); ++k) {
      textToShow +=
          "\nPrediction ["
              + k
              + "] = Class "
              + Integer.toString(info.get(k).htopKIndex)
              + " ("
              + info.get(k).topKClass
              + ") : "
              + Float.toString(info.get(k).topKProb);
    }
    return textToShow;
  }

  public String toJson() {
    String jsonString = "{\"Recog\":[";
    for (int k = 0; k < info.size(); ++k) {

      jsonString += "{\"ModelName\":\"" + info.get(k).topKClass + "\",";
      // if you want Score of Detection
      jsonString += "\"score\":" + info.get(k).topKProb;
      jsonString += "},";
    }
    jsonString = jsonString.substring(0, jsonString.length() - 1);
    jsonString += "]}";

    return jsonString;
  }

}
