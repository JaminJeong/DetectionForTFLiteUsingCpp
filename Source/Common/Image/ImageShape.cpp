#include "Common/Image/ImageShape.h"

namespace ContAI {
    ImageShape::ImageShape(
               int _width,
               int _height,
               int _channel
               )
    {
        m_Shape.resize(4);
        
        m_Shape[0] = 1;
        m_Shape[2] = _width;
        m_Shape[1] = _height;
        m_Shape[3] = _channel;
    }
    
    ImageShape::ImageShape(const ImageShape& other):
    m_Shape(other.m_Shape)
    {}
    
    bool ImageShape::operator == (const ImageShape& other) const
    {
        return std::equal(m_Shape.begin(), m_Shape.end(), other.m_Shape.begin());
    }
    bool ImageShape::operator != (const ImageShape& other) const
    {
        return !this->operator==(other);
    }

    int ImageShape::GetWidth() const
    {return m_Shape[2];}
    int ImageShape::GetHeight() const
    {return m_Shape[1];}
    int ImageShape::GetChannel() const
    {return m_Shape[3];}
    std::vector<int> ImageShape::GetBWHC() const
    {
        std::vector<int> result;
        result.push_back(m_Shape[0]);
        result.push_back(m_Shape[2]);
        result.push_back(m_Shape[1]);
        result.push_back(m_Shape[3]);
        return result;
    }
    std::vector<int> ImageShape::GetBHWC() const
    {
        return m_Shape;
    }
    
#if defined(DEBUG_PRINT)
    std::ostream& operator<<(std::ostream& stream,
                             const ImageShape& other)
    {
        stream << "ImageShape : \n";
        stream << "width : " << other.GetWidth() <<"\n";
        stream << "height : " << other.GetHeight() <<"\n";
        stream << "channel : " << other.GetChannel() <<"\n";
        stream << "\n";
        return stream;
    }
#endif // DEBUG_PRINT

} // namespace ContAI
