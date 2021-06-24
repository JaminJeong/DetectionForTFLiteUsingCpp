#ifndef _MODEL_CLASSIFICATION_TFLITE_TFCLASSIFICATION_H_
#define _MODEL_CLASSIFICATION_TFLITE_TFCLASSIFICATION_H_

#include <vector>
#include <memory.h>

#include "Common/Image/Image8.h"
#include "Model/Classification/Info/ClassificationInfo.h"
#include "Model/Classification/Impl/ClassificationImpl.h"

namespace tflite {
    class FlatBufferModel;
    class Interpreter;
}

namespace ContAI {
    
    class ClassificationResult;

    class TFClassification : public ClassificationImpl
    {
    public:
        explicit TFClassification(
                                    const ImageShape& _inputImageShape,
                                    const ImageShape& _modelInputImageShape,
                                    const std::string & _ModelPath,
                                    const std::string & _LabelPath,
                                    float _classificationThreshold,
                                    const ClassificationInfo::CategoryContainer& _category,
                                    const std::string & _InputLayerType = "uint8",
                                    unsigned int _NumThreads = 1
                                    );
        
        explicit TFClassification(
                                     const ClassificationInfo& _Info,
                                     const std::string & _InputLayerType = "uint8",
                                     unsigned int _NumThreads = 1
                                     );
        virtual ~TFClassification() {}
        
    public:
        virtual bool Init() override;
        virtual std::unique_ptr<ClassificationResult> Run(Image8& imageData) override;

    public:
        unsigned int GetLabelSize() const
        {
            return static_cast<unsigned int>(m_LabelList.size());
        }
        
    private:
        ClassificationInfo m_Info;
        unsigned int m_NumClassses;
        unsigned int m_NumThreads;
        std::string m_InputLayerType;
        
        std::vector<int> m_ClassificationTensorSize;
        LabelListType m_LabelList;
        std::unique_ptr<tflite::FlatBufferModel> m_TFClassification;
        std::unique_ptr<tflite::Interpreter> m_ClassificationInterpreter;
        int m_InputNum;
    };

} // namespace ContAI

#endif // _MODEL_CLASSIFICATION_TFLITE_TFCLASSIFICATION_H_
