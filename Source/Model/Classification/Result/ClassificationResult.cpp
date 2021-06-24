#include <algorithm>
#include "Model/Classification/Result/ClassificationResult.h"

namespace ContAI {

    ClassificationResult::Element::Element(float _score, const std::string& _labelName)
    :Score(_score), LabelName(_labelName)
    {
    }

    ClassificationResult::Element::Element(const ClassificationResult::Element& other)
    :Score(other.Score), LabelName(other.LabelName)
    {
    }

    ClassificationResult::Element&
    ClassificationResult::Element::operator = (const ClassificationResult::Element& other)
    {
        Score = other.Score;
        LabelName = other.LabelName;
        
        return *this;
    }
    // order score
    bool ClassificationResult::Element::operator <(const ClassificationResult::Element& other) const
    {
        return other.Score < Score;
    }
    // order area
    //     bool DetectionResult::Element::operator <(const DetectionResult::Element& other) const{
    //        return Point.Area() < other.Point.Area();
    //    }
    bool ClassificationResult::Element::operator >(const ClassificationResult::Element& other) const
    {
        return !operator<(other);
    }
    
    void ClassificationResult::Add(const ClassificationResult::Element& _element)
    {
        m_ElementList.push_back(_element);
    }

    void ClassificationResult::Add(float _score, const std::string& _labelName)
    {
        m_ElementList.push_back(ClassificationResult::Element(_score, _labelName));
    }

    const ClassificationResult::Element& ClassificationResult::At(unsigned int index) const
    {
        return m_ElementList.at(index);
    }

    const ClassificationResult::Container& ClassificationResult::ContainerClone() const
    {
        return m_ElementList;
    }

    void ClassificationResult::Clean()
    {
        m_ElementList.clear();
    }

    void ClassificationResult::Sort()
    {
        std::sort(m_ElementList.begin(), m_ElementList.end());
    }
    
    const ClassificationResult::Container& ClassificationResult::GetElementList()
    {
        return m_ElementList;
    }
    
#if defined(DEBUG_PRINT)
    std::ostream& operator <<(std::ostream& stream, const ClassificationResult& other)
    {
        stream << "ClassificationResult : \n";
        
        for (ClassificationResult::ConstIterator itr = other.m_ElementList.begin();
             itr != other.m_ElementList.end();
             ++itr)
        {
            stream << "Score : " << itr->Score << "\n";
            stream << "LableName : " << itr->LabelName << "\n";
        }
        
        stream << "\n";
        
        return stream;
    }
    
#endif // DEBUG_PRINT

} // namespace ContAI
