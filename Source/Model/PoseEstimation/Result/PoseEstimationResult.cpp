#include "Model/PoseEstimation/Result/PoseEstimationResult.h"

namespace ContAI{
    PoseEstimationResult::PoseEstimationResult()
    : m_ConfidenceMap(nullptr)
    {}

    PoseEstimationResult::~PoseEstimationResult()
    {
        if (m_ConfidenceMap != nullptr)
            delete m_ConfidenceMap;
    }

    PoseEstimationResult::Element::Element(const float _score, const float _x, const float _y)
    :Score(_score), X(_x), Y(_y)
    {
    }

    PoseEstimationResult::Element::Element(const PoseEstimationResult::Element& other)
    :Score(other.Score), X(other.X), Y(other.Y)
    {
    }

    PoseEstimationResult::Element&
    PoseEstimationResult::Element::operator = (const PoseEstimationResult::Element& other)
    {
        Score = other.Score;
        X = other.X;
        Y = other.Y;

        return *this;
    }

    void PoseEstimationResult::Add(const PoseEstimationResult::Element& _element)
    {
        m_ElementList.push_back(_element);
    }

    void PoseEstimationResult::Add(const float _score, const float _x, const float _y)
    {
        m_ElementList.push_back(Element(_score, _x, _y));
    }

    void PoseEstimationResult::GetCoordinates(const float* _confidenceMap, const ImageShape& _modelOutputShape,
                                              const std::vector<float>& _thresholds)
    {
        unsigned int outputWidth    = _modelOutputShape.GetWidth();
        unsigned int outputHeight   = _modelOutputShape.GetHeight();
        unsigned int outputChannel  = _modelOutputShape.GetChannel();

        m_ElementList.reserve(outputChannel);

        unsigned int oneDimIndex = 0;
        float sLargest = 0, xCoordi = 0, yCoordi = 0;

        for (unsigned int _poseIndex = 0; _poseIndex < outputChannel; ++_poseIndex){
            // Get Coordinates based on MaxConfidenceScore for each Joint
            for (unsigned int h = 0; h < outputHeight; ++h){
                for (unsigned int w = 0; w < outputWidth; ++w){
                    oneDimIndex = outputWidth * outputChannel * h + outputChannel * w + _poseIndex;
                    if (sLargest <= _confidenceMap[oneDimIndex]){
                        sLargest =  _confidenceMap[oneDimIndex];
                        yCoordi  = h;
                        xCoordi  = w;
                    }
                } 
            }
            // ReScale x,y Coordinates into [0,1)
            xCoordi = xCoordi / (float) outputWidth;
            yCoordi = yCoordi / (float) outputHeight;
            
            if (sLargest < _thresholds[_poseIndex])
                PoseEstimationResult::Add(sLargest, -99, -99);
            else
                PoseEstimationResult::Add(sLargest, xCoordi, yCoordi);

            sLargest = 0, xCoordi = 0, yCoordi = 0;
        }
    }

