/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

package com.continuumai.continuumaiandroidunitybuild.tflite;

import android.app.Activity;
import java.io.IOException;

/** This classifier works with the float MobileNet model. */
public class DetectorFloatMobileNet extends Detector {
  private static final int IMAGE_MEAN = 128;
  private static final float IMAGE_STD = 128.0f;

  /**
   * Initializes an {@code DetectorFloatMobileNet}.
   */
  DetectorFloatMobileNet(Activity activity, Device device, int numThreads) throws IOException {
    super(activity, device, numThreads);
  }

  @Override
  protected String getLabelPath() {
    // you can download this file from
    // see build.gradle for where to obtain this file. It should be auto
    // downloaded into assets.
    return "labelmap.txt";
  }

  @Override
  protected String getModelPath() {
    return "detect.tflite";
  }

  @Override
  protected int getImageSizeX() {
    return 300;
  }

  @Override
  protected int getImageSizeY() {
    return 300;
  }

  @Override
  protected int getNumBytesPerChannel() {
    // a 32bit float value requires 4 bytes
    return 4;
  }

  @Override
  protected void addPixelValue(int pixelValue) {
    imgData.putFloat((((pixelValue >> 16) & 0xFF) - IMAGE_MEAN) / IMAGE_STD);
    imgData.putFloat((((pixelValue >> 8) & 0xFF) - IMAGE_MEAN) / IMAGE_STD);
    imgData.putFloat(((pixelValue & 0xFF) - IMAGE_MEAN) / IMAGE_STD);
  }
}
