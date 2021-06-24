#include "Model/PoseEstimation/Info/PoseEstimationInfo.h"

namespace ContAI{

    PoseEstimationInfo::PoseEstimationInfo(
                                        const ImageShape& _inputImageShape,
                                        const ImageShape& _modelInputImageShape,
                                        const std::string& _modelPath,
                                        const ImageShape& _modelOutputShape,
                                        const PoseEstimationInfo::PoseThresholdContainer& _thresholds,
                                        const float _inputMean,
                                        const float _inputStd,
                                        const int _windowSize,
                                        const PoseEstimationInfo::PostProcessingType& _PostProcessingType
                                        )
    :ModelInfo(_inputImageShape,
               _modelInputImageShape,
               _modelPath),
    m_ModelOutputShape(_modelOutputShape),
    m_PoseThresholdList(_thresholds),
    m_PoseInputMean(_inputMean),
    m_PoseInputStd(_inputStd),
    m_WindowSize(_windowSize),
    m_PostProcessingType(_PostProcessingType)
    {
    }

    PoseEstimationInfo::PoseEstimationInfo(
                                        const ModelInfo& _modelInfo,
                                        const ImageShape& _modelOutputShape,
                                        const PoseEstimationInfo::PoseThresholdContainer& _thresholds,
                                        const float _inputMean,
                                        const float _inputStd,
                                        const int _windowSize,
                                        const PoseEstimationInfo::PostProcessingType& _PostProcessingType
                                        )
    :ModelInfo(_modelInfo),
    m_ModelOutputShape(_modelOutputShape),
    m_PoseThresholdList(_thresholds),
    m_PoseInputMean(_inputMean),
    m_PoseInputStd(_inputStd),
    m_WindowSize(_windowSize),
    m_PostProcessingType(_PostProcessingType)
    {
    }

    const ImageShape& PoseEstimationInfo::GetModelOutputShape() const
    {
        return m_ModelOutputShape;
    }

    const PoseEstimationInfo::PoseThresholdContainer& PoseEstimationInfo::GetPoseThresholdList() const
    {
        return m_PoseThresholdList;
    }

    const float PoseEstimationInfo::GetPoseInputMean() const
    {
        return m_PoseInputMean;
    }

    const float PoseEstimationInfo::GetPoseInputStd() const
    {
        return m_PoseInputStd;
    }

    const int PoseEstimationInfo::GetWindowSize() const
    {
        return m_WindowSize;
    }

    const PoseEstimationInfo::PostProcessingType& PoseEstimationInfo::GetPostProcessingType() const
    {
        return m_PostProcessingType;
    }

} // namespace ContAI
