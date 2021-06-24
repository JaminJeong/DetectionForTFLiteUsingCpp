#ifndef _COMMON_IMAGE_SAVE_IMAGE_H_
#define _COMMON_IMAGE_SAVE_IMAGE_H_

#include "Common/Common.h"
#include "Common/Platform.h"

#if defined(DEBUG_SAVE_IMAGE)

//add opencv library
#if defined(OPENCV)
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
//#include <opencv2/highgui/ios.h>
//#include <opencv2/highgui/cap_ios.h>
#endif // OPENCV

#if defined(_IOS_)
#import <Foundation/Foundation.h>
namespace ContAI {
#if defined(OPENCV)
    UIImage* UIImageFromCVMat(cv::Mat& cvMat);
#endif // OPENCV
    class Image8;
    UIImage* UIImageFromContAIImage(Image8& inputImage);
} // namespace ContAI
#endif // _IOS_

#endif // DEBUG_SAVE_IMAGE

#endif // _COMMON_IMAGE_SAVE_IMAGE_H_
