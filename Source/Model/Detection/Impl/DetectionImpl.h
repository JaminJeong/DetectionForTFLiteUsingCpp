#ifndef _MODEL_DETECTION_IMPL_DETECTION_IMPL_H_
#define _MODEL_DETECTION_IMPL_DETECTION_IMPL_H_

#include "Common/Image/Image8.h"
#include "Model/Common/Impl/ModelImpl.h"
#include "Model/Detection/Result/DetectionResult.h"

namespace ContAI {

    typedef ModelImpl<Image8, DetectionResult> DetectionImpl;

} // namespace ContAI

#endif // _MODEL_DETECTION_IMPL_DETECTION_IMPL_H_

