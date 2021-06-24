#ifndef _MODULE_DETECTIONTOCLASSIFICATION_RESULT_DETECTIONTOCLASSIFICATIONRESULT_H_
#define _MODULE_DETECTIONTOCLASSIFICATION_RESULT_DETECTIONTOCLASSIFICATIONRESULT_H_

#include <vector>
#include <string>
#include "Model/Common/Result/Result.h"
#include "Common/Common.h"
#include "Model/Classification/Result/ClassificationResult.h"
#include "Model/Detection/Result/DetectionResult.h"

#if defined(DEBUG_PRINT)
#   include <sstream>
#endif // DEBUG_PRINT

namespace ContAI {

    class DetectionToClassificationResult : public Result
    {
    public:
        class Element
        {
        public:
            Element( const DetectionResult::Element& _detectionElement );
            Element(
                const DetectionResult::Element& _detectionElement,
                 const ClassificationResult::Container& _classificationResult
                 );
            
        // Default Copy Constructor
        //    Element(const Element& other);
            
        // Default Copy Operator
        //    Element operator = (const Element& other);
            
            bool operator < (const Element& other) const;
            bool operator > (const Element& other) const;
        public:
            DetectionResult::Element m_DetectionElement;
            ClassificationResult::Container m_ClassificationResult;
        };
        
        // if Element is too long vector or too heavy Element,
        // chage Container to priority_queue, not vector
        typedef std::vector<Element> Container;
        typedef Container::const_iterator ConstIterator;
        typedef Container::iterator Iterator;
        
    public:
        DetectionToClassificationResult(const DetectionInfo& _detectionInfo);

    private:
        // Don't allow Copy Constructor
        DetectionToClassificationResult(const DetectionToClassificationResult& other) = delete;
        // Don't allow Copy Operator
        DetectionToClassificationResult& operator =(const DetectionToClassificationResult& other) = delete;

    public:
        void Add(const Element& _element);
        void Clean();
        bool IsEmpty();
        void Sort();
        void Top1();
        std::string ToJson();
        const Container& GetElementList() const;
        Container& GetElementList();

        void SetRectCoordinate(DetectionResult::RectCoord _rectCoord);
        void ChangeRectCoordinate(DetectionResult::RectCoord _rectCoord);
        
    private:
        Container m_ElementList;
        std::unique_ptr<DetectionInfo> m_DetectionInfoPtr;
        DetectionResult::RectCoord m_RectCoord;
        
#if defined(DEBUG_PRINT)
    public:
        friend std::ostream& operator <<(std::ostream& stream, const DetectionToClassificationResult& other);
#endif // DEBUG_PRINT
    };

} // namespace ContAI

#endif // _MODULE_DETECTIONTOCLASSIFICATION_RESULT_DETECTIONTOCLASSIFICATIONRESULT_H_
