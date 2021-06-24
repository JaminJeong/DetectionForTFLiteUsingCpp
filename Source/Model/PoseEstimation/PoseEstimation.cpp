#include "Model/PoseEstimation/PoseEstimation.h"
#include "Model/PoseEstimation/Result/PoseEstimationResult.h"

#if defined(TENSORFLOW)
#include "Model/Common/TFLite/TFHeader.h"
#include "Model/PoseEstimation/TFLite/TFPoseEstimation.h"
#endif //TENSORFLOW

namespace ContAI{

    PoseEstimation::PoseEstimation(const PoseEstimationInfo& PoseEstimationInfo)
    :m_InfoType(PoseEstimationInfo),
    m_ModelType(nullptr)
    {}

    bool PoseEstimation::Init()
    {
    #if defined(TENSORFLOW)
        m_ModelType = std::unique_ptr<TFPoseEstimation>(new TFPoseEstimation(m_InfoType));
    #endif //TENSORFLOW
        if(m_ModelType == nullptr) {
            return false;
        }
        else {
            m_ModelType->Init();
        }
        
        return true;
    }

    std::unique_ptr<PoseEstimationResult> PoseEstimation::Run(Image8& imageData)
    {
        return m_ModelType->Run(imageData);
    }

} // namespace ContAI
