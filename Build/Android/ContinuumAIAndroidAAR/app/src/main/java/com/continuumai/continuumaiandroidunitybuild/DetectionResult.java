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
public class DetectionResult {

    private static final String TAG = DetectionResult.class.getSimpleName();
    // Top K classes and probabilities.
  public final ArrayList<BoundingBox> detections;
  // Latency (ms).
  public Long latency = -1L;
  // Number of valid detections (separately maintained, maybe different from detections.size()).
  public int count = 0;

  public DetectionResult() {
    detections = new ArrayList<BoundingBox>();
  }

  public void resetTo(Long latency) {
    detections.clear();
    count = 0;
    this.latency = latency;
  }

  public void addBox(float x1, float y1, float x2, float y2, int category, String labelname, float score) {
    detections.add(new BoundingBox(x1, y1, x2, y2, category, labelname, score));
    count += 1;
  }
  public void addBox(BoundingBox boundingBox) {
    detections.add(new BoundingBox(
            boundingBox.x1, boundingBox.y1, boundingBox.x2, boundingBox.y2,
            boundingBox.category, boundingBox.categoryname, boundingBox.score));
    count += 1;
  }
  public void scaleUp(double scaleFactorWidth, double scaleFactorHeight) {
    for (BoundingBox box : detections) {
      box.x1 = (float) (box.x1 * scaleFactorWidth);
      box.y1 = (float) (box.y1 * scaleFactorHeight);
      box.x2 = (float) (box.x2 * scaleFactorWidth);
      box.y2 = (float) (box.y2 * scaleFactorHeight);
    }
  }

  public void sort() {
    detections.sort(new Comparator<BoundingBox>() {
      @Override
      public int compare(BoundingBox o1, BoundingBox o2) {
        return (o1.score < o2.score) ? 1 : -1;
      }
    });
  }

  public void remainCategoryName(ArrayList<String> categorys) {
    DetectionResult detectionResult = new DetectionResult();

    for (BoundingBox box : detections) {
      for (String category : categorys) {
        if (category.equals(box.categoryname)) {
          detectionResult.addBox(box);
        }
      }
    }

    this.resetTo((long) 0);
    for (BoundingBox box : detectionResult.detections) {
      this.addBox(box);
    }
  }

  public void remainCategory(ArrayList<Integer> categorys) {
    DetectionResult detectionResult = new DetectionResult();

    for (BoundingBox box : detections) {
      for (int category : categorys) {
        if (category == box.category) {
          detectionResult.addBox(box);
        }
      }
    }

    this.resetTo((long) 0);
    for (BoundingBox box : detectionResult.detections) {
      this.addBox(box);
    }
  }

  @Override
  public String toString() {
    String textToShow = latency + "ms";
    int k = 0;
    for (BoundingBox box : detections) {
      textToShow +=
          "\nPrediction ["
              + k
              + "] = Class : "
              + box.category
              +", "
              + box.categoryname
              + " ("
              + box.x1
              + ", "
              + box.y1
              + ", "
              + box.x2
              + ", "
              + box.y2
              + ") : "
              + box.score;
      k++;
    }
    return textToShow;
  }

  public String toJson() {
    if (detections.size() == 0) {
      return "";
    }

    String jsonString = "{\"Recog\":[";
    for (BoundingBox box : detections) {

      jsonString += "{\"ModelName\":\"" + box.categoryname + "\",";
      // if you want Score of Detection
      jsonString += "\"x\":" + box.x1 + ",";
      jsonString += "\"y\":" + box.y1 + ",";
      jsonString += "\"w\":" + box.getWidth() + ",";
      jsonString += "\"h\":" + box.getHeight() + ",";
      jsonString += "\"score\":" + box.score;
      jsonString += "},";
    }
    jsonString = jsonString.substring(0, jsonString.length() - 1);
    jsonString += "]}";

    return jsonString;
  }

}
