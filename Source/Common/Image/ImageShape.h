#ifndef _COMMON_IMAGE_IMAGESHAPE_H_
#define _COMMON_IMAGE_IMAGESHAPE_H_

#include "Common/Common.h"

#if defined(DEBUG_PRINT)
#   include <sstream>
#endif // DEBUG_PRINT

namespace ContAI {

    class ImageShape
    {
    public:
// if you want to
//        ImageShape(
//                   ImageSizeType _batch,
//                   ImageSizeType _width,
//                   ImageSizeType _height,
//                   ImageSizeType _channel
//                   );
        
        ImageShape(
                  int _width,
                  int _height,
                  int _channel
                   );
        
        ImageShape(const ImageShape& other);
    
    public:
        bool operator == (const ImageShape& other) const;
        bool operator != (const ImageShape& other) const;

    public:
        int GetWidth() const;
        int GetHeight() const;
        int GetChannel() const;
        std::vector<int> GetBWHC() const;
        std::vector<int> GetBHWC() const;
        
#if defined(DEBUG_PRINT)
        friend std::ostream& operator<<(std::ostream& stream, const ImageShape& tracked_object);
#endif // DEBUG_PRINT
        
    private:
        std::vector<int> m_Shape;
    };
    
} // namespace ContAI

#endif // _COMMON_IMAGE_IMAGESHAPE_H_
