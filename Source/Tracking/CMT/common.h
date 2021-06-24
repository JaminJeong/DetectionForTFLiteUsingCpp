#ifndef COMMON_H
#define COMMON_H

#if defined(__APPLE__) || defined(TARGET_OS_IPHONE)
#   define _IOS_
#elif defined(__ANDROID__)
#   define _ANDROID_
#else
// Unsupported system
#error This operating system is not supported
#endif

#if defined(_IOS_)
#define OPENCV_2
#elif defined(_ANDROID_)
#define OPENCV_3
#endif // _ANDROID_

#include <limits>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>


using cv::Mat;
using cv::Point2f;
using cv::Size2f;
using std::numeric_limits;
using std::string;
using std::vector;

namespace cmt
{
    float median(vector<float> & A);
    Point2f rotate(const Point2f v, const float angle);
    template<class T>
    int sgn(T x)
    {
        if (x >=0) return 1;
        else return -1;
    }

} /* namespace cmt */

#endif /* end of include guard: COMMON_H */
