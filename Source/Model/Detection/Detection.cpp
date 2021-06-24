#include "Model/Detection/Detection.h"
#include "Model/Detection/Result/DetectionResult.h"

#if defined(TENSORFLOW)
#include "Model/Common/TFLite/TFHeader.h"
#include "Model/Detection/TFLite/TFDetection.h"
#endif //TENSORFLOW

namespace ContAI {

    Detection::Detection(const DetectionInfo& detectionInfo)
    :m_InfoType(detectionInfo),
    m_ModelType(nullptr)
    {}

    bool Detection::Init()
    {
    #if defined(TENSORFLOW)
        m_ModelType = std::unique_ptr<TFDetection>(new TFDetection(m_InfoType));
    #endif //TENSORFLOW
        if(m_ModelType == nullptr) {
            return false;
        }
        else {
             m_ModelType->Init();
        }

        return true;
    }

    std::unique_ptr<DetectionResult> Detection::Run(Image8& imageData)
    {
        return m_ModelType->Run(imageData);
    }

} // namespace ContAI
