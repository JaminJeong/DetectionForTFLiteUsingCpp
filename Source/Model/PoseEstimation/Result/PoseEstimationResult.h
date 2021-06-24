#ifndef _MODEL_POSEESTIMATION_RESULT_POSEESTIMATION_RESULT_H_
#define _MODEL_POSEESTIMATION_RESULT_POSEESTIMATION_RESULT_H_

#include <vector>
#include <string>
#include "Model/Common/Result/Result.h"
#include "Common/Image/ImageShape.h"

#if defined(DEBUG_PRINT)
#   include <sstream>
#endif // DEBUG_PRINT

namespace ContAI{

    class PoseEstimationResult : public Result
    {
    public:
        class Element
        {
        public:
            Element(){}
            Element(float _score, float _x, float _y);
            Element(const Element& other);
            Element& operator = (const Element& other);

        public:
            float Score;
            float X;
            float Y;
        };
        
        // if Element is too long vector or too heavy Element,
        // chage Container to priority_queue, not vector
        typedef std::vector<Element> Container;
        typedef Container::const_iterator ConstIterator;
        typedef Container::iterator Iterator;
        
    public:
        PoseEstimationResult();
        ~PoseEstimationResult();

    private:
        // Don't allow Copy Constructor
        PoseEstimationResult(const PoseEstimationResult& other) = delete;
        // Don't allow Copy Operator
        PoseEstimationResult& operator = (const PoseEstimationResult& other) = delete;

    public:
        void Add(const Element& _element);
        void Add(float _score, float _x, float _y);

        void OuputClone(const float* output, const ImageShape& _modelOutputShape);
        void GetCoordinates(const float* _confidenceMap, const ImageShape& _modelOutputShape, const std::vector<float>& _thresholds);
        void WeightedAverage(const float* _confidenceMap, const ImageShape& _modelOutputShape,
                             const std::vector<float>& _thresholds, const int _windowSize);

        const Element& At(unsigned int index) const;
        const Container& ContrainerClone() const;
        void Clean();

        const Container& GetElementList();
        const float* GetConfidenceMap();
        
    private:
        float* m_ConfidenceMap;
        Container m_ElementList;

#if defined(DEBUG_PRINT)
    public:
        friend std::ostream& operator <<(std::ostream& stream, const PoseEstimationResult& other);
#endif // DEBUG_PRINT
    };
    
} // namespace ContAI

#endif // _POSEESTIMATION_RESULT_H_