    void PoseEstimationResult::WeightedAverage(const float* _confidenceMap, const ImageShape& _modelOutputShape,
                                               const std::vector<float>& _thresholds, const int _halfWindowSize)
    {
        unsigned int outputWidth    = _modelOutputShape.GetWidth();
        unsigned int outputHeight   = _modelOutputShape.GetHeight();
        unsigned int outputChannel  = _modelOutputShape.GetChannel();

        m_ElementList.reserve(outputChannel);

        unsigned int   oneDimIndex = 0;
        int            xCoordi = 0, yCoordi = 0, hNew = 0, wNew = 0;
        float          yWeightedAverage = 0, xWeightedAverage = 0;
        float          xWeightedSum = 0, yWeightedSum = 0, valueSum = 0;
        float          sLargest = 0;

        for (unsigned int _poseIndex = 0; _poseIndex < outputChannel; ++_poseIndex){
            // GetCoordinates based on MaxConfidenceScore for each Joint
            for (unsigned int h = 0; h < outputHeight; ++h){
                for (unsigned int w = 0; w < outputWidth; ++w){
                    oneDimIndex = outputWidth * outputChannel * h + outputChannel * w + _poseIndex;
                    if (sLargest <= _confidenceMap[oneDimIndex]){
                        sLargest =  _confidenceMap[oneDimIndex];
                        yCoordi  = h;
                        xCoordi  = w;
                    }
                } 
            }
            // Refine Coordinates based on WeightedAverage for each Joint
            for (int h = yCoordi - _halfWindowSize; h <= yCoordi + _halfWindowSize; ++h){
                for (int w = xCoordi - _halfWindowSize; w <= xCoordi + _halfWindowSize; ++w){
                    if (h < 0 || h >= outputHeight)
                        hNew = yCoordi * 2 - h;
                    else
                        hNew = h;

                    if (w < 0 || w >= outputWidth)
                        wNew = xCoordi * 2 - w;
                    else
                        wNew = w;
                    
                    oneDimIndex = outputWidth * outputChannel * hNew + outputChannel * wNew + _poseIndex;
                    yWeightedSum += (float) h * _confidenceMap[oneDimIndex];
                    xWeightedSum += (float) w * _confidenceMap[oneDimIndex];
                    valueSum     += _confidenceMap[oneDimIndex];
                }
            }
            xWeightedAverage = xWeightedSum / valueSum;
            yWeightedAverage = yWeightedSum / valueSum;

            // ReScale x,y Coordinates into [0,1)
            xWeightedAverage = xWeightedAverage / (float) outputWidth;
            yWeightedAverage = yWeightedAverage / (float) outputHeight;

            if (sLargest < _thresholds[_poseIndex])
                PoseEstimationResult::Add(sLargest, -99, -99);
            else
                PoseEstimationResult::Add(sLargest, xWeightedAverage, yWeightedAverage);

            sLargest = 0, xCoordi = 0, yCoordi = 0;
            yWeightedSum = 0, xWeightedSum = 0, valueSum = 0;
        }
    }

    const PoseEstimationResult::Element& PoseEstimationResult::At(unsigned int index) const
    {
        return m_ElementList.at(index);
    }

    void PoseEstimationResult::OuputClone(const float* output, const ImageShape& _modelOutputShape)
    {
        unsigned int outputWidth    = _modelOutputShape.GetWidth();
        unsigned int outputHeight   = _modelOutputShape.GetHeight();
        unsigned int outputChannel  = _modelOutputShape.GetChannel();
        unsigned int length = outputWidth * outputHeight * outputChannel; 

        m_ConfidenceMap = new float[length];
        for (unsigned int i = 0; i < length; ++i)
            m_ConfidenceMap[i] = output[i];        
    }

    const PoseEstimationResult::Container& PoseEstimationResult::ContrainerClone() const
    {
        return m_ElementList;
    }

    void PoseEstimationResult::Clean()
    {
        m_ElementList.clear();
    }
    
    const PoseEstimationResult::Container& PoseEstimationResult::GetElementList()
    {
        return m_ElementList;
    }

    const float* PoseEstimationResult::GetConfidenceMap()
    {
        return m_ConfidenceMap;
    }

#if defined(DEBUG_PRINT)
    std::ostream& operator <<(std::ostream& stream, const PoseEstimationResult& other)
    {
        stream << "PoseEstimationResult \n";
        stream << "ElementList Size : " << " " << other.m_ElementList.size() << "\n";
        for (PoseEstimationResult::ConstIterator itr = other.m_ElementList.begin();
             itr != other.m_ElementList.end();
             ++itr)
        {
            stream << "Score : " << itr->Score << "\n";
            stream << "X Coordinate : " << itr->X << "\n";
            stream << "Y Coordinate : " << itr->Y << "\n";
        }
        
        stream << "\n";
        
        return stream;
    }
#endif // DEBUG_PRINT
    
} // namespace ContAI
