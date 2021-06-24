#ifndef _MODEL_CLASSIFICATION_IMPL_CLASSIFICATIONIMPL_H_
#define _MODEL_CLASSIFICATION_IMPL_CLASSIFICATIONIMPL_H_

#include "Common/Image/Image8.h"
#include "Model/Common/Impl/ModelImpl.h"

namespace ContAI {

    class ClassificationResult;

    typedef ModelImpl<Image8, ClassificationResult> ClassificationImpl;

}

#endif // _MODEL_CLASSIFICATION_IMPL_CLASSIFICATIONIMPL_H_
