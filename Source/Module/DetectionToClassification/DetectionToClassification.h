#ifndef MODULE_DETECTIONTOCLASSIFICATION_DETECTIONTOCLASSIFICATION_H_
#define MODULE_DETECTIONTOCLASSIFICATION_DETECTIONTOCLASSIFICATION_H_

#include "Common/UnCopyable.h"

#include "Model/Classification/Info/ClassificationInfo.h"
#include "Model/Detection/Info/DetectionInfo.h"
#include "Model/Classification/Classification.h"
#include "Model/Detection/Detection.h"

namespace ContAI {

    class DetectionToClassificationResult;
    
    class DetectionToClassification : public UnCopyable
    {
    public:
        explicit DetectionToClassification(
                                  const ClassificationInfo& _classificationInfo,
                                  const DetectionInfo& _detectionInfo
                                  );

    public:
        bool Init();
        std::unique_ptr<DetectionToClassificationResult> Run(Image8& imageData);
        
    private:
        ClassificationInfo m_ClassificationInfo;
        DetectionInfo m_DetectionInfo;

        std::unique_ptr<Classification> m_Classification;
        std::unique_ptr<Detection> m_Detection;
    };
} // namespace ContAI

#endif // MODULE_DETECTIONTOCLASSIFICATION_DETECTIONTOCLASSIFICATION_H_
