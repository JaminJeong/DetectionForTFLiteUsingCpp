#include "Model/Detection/Info/DetectionInfo.h"

namespace ContAI {

    DetectionInfo::DetectionInfo(
                           const ImageShape& _inputImageShape,
                           const ImageShape& _modelInputImageShape,
                           const std::string& _modelPath,
                           const std::string& _labelPath,
                           unsigned int _NumResults,
                           float _detectionThreshold,
                           unsigned int _maxDetectionBoxNum,
                           float _nonMaximumThreshold,
                           float _tooSmallArea
                           )
    :ModelInfo(
               _inputImageShape,
               _modelInputImageShape,
               _modelPath),
    m_LabelPath(_labelPath),
    m_NumResults(_NumResults),
    m_DetectionThreshold(_detectionThreshold),
    m_MaxDetectionBoxNum(_maxDetectionBoxNum),
    m_NonMaximumThreshold(_nonMaximumThreshold),
    m_TooSmallArea(_tooSmallArea)
    {
    }

    DetectionInfo::DetectionInfo(
                           const ModelInfo& _modelInfo,
                           const std::string& _labelPath,
                           const std::string & _InputLayerType,
                           unsigned int _NumResults,
                           float _detectionThreshold,
                           unsigned int _maxDetectionBoxNum,
                           float _nonMaximumThreshold,
                           float _tooSmallArea
                           )
    :ModelInfo(_modelInfo),
    m_LabelPath(_labelPath),
    m_NumResults(_NumResults),
    m_DetectionThreshold(_detectionThreshold),
    m_MaxDetectionBoxNum(_maxDetectionBoxNum),
    m_NonMaximumThreshold(_nonMaximumThreshold),
    m_TooSmallArea(_tooSmallArea)
    {
    }

    unsigned int DetectionInfo::GetNumResults() const
    {
        return m_NumResults;
    }
    float DetectionInfo::GetDetectionThreshold() const
    {
        return m_DetectionThreshold;
    }
    unsigned int DetectionInfo::GetMaxDetectionBoxNum() const
    {
        return m_MaxDetectionBoxNum;
    }
    float DetectionInfo::GetNonMaximumThreshold() const
    {
        return m_NonMaximumThreshold;
    }
    float DetectionInfo::GetTooSmallArea() const
    {
        return m_TooSmallArea;
    }
    const std::string& DetectionInfo::GetLabelPath() const
    {
        return m_LabelPath;
    }

#if defined(DEBUG_PRINT)
    std::ostream& operator<<(std::ostream& stream, const DetectionInfo& other)
    {
        stream << "DetectionInfo :  \n";
        
        stream << "m_InputImageShape : " << other.m_InputImageShape <<"\n";
        stream << "m_ModelInputImageShape : " << other.m_InputImageShape <<"\n";
        stream << "m_ModelPath : " << other.m_ModelPath <<"\n";
        stream << "m_LabelPath : " << other.m_LabelPath <<"\n";

        stream << "m_NumResults : " << other.m_NumResults <<"\n";
        stream << "m_DetectionThreshold : " << other.m_DetectionThreshold <<"\n";
        stream << "m_MaxDetectionBoxNum : " << other.m_MaxDetectionBoxNum <<"\n";
        stream << "m_NonMaximumThreshold : " << other.m_NonMaximumThreshold <<"\n";
        stream << "m_TooSmallArea : " << other.m_TooSmallArea <<"\n";

        stream << "\n";
        
        return stream;
    }
#endif // DEBUG_PRINT

} // namespace ContAI
