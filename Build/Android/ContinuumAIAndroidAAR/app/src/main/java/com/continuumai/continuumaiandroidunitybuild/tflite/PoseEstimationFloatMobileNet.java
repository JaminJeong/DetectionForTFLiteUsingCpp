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
public class PoseEstimationFloatMobileNet extends PoseEstimation {

  public static final String TAG = PoseEstimationFloatMobileNet.class.getSimpleName();
  private static final int IMAGE_MEAN = 128;
  private static final float IMAGE_STD = 128.0f;

  float[][] kernel = null;

  private int outputW = 36;
  private int outputH = 64;

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
  public PoseEstimationFloatMobileNet(Activity activity) throws IOException {
    super(activity);
    heatMapArray = new float[1][outputH][outputW][14];
    kernel = new float[SLIDE_X][SLIDE_Y];
    mPrintPointArray = new float[2][14];
  }

  @Override
  protected String getModelPath() {
    // you can download this file from
    // https://storage.googleapis.com/download.tensorflow.org/models/tflite/inception_v3_slim_2016_android_2017_11_10.zip
    return "optimized_sa_mobile_yaxis_192_1stack_bilinear_inv_discount_structure.tflite";
  }

  @Override
  public float getRatio() {
    return 0.25f;
  }

  @Override
  public int getImageSizeX() {
    return 144;
  }


  @Override
  public int getImageSizeY() {
    return 256;
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


  protected void switchKeyPoint() {

    // Joint ID
    final int JT_HEAD = 13;			// 머리
    final int JT_NECK = 12;			// 목
    final int JT_LEFT_SHOULDER = 9;	// 좌 어깨
    final int JT_RIGHT_SHOULDER = 8;	// 우 어깨
    final int JT_LEFT_HIP = 3;		// 좌 골반
    final int JT_RIGHT_HIP = 2;		// 우 골반
    final int JT_LEFT_WRIST = 11;		// 좌 손목
    final int JT_RIGHT_WRIST = 6;		// 우 손목
    final int JT_LEFT_ELBOW = 10;		// 좌 팔꿈치
    final int JT_RIGHT_ELBOW = 7;		// 우 팔꿈치
    final int JT_LEFT_KNEE = 4;		// 좌 무릎
    final int JT_RIGHT_KNEE = 1;		// 우 무릎
    final int JT_LEFT_ANKLE = 5;		// 좌 발목
    final int JT_RIGHT_ANKLE = 0;		// 우 발목

    final int top = 0;
    final int neck = 1;
    final int l_shoulder = 2;
    final int l_elbow = 3;
    final int l_wrist = 4;
    final int r_shoulder = 5;
    final int r_elbow = 6;
    final int r_wrist = 7;
    final int l_hip = 8;
    final int l_knee = 9;
    final int l_ankle = 10;
    final int r_hip = 11;
    final int r_knee = 12;
    final int r_ankle = 13;

    float[][] mNewPrintPointArray = new float[2][14];
    mNewPrintPointArray[0][top] = mPrintPointArray[0][JT_HEAD];
    mNewPrintPointArray[1][top] = mPrintPointArray[1][JT_HEAD];
    mNewPrintPointArray[0][neck] = mPrintPointArray[0][JT_NECK];
    mNewPrintPointArray[1][neck] = mPrintPointArray[1][JT_NECK];
    mNewPrintPointArray[0][l_shoulder] = mPrintPointArray[0][JT_LEFT_SHOULDER];
    mNewPrintPointArray[1][l_shoulder] = mPrintPointArray[1][JT_LEFT_SHOULDER];
    mNewPrintPointArray[0][l_elbow] = mPrintPointArray[0][JT_LEFT_ELBOW];
    mNewPrintPointArray[1][l_elbow] = mPrintPointArray[1][JT_LEFT_ELBOW];
    mNewPrintPointArray[0][l_wrist] = mPrintPointArray[0][JT_LEFT_WRIST];
    mNewPrintPointArray[1][l_wrist] = mPrintPointArray[1][JT_LEFT_WRIST];
    mNewPrintPointArray[0][r_shoulder] = mPrintPointArray[0][JT_RIGHT_SHOULDER];
    mNewPrintPointArray[1][r_shoulder] = mPrintPointArray[1][JT_RIGHT_SHOULDER];
    mNewPrintPointArray[0][r_elbow] = mPrintPointArray[0][JT_RIGHT_ELBOW];
    mNewPrintPointArray[1][r_elbow] = mPrintPointArray[1][JT_RIGHT_ELBOW];
    mNewPrintPointArray[0][r_wrist] = mPrintPointArray[0][JT_RIGHT_WRIST];
    mNewPrintPointArray[1][r_wrist] = mPrintPointArray[1][JT_RIGHT_WRIST];
    mNewPrintPointArray[0][l_hip] = mPrintPointArray[0][JT_LEFT_HIP];
    mNewPrintPointArray[1][l_hip] = mPrintPointArray[1][JT_LEFT_HIP];
    mNewPrintPointArray[0][l_knee] = mPrintPointArray[0][JT_LEFT_KNEE];
    mNewPrintPointArray[1][l_knee] = mPrintPointArray[1][JT_LEFT_KNEE];
    mNewPrintPointArray[0][l_ankle] = mPrintPointArray[0][JT_LEFT_ANKLE];
    mNewPrintPointArray[1][l_ankle] = mPrintPointArray[1][JT_LEFT_ANKLE];
    mNewPrintPointArray[0][r_hip] = mPrintPointArray[0][JT_RIGHT_HIP];
    mNewPrintPointArray[1][r_hip] = mPrintPointArray[1][JT_RIGHT_HIP];
    mNewPrintPointArray[0][r_knee] = mPrintPointArray[0][JT_RIGHT_KNEE];
    mNewPrintPointArray[1][r_knee] = mPrintPointArray[1][JT_RIGHT_KNEE];
    mNewPrintPointArray[0][r_ankle] = mPrintPointArray[0][JT_RIGHT_ANKLE];
    mNewPrintPointArray[1][r_ankle] = mPrintPointArray[1][JT_RIGHT_ANKLE];

    mPrintPointArray = mNewPrintPointArray;
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

    switchKeyPoint();
  }

}