#include "Module/DetectionToClassification/Result/DetectionToClassificationResult.h"
#include "Common/Image/Image8.h"

#include "Module/DetectionToClassification/DetectionToClassification.h"
#include "Common/Debug/DebugUtil.h"

namespace ContAI {

    DetectionToClassification::DetectionToClassification(
                                const ClassificationInfo& _classificationInfo,
                                const DetectionInfo& _detectionInfo
                                )
    :m_ClassificationInfo(_classificationInfo),
    m_DetectionInfo(_detectionInfo),
    m_Classification(nullptr),
    m_Detection(nullptr)
    {
    }

    bool DetectionToClassification::Init()
    {
        m_Classification = std::unique_ptr<Classification>(new Classification(m_ClassificationInfo));
        m_Detection = std::unique_ptr<Detection>(new Detection(m_DetectionInfo));

        if (m_Classification == nullptr || m_Detection == nullptr) {
            return false;
        }

        bool classResult = m_Classification->Init();
        bool detectResult = m_Detection->Init();

        return (classResult == true && detectResult == true);
    }

    std::unique_ptr<DetectionToClassificationResult> DetectionToClassification::Run(Image8& imageData)
    {
        std::unique_ptr<DetectionToClassificationResult> result = std::unique_ptr<DetectionToClassificationResult>(new DetectionToClassificationResult(m_DetectionInfo));
        std::unique_ptr<DetectionResult> detectionResult = m_Detection->Run(imageData);
        
        if (detectionResult == nullptr) {
            return nullptr;
        }
        
        detectionResult->RemoveSmallArea();
        
        if (detectionResult->IsEmpty() == true) {
            return nullptr;
        }
        
        detectionResult->Sort();
        detectionResult->ChangeRectCoordinate(DetectionResult::RectCoord::ImageInputSize);

#if defined(DEBUG_PRINT)
        std::stringstream stream;
        stream << *detectionResult;
        PrintLog(stream);
#endif // DEBUG_PRINT
        
        for (DetectionResult::ConstIterator itr = detectionResult->Begin();
        itr != detectionResult->End();
        ++itr)
        {
            if (detectionResult->IsEmpty() == true) {
                break;
            }
            std::unique_ptr<ClassificationResult> classificationResult = nullptr;
            if (m_ClassificationInfo.IsIncludeCategory(itr->LabelName) == true) {
                std::unique_ptr<Image8> inputImage = std::unique_ptr<Image8>(
                                               imageData.Crop(
                                                  itr->DetectBox.left_,
                                                  itr->DetectBox.top_,
                                                  itr->DetectBox.right_,
                                                  itr->DetectBox.bottom_));
                classificationResult = m_Classification->Run(*inputImage);
            }
            
            if (classificationResult != nullptr) {
                result->Add(DetectionToClassificationResult::Element(*itr, classificationResult->ContainerClone()));
            }
            else {
                result->Add(DetectionToClassificationResult::Element(*itr));
            }
        }
        
        result->SetRectCoordinate(DetectionResult::RectCoord::ImageInputSize);
        
        return result;
    }
    
} // namespace ContAI
