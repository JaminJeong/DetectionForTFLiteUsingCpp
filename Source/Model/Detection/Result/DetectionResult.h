#ifndef _MODEL_DETECTION_RESULT_DETECTIONRESULT_H_
#define _MODEL_DETECTION_RESULT_DETECTIONRESULT_H_

#include <vector>
#include <string>
#include <memory>
#include "Model/Common/Result/Result.h"
#include "Common/Common.h"
#include "Tracking/GoogleObjectTracker/Common/geom.h"

#if defined(DEBUG_PRINT)
#   include <sstream>
#endif // DEBUG_PRINT


namespace ContAI {
 
    class DetectionInfo;

    class DetectionResult : public Result
    {
    public:
        enum class RectCoord
        {
            ModelInputSize = 0,
            ImageInputSize,
            ZeroToOneSize
        };
        
        class Element
        {
        public:
            Element(float _score,
                    const std::string& _labelName,
                    const tf_tracking::BoundingBox& _point);
        public:
            // order score
            bool operator <(const Element& other) const;
            // order area
            // bool operator <(const Element& other) const
            bool operator >(const Element& other) const;
            
        public:
            float Score;
            std::string LabelName;
            tf_tracking::BoundingBox DetectBox;
        };
        
        // if Element is too long vector or too heavy Element,
        // chage Container to priority_queue, not vector
        typedef std::vector<Element> Container;
        typedef Container::const_iterator ConstIterator;
        typedef Container::iterator Iterator;
        
    public:
        explicit DetectionResult(const DetectionInfo& _detectionInfo);
        
    private:
        // Don't allow Copy Constructor
        DetectionResult(const DetectionResult& other) = delete;
        // Don't allow Copy Operator
        DetectionResult& operator =(const DetectionResult& other) = delete;
        
        
    public:
        void Add(const Element& _element);
        void Add(float _score,
                 const std::string& _labelName,
                 const tf_tracking::BoundingBox& _point);
        unsigned int Size() const ;
        void Clean();
        void Sort();
        bool IsEmpty();
        void RemoveSmallArea();
        const Container& GetElementList() const;
        Container& GetElementList();

        ConstIterator Begin() const;
        ConstIterator End() const;
        
        void SetRectCoordinate(RectCoord _rectCoord);
        void ChangeRectCoordinate(RectCoord _rectCoord);
        std::string ToJson() const ;
        
    #if defined(DEBUG_PRINT)
        friend std::ostream& operator<<(std::ostream& stream, const DetectionResult& other);
    #endif // DEBUG_PRINT
        
    private:
        Container m_ElementList;
        std::unique_ptr<DetectionInfo> m_DetectionInfoPtr;
        RectCoord m_RectCoord;
    };
} // namespace ContAI
    
#endif // _MODEL_DETECTION_RESULT_DETECTIONRESULT_H_

