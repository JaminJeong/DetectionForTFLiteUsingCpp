#include <algorithm>
#include "Model/Detection/Result/DetectionResult.h"
#include "Model/Detection/Info/DetectionInfo.h"

namespace ContAI {

    DetectionResult::Element::Element(float _score,
                                      const std::string& _labelName,
                                      const tf_tracking::BoundingBox& _rect)
    :Score(_score),
    LabelName(_labelName),
    DetectBox(_rect)
    {
    }

    DetectionResult::DetectionResult(const DetectionInfo& _detectionInfo)
    :m_DetectionInfoPtr(std::unique_ptr<DetectionInfo>(new DetectionInfo(_detectionInfo))),
    m_RectCoord(DetectionResult::RectCoord::ZeroToOneSize)
    {
    }

    // order score
    bool DetectionResult::Element::operator <(const DetectionResult::Element& other) const
    {
        return other.Score < Score;
    }
    // order area
    //     bool DetectionResult::Element::operator <(const DetectionResult::Element& other) const{
    //        return Point.Area() < other.Point.Area();
    //    }
    bool DetectionResult::Element::operator >(const DetectionResult::Element& other) const
    {
        return !operator<(other);
    }

    void DetectionResult::Add(const DetectionResult::Element& _element)
    {
        m_ElementList.push_back(_element);
    }

    void DetectionResult::Add(float _score,
                              const std::string& _labelName,
                              const tf_tracking::BoundingBox& box)
    {
        m_ElementList.push_back(DetectionResult::Element(_score, _labelName, box));
    }

    unsigned int DetectionResult::Size() const
    {
        return static_cast<unsigned int>(m_ElementList.size());
    }

    void DetectionResult::Clean()
    {
        m_ElementList.clear();
    }

    void DetectionResult::Sort()
    {
        std::sort(m_ElementList.begin(), m_ElementList.end());
    }

    bool DetectionResult::IsEmpty()
    {
        return m_ElementList.empty();
    }

    void DetectionResult::RemoveSmallArea()
    {
        for (Iterator itr = m_ElementList.begin();
             itr != m_ElementList.end(); )
        {
            float itrArea = itr->DetectBox.GetArea();
            if (itrArea < m_DetectionInfoPtr->GetTooSmallArea()) {
                itr = m_ElementList.erase(itr);
            }
            else {
                ++itr;
            }
        }
    }

    DetectionResult::ConstIterator DetectionResult::Begin() const
    {
        return m_ElementList.begin();
    }
    DetectionResult::ConstIterator DetectionResult::End() const
    {
        return m_ElementList.end();
    }

    void DetectionResult::SetRectCoordinate(DetectionResult::RectCoord _rectCoord)
    {
        m_RectCoord = _rectCoord;
    }

    void DetectionResult::ChangeRectCoordinate(DetectionResult::RectCoord _rectCoord)
    {
        float widthRatio(1.f);
        float heightRatio(1.f);
        
        if (_rectCoord == m_RectCoord)
        {
            return;
        }
        
        if (m_RectCoord == RectCoord::ModelInputSize) {
            widthRatio /= m_DetectionInfoPtr->GetModelInputImageShape().GetWidth();
            heightRatio /= m_DetectionInfoPtr->GetModelInputImageShape().GetHeight();
        }
        else if (m_RectCoord == RectCoord::ImageInputSize) {
            widthRatio /= m_DetectionInfoPtr->GetInputImageShape().GetWidth();
            heightRatio /= m_DetectionInfoPtr->GetInputImageShape().GetHeight();
        }
        
        if (_rectCoord == RectCoord::ModelInputSize) {
            widthRatio *= m_DetectionInfoPtr->GetModelInputImageShape().GetWidth();
            heightRatio *= m_DetectionInfoPtr->GetModelInputImageShape().GetHeight();
        }
        else if (_rectCoord == RectCoord::ImageInputSize) {
            widthRatio *= m_DetectionInfoPtr->GetInputImageShape().GetWidth();
            heightRatio *= m_DetectionInfoPtr->GetInputImageShape().GetHeight();
        }
        
        for (Container::iterator itr = m_ElementList.begin();
             itr != m_ElementList.end();
             ++itr)
        {
            itr->DetectBox.ScaleOrigin(widthRatio, heightRatio);
        }
        
        m_RectCoord = _rectCoord;
    }

    std::string DetectionResult::ToJson() const
    {
        std::stringstream strStream;
        
        strStream << "{\"Recog\":[";
        
        for (Container::const_iterator itr = m_ElementList.begin();
             itr != m_ElementList.end();
             ++itr)
        {
            strStream << "{\"ModelName\":\"" << itr->LabelName << "\"," ;
            strStream << "\"x\":" << itr->DetectBox.left_ << ",";
            strStream << "\"y\":" << itr->DetectBox.top_ << ",";
            strStream << "\"w\":" << itr->DetectBox.GetWidth() << ",";
            strStream << "\"h\":" << itr->DetectBox.GetHeight();
            Container::const_iterator isEnd = itr + 1;
            if (isEnd != m_ElementList.end()) {
                strStream << "},";
            }
            else {
                strStream << "}]}";
            }
        }
        
        return strStream.str();
    }
    
    const DetectionResult::Container& DetectionResult::GetElementList() const
    {
        return m_ElementList;
    }
    DetectionResult::Container& DetectionResult::GetElementList() 
    {
        return m_ElementList;
    }
    
#if defined(DEBUG_PRINT)
    std::ostream& operator<<(std::ostream& stream, const DetectionResult& other)
    {
        stream << "DetectionResult : \n";
        
        for (DetectionResult::ConstIterator itr = other.GetElementList().begin();
             itr != other.GetElementList().end();
             ++itr)
        {
            stream << "Score : " << itr->Score << "\n";
            stream << "LableName : " << itr->LabelName << "\n";
            stream << "DetectBox : " << itr->DetectBox << "\n";
        }
        
        stream << "\n";
        
        return stream;
    }
#endif // DEBUG_PRINT

} // namespace ContAI
