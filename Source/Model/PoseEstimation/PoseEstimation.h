#ifndef _MODEL_POSEESTIMATION_POSEESTIMATION_H_
#define _MODEL_POSEESTIMATION_POSEESTIMATION_H_

#include "Common/Image/Image8.h"
#include "Model/Common/Model.h"
#include "Model/PoseEstimation/Info/PoseEstimationInfo.h"
#include "Model/PoseEstimation/Impl/PoseEstimationImpl.h"

namespace ContAI{

    class PoseEstimationResult;

    class PoseEstimation : public Model<Image8, PoseEstimationResult>
    {
    public:
        explicit PoseEstimation(const PoseEstimationInfo& PoseEstimationInfo);
        virtual ~PoseEstimation() {}
        virtual bool Init() override ;
        virtual std::unique_ptr<PoseEstimationResult> Run(Image8& imageData) override ;

    private:
        std::unique_ptr<PoseEstimationImpl> m_ModelType;
        PoseEstimationInfo m_InfoType;
    };

} // namespace ContAI
#endif // _POSEESTIMATION_H_
