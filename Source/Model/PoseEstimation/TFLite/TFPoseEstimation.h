#ifndef _MODEL_POSEESTIMATION_TFLITE_TFPOSEESTIMATION_H_
#define _MODEL_POSEESTIMATION_TFLITE_TFPOSEESTIMATION_H_

#include <vector>
#include <memory.h>

#include "Common/Image/Image8.h"
#include "Common/Image/Image32.h"

#include "Model/PoseEstimation/Info/PoseEstimationInfo.h"
#include "Model/PoseEstimation/Impl/PoseEstimationImpl.h"

namespace tflite{
    class FlatBufferModel;
    class Interpreter;
}
namespace ContAI{

    class PoseEstimationResult;

    class TFPoseEstimation : public PoseEstimationImpl
    {
    public:
        explicit TFPoseEstimation(
                                    const ImageShape&  _inputImageShape,
                                    const ImageShape&  _modelInputImageShape,
                                    const std::string& _modelPath,
                                    const ImageShape&  _modelOutputShape,
                                    const PoseEstimationInfo::PoseThresholdContainer& _thresholds,
                                    const float _inputMean,
                                    const float _inputStd,
                                    const int _windowSize,
                                    const PoseEstimationInfo::PostProcessingType& _PostProcessingType,
                                    const std::string & _InputLayerType = "float",
                                    unsigned int _NumThreads = 1
                                    );
        
        explicit TFPoseEstimation(
                                    const PoseEstimationInfo& _Info,
                                    const std::string & _InputLayerType = "float",
                                    unsigned int _NumThreads = 1
                                    );
    public:
        virtual bool Init() override;
        virtual std::unique_ptr<PoseEstimationResult> Run(Image8& imageData) override;

    private:
        PoseEstimationInfo m_Info;
        std::string m_InputLayerType;
        std::vector<int> m_PoseEstimationTensorSize;
        std::unique_ptr<tflite::FlatBufferModel> m_TFPoseEstimation;
        std::unique_ptr<tflite::Interpreter> m_PoseEstimationInterpreter;

        unsigned int m_InputNum;
        unsigned int m_NumThreads; 
    };

}   // namespace ContAI

#endif // _TFPOSEESTIMATION_H_
