#ifndef _COMMON_COMMON_H_
#define _COMMON_COMMON_H_

#include "Common/Platform.h"

//#define NDEBUG

#define DEBUG_TF

#if defined(DEBUG_TF)
#   define DEBUG_PRINT
//#   if defined(_IOS_)
//#       define DEBUG_SAVE_IMAGE
//#   endif // _IOS_
#   define DEBUG_TIME_PRINT
#endif // DEBUG_TF

#define TENSORFLOW
#if defined(TENSORFLOW)
//#   define TENSORFLOW_1_10
#   define TENSORFLOW_1_13
#endif // TENSORFLOW

//#define OPENCV
#if defined(_IOS_) && defined(OPENCV)
#   include <opencv2/highgui/ios.h>
#   include <opencv2/highgui/cap_ios.h>
#endif // OPENCV

#if defined(DEBUG_TIME_PRINT)
#   include "Common/Time/CaTime.h"
#endif // DEBUG_TIME_PRINT

#define NUM_RESULTS 1917
    
#endif // _COMMON_COMMON_H_
