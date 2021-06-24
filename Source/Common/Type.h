#ifndef _COMMON_TYPE_H_
#define _COMMON_TYPE_H_


#include <vector>
#include <map>
#include <string>

namespace ContAI {
    
    typedef std::map<int, std::string> LabelIndexMapType;
    typedef std::vector<std::string> LabelListType;
    typedef std::map<std::string, int> LabelStringMapType;
    typedef unsigned int ImageSizeType;
    typedef uint8_t ImagePixelType;
    typedef std::vector<ImagePixelType> ImageVecType;

} // namespace ContAI

#endif // _COMMON_TYPE_H_
