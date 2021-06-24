#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#if defined(__APPLE__) || defined(TARGET_OS_IPHONE)
#   define _IOS_
#elif defined(__ANDROID__)
#   define _ANDROID_
#else
// Unsupported system
#error This operating system is not supported
#endif

#if defined(_IOS_)
#import <Foundation/Foundation.h>
#elif defined(_ANDROID_)
#include <android/log.h>
#endif // _ANDROID_

#include "Common/Type.h"

#endif // _PLATFORM_H_
