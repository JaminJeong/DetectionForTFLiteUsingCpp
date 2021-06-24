#ifndef _MODEL_POSEESTIMATION_INFO_POSEESTIMATIONINFO_H_
#define _MODEL_POSEESTIMATION_INFO_POSEESTIMATIONINFO_H_

#include "Model/Common/Info/ModelInfo.h"

namespace ContAI{

    class PoseEstimationInfo : public ModelInfo
    {
    public:
        enum class PostProcessingType
        {
            CONFIDENCEMAP, COORDINATE, WEIGHTEDAVERAGE
        };    

    public:
        typedef std::vector<float> PoseThresholdContainer;

    public:
        explicit PoseEstimationInfo(const ImageShape&  _inputImageShape,
                                    const ImageShape&  _modelInputImageShape,
                                    const std::string& _modelPath,
                                    const ImageShape&  _modelOutputShape,
                                    const PoseThresholdContainer& _thresholds,
                                    const float _inputMean,
                                    const float _inputStd,
                                    const int _windowSize,
                                    const PostProcessingType& _PostProcessingType
                                    );
        
        explicit PoseEstimationInfo(const ModelInfo&  _modelInfo,
                                    const ImageShape& _modelOutputShape,
                                    const PoseThresholdContainer& _thresholds,
                                    const float _inputMean,
                                    const float _inputStd,
                                    const int _windowSize,
                                    const PostProcessingType& _PostProcessingType
                                    );
        
        ~PoseEstimationInfo()
        {}
        
    public:
        const ImageShape& GetModelOutputShape() const;
        const PoseThresholdContainer& GetPoseThresholdList() const;
        const float GetPoseInputMean() const;
        const float GetPoseInputStd() const;
        const int GetWindowSize() const;
        const PostProcessingType& GetPostProcessingType() const;
        
    private:
        ImageShape m_ModelOutputShape;
        PoseThresholdContainer m_PoseThresholdList;
        float m_PoseInputMean;
        float m_PoseInputStd;
        int m_WindowSize;
        PostProcessingType m_PostProcessingType;
    };

} // namespace ContAI

#endif // _MODEL_POSEESTIMATION_INFO_POSEESTIMATIONINFO_H_
