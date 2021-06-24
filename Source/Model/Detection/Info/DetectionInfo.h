#ifndef _MODEL_DETECTION_INFO_DETECTIONINFO_H_
#define _MODEL_DETECTION_INFO_DETECTIONINFO_H_

#include "Model/Common/Info/ModelInfo.h"

namespace ContAI {

    class DetectionInfo : public ModelInfo
    {
    public:
        explicit DetectionInfo(
                               const ImageShape& _inputImageShape,
                               const ImageShape& _modelInputImageShape,
                               const std::string& _modelPath,
                               const std::string& _labelPath,
                               unsigned int _NumResults,
                               float _detectionThreshold,
                               unsigned int _maxDetectionBoxNum,
                               float _nonMaximumThreshold,
                               float _tooSmallArea
                             );
        
        explicit DetectionInfo(
                               const ModelInfo& _modelInfo,
                               const std::string& _labelPath,
                               const std::string & _InputLayerType,
                               unsigned int _NumResults,
                               float _detectionThreshold,
                               unsigned int _maxDetectionBoxNum,
                               float _nonMaximumThreshold,
                               float _tooSmallArea
                               );

        ~DetectionInfo()
        {}

    // public:
    //     // Default Constructor
    //     DetectionInfo(const DetectionInfo& other);
    //     // Default operator=
    //     DetectionInfo& operator=(const DetectionInfo& other);

    public:
        unsigned int GetNumResults() const;
        float GetDetectionThreshold() const;
        unsigned int GetMaxDetectionBoxNum() const;
        float GetNonMaximumThreshold() const;
        float GetTooSmallArea() const;
        const std::string& GetLabelPath() const;

#if defined(DEBUG_PRINT)
        friend std::ostream& operator<<(std::ostream& stream, const DetectionInfo& other);
#endif // DEBUG_PRINT

    private:
        std::string m_LabelPath;
        unsigned int m_NumResults;
        float m_DetectionThreshold;
        unsigned int m_MaxDetectionBoxNum;
        float m_NonMaximumThreshold;
        float m_TooSmallArea;
    };
    
#if defined(DEBUG_PRINT)
    std::ostream& operator<<(std::ostream& stream, const DetectionInfo& other);
#endif // DEBUG_PRINT

} // namespace ContAI

#endif // _MODEL_DETECTION_INFO_DETECTIONINFO_H_

