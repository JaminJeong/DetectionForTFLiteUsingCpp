#include "Model/Detection/Result/DetectionResult.h"
#include "Model/Detection/Info/DetectionInfo.h"
#include "Module/DetectionToClassification/Result/DetectionToClassificationResult.h"

#include <sstream>
#include <algorithm>

namespace ContAI {

    DetectionToClassificationResult::Element::Element(
                                                      const DetectionResult::Element& _detectionElement,
                                                      const ClassificationResult::Container& _classificationResult
                                                      )
    :m_DetectionElement(_detectionElement),
    m_ClassificationResult(_classificationResult)
    {
    }

    DetectionToClassificationResult::Element::Element( const DetectionResult::Element& _detectionElement )
    :m_DetectionElement(_detectionElement),
    m_ClassificationResult()
    {
    }

    // order score
    bool DetectionToClassificationResult::Element::operator <(const DetectionToClassificationResult::Element& other) const
    {
        return other.m_DetectionElement.Score < m_DetectionElement.Score;
    }
    bool DetectionToClassificationResult::Element::operator >(const DetectionToClassificationResult::Element& other) const
    {
        return !operator<(other);
    }

    DetectionToClassificationResult::DetectionToClassificationResult(const DetectionInfo& _detectionInfo)
    :m_DetectionInfoPtr(std::unique_ptr<DetectionInfo>(new DetectionInfo(_detectionInfo))),
    m_RectCoord(DetectionResult::RectCoord::ZeroToOneSize)
    {
    }


    void DetectionToClassificationResult::Add(
        const DetectionToClassificationResult::Element& _element)
    {
        m_ElementList.push_back(_element);
    }

    void DetectionToClassificationResult::Clean()
    {
        m_ElementList.clear();
    }

    bool DetectionToClassificationResult::IsEmpty()
    {
        return m_ElementList.empty();
    }

    void DetectionToClassificationResult::Sort()
    {
        std::sort(m_ElementList.begin(), m_ElementList.end());
    }

    void DetectionToClassificationResult::Top1()
    {
        if (m_ElementList.size() == 1 || IsEmpty())
        {
            return;
        }
        
        Sort();
        DetectionToClassificationResult::Element top1 = m_ElementList[0];
        m_ElementList.clear();
        m_ElementList.push_back(top1);
    }
    
    std::string DetectionToClassificationResult::ToJson()
    {
        std::stringstream strStream;
        
        strStream << "{\"Recog\":[";
        
        for (Container::const_iterator itr = m_ElementList.begin();
             itr != m_ElementList.end();
             ++itr)
        {
            std::string modelName = itr->m_DetectionElement.LabelName;
            if (itr->m_ClassificationResult.size() != 0) {
                modelName = itr->m_ClassificationResult[0].LabelName;
            }
            strStream << "{\"ModelName\":\"" << modelName << "\"," ;
            strStream << "\"x\":" << itr->m_DetectionElement.DetectBox.left_ << ",";
            strStream << "\"y\":" << itr->m_DetectionElement.DetectBox.top_ << ",";
            strStream << "\"w\":" << itr->m_DetectionElement.DetectBox.GetWidth() << ",";
            strStream << "\"h\":" << itr->m_DetectionElement.DetectBox.GetHeight();
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
    
    const DetectionToClassificationResult::Container& DetectionToClassificationResult::GetElementList() const
    {
        return m_ElementList;
    }
    DetectionToClassificationResult::Container& DetectionToClassificationResult::GetElementList()
    {
        return m_ElementList;
    }
    
    void DetectionToClassificationResult::ChangeRectCoordinate(DetectionResult::RectCoord _rectCoord)
    {
        float widthRatio(1.f);
        float heightRatio(1.f);
        
        if (_rectCoord == m_RectCoord)
        {
            return;
        }
        
        
        if (m_RectCoord == DetectionResult::RectCoord::ModelInputSize) {
            widthRatio /= m_DetectionInfoPtr->GetModelInputImageShape().GetWidth();
            heightRatio /= m_DetectionInfoPtr->GetModelInputImageShape().GetHeight();
        }
        else if (m_RectCoord == DetectionResult::RectCoord::ImageInputSize) {
            widthRatio /= m_DetectionInfoPtr->GetInputImageShape().GetWidth();
            heightRatio /= m_DetectionInfoPtr->GetInputImageShape().GetHeight();
        }
        
        if (_rectCoord == DetectionResult::RectCoord::ModelInputSize) {
            widthRatio *= m_DetectionInfoPtr->GetModelInputImageShape().GetWidth();
            heightRatio *= m_DetectionInfoPtr->GetModelInputImageShape().GetHeight();
        }
        else if (_rectCoord == DetectionResult::RectCoord::ImageInputSize) {
            widthRatio *= m_DetectionInfoPtr->GetInputImageShape().GetWidth();
            heightRatio *= m_DetectionInfoPtr->GetInputImageShape().GetHeight();
        }
        
        for (Container::iterator itr = m_ElementList.begin();
             itr != m_ElementList.end();
             ++itr)
        {
            itr->m_DetectionElement.DetectBox.ScaleOrigin(widthRatio, heightRatio);
        }
        
        m_RectCoord = _rectCoord;
    }
    void DetectionToClassificationResult::SetRectCoordinate(DetectionResult::RectCoord _rectCoord)
    {
        m_RectCoord = _rectCoord;
    }

#if defined(DEBUG_PRINT)
    std::ostream& operator <<(std::ostream& stream, const DetectionToClassificationResult& other)
    {
        stream << *(other.m_DetectionInfoPtr);
        
        stream << "DetectionToClassificationResult : \n";
        
        for (DetectionToClassificationResult::ConstIterator itr = other.GetElementList().begin();
             itr != other.GetElementList().end();
             ++itr)
        {
            stream << "Score : " << itr->m_DetectionElement.Score << "\n";
            stream << "LabelName : " << itr->m_DetectionElement.LabelName << "\n";
            stream << "DetectBox : " << itr->m_DetectionElement.DetectBox << "\n";

            for (ClassificationResult::ConstIterator cIter = itr->m_ClassificationResult.begin();
            cIter != itr->m_ClassificationResult.end();
            ++cIter)
            {
                stream << "ClassificationResult \n";
                stream << "Score : " << cIter->Score << "\n";
                stream << "LabelName : " << cIter->LabelName << "\n";
            }
        }

        stream << "\n";

        return stream;
    }
#endif // DEBUG_PRINT

} // namespace ContAI
