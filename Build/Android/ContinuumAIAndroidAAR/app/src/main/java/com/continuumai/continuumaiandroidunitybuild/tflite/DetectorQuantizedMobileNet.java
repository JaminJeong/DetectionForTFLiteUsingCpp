package com.continuumai.continuumaiandroidunitybuild.tflite;

import android.app.Activity;
import android.content.res.AssetManager;

import com.continuumai.continuumaiandroidunitybuild.DetectionResult;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

/**
 * This classifier works with the quantized MobileNet model.
 */
public class DetectorQuantizedMobileNet extends Detector {

    /**
     * Initializes an {@code DetectorQuantizedMobileNet}.
     */
    DetectorQuantizedMobileNet(Activity activity, Device device, int numThreads) throws IOException {
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
        // the quantized model uses a single byte only
        return 1;
    }

    @Override
    protected void addPixelValue(int pixelValue) {
        imgData.put((byte) ((pixelValue >> 16) & 0xFF));
        imgData.put((byte) ((pixelValue >> 8) & 0xFF));
        imgData.put((byte) (pixelValue & 0xFF));
    }
}