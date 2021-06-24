#include "Model/Classification/Classification.h"
#include "Model/Classification/Result/ClassificationResult.h"

#if defined(TENSORFLOW)
#include "Model/Common/TFLite/TFHeader.h"
#include "Model/Classification/TFLite/TFClassification.h"
#endif //TENSORFLOW

namespace ContAI {

    Classification::Classification(const ClassificationInfo& classificationInfo)
    :m_InfoType(classificationInfo),
    m_ModelType(nullptr)
    {}

    bool Classification::Init()
    {
    #if defined(TENSORFLOW)
        m_ModelType = std::unique_ptr<TFClassification>(new TFClassification(m_InfoType));
    #endif //TENSORFLOW
        if(m_ModelType == nullptr) {
            return false;
        }
        else {
            m_ModelType->Init();
        }
        
        return true;
    }

    std::unique_ptr<ClassificationResult> Classification::Run(Image8& image_data)
    {
        return m_ModelType->Run(image_data);
    }

} // namespace ContAI
