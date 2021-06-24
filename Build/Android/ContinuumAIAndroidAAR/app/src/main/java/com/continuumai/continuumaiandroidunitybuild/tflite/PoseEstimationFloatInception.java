/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

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

/**
 * This classifier works with the Inception-v3 slim model.
 * It applies floating point inference rather than using a quantized model.
 */
public class PoseEstimationFloatInception extends PoseEstimation {

  public static final String TAG = PoseEstimationFloatInception.class.getSimpleName();

  float[][] kernel = null;

  private int outputW = 112;
  private int outputH = 112;
  public float ratio = 0.25f;

  /**
   * An array to hold inference results, to be feed into Tensorflow Lite as outputs.
   * This isn't part of the super class, because we need a primitive array here.
   */
  private float[][][][] heatMapArray = null;
  private final int SLIDE_X = 3;
  private final int SLIDE_Y = 3;

  /**
   * Initializes an {@code PoseEstimation}.
   *
   * @param activity
   */
  public PoseEstimationFloatInception(Activity activity) throws IOException {
    super(activity);
    heatMapArray = new float[1][outputH][outputW][14];
    kernel = new float[SLIDE_X][SLIDE_Y];
    mPrintPointArray = new float[2][14];
  }

  @Override
  protected String getModelPath() {
    // you can download this file from
    // https://storage.googleapis.com/download.tensorflow.org/models/tflite/inception_v3_slim_2016_android_2017_11_10.zip
    return "mv2-cpm-224.tflite";
  }

  @Override
  public float getRatio() {
    return 0.5f;
  }

  @Override
  public int getImageSizeX() {
    return 224;
  }


  @Override
  public int getImageSizeY() {
    return 224;
  }

  @Override
  protected int getNumBytesPerChannel() {
    // a 32bit float value requires 4 bytes
    return 4;
  }

  @Override
  protected void addPixelValue(int pixelValue) {
    imgData.putFloat(((float)(pixelValue & 0xFF)));
    imgData.putFloat(((float)((pixelValue >> 8) & 0xFF)));
    imgData.putFloat(((float)((pixelValue >> 16) & 0xFF)));
  }

  @Override
  protected float getProbability(int labelIndex) {

//    return labelProbArray[0][labelIndex];
    return 0;
  }

  @Override
  protected void setProbability(int labelIndex, Number value) {
//    labelProbArray[0][labelIndex] = value.floatValue();
  }

  @Override
  protected float getNormalizedProbability(int labelIndex) {
    // TODO the following value isn't in [0,1] yet, but may be greater. Why?
    return getProbability(labelIndex);
  }

  private float get(int x, int y, float[] arr) {
    if (x < 0 || y < 0 || x >= outputW || y >= outputH)
      return -1;
    else
      return arr[x * outputW + y];
  }

  @Override
  protected void runInference() {

    tflite.run(imgData, heatMapArray);

    // post processing
    // weighted average
    for (int i = 0; i <= 13; i++) {
      int maxX = 0;
      int maxY = 0;
      float ProbValue = 0.f;

      for (int x = 0; x < outputW; x++) {
        for (int y = 0; y < outputH; y++) {
          float indexvalue = heatMapArray[0][y][x][i];
          if (ProbValue < indexvalue) {
            ProbValue = indexvalue;
            maxX = x;
            maxY = y;
          }
        }
      }

      for (int kernelIdxX = 0; kernelIdxX < SLIDE_X; ++kernelIdxX) {
        for (int kernelIdxY = 0; kernelIdxY < SLIDE_Y; ++kernelIdxY) {
          if ((maxY - 1 + kernelIdxY) < 0 || (maxX - 1 + kernelIdxX) < 0 || (maxY - 1 + kernelIdxY) >= outputH || (maxX - 1 + kernelIdxX) >= outputW)
            kernel[kernelIdxX][kernelIdxY] = 0;
          else
            kernel[kernelIdxX][kernelIdxY] = heatMapArray[0][(maxY - 1 + kernelIdxY)][(maxX - 1 + kernelIdxX)][i];
        }
      }

      float probSum = 0;
      float sumX = 0;
      float sumY = 0;
      for (int kernelIdxX = 0; kernelIdxX < SLIDE_X; ++kernelIdxX) {
        for (int kernelIdxY = 0; kernelIdxY < SLIDE_Y; ++kernelIdxY) {
          sumX += kernel[kernelIdxX][kernelIdxY] * (maxX - 1 + kernelIdxX);
          sumY += kernel[kernelIdxX][kernelIdxY] * (maxY - 1 + kernelIdxY);
          probSum += kernel[kernelIdxX][kernelIdxY];
        }
      }

      float slideMeanX = sumX / probSum;
      float slideMeanY = sumY / probSum;

      mPrintPointArray[0][i] = slideMeanX;
      mPrintPointArray[1][i] = slideMeanY;

//      Log.d(TAG, "mPrintPointArray idx : " + String.valueOf(i)
//              + ",  (" + String.valueOf(mPrintPointArray[0][i])
//              + ", " + String.valueOf(mPrintPointArray[1][i]) + " )");
    }
  }
}