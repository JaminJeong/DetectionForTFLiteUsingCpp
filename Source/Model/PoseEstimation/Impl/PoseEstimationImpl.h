#ifndef _MODEL_POSEESTIMATION_IMPL_POSEESTIMATION_IMPL_H_
#define _MODEL_POSEESTIMATION_IMPL_POSEESTIMATION_IMPL_H_

#include "Common/Image/Image8.h"
#include "Model/Common/Impl/ModelImpl.h"

namespace ContAI{

    class PoseEstimationResult;

    typedef ModelImpl<Image8, PoseEstimationResult> PoseEstimationImpl;

}

#endif // _POSEESTIMATION_IMPL_H_
