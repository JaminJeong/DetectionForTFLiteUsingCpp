
#ifndef _MODEL_CLASSIFICATION_CLASSIFICATION_H_
#define _MODEL_CLASSIFICATION_CLASSIFICATION_H_

#include "Common/Image/Image8.h"
#include "Model/Common/Model.h"
#include "Model/Classification/Info/ClassificationInfo.h"
#include "Model/Classification/Impl/ClassificationImpl.h"

namespace ContAI {

    class ClassificationResult;

    class Classification : public Model<Image8, ClassificationResult>
    {
    public:
        explicit Classification(const ClassificationInfo& classificationInfo);
        virtual ~Classification() {}
        virtual bool Init() override ;
        virtual std::unique_ptr<ClassificationResult> Run(Image8& image_data) override ;
        
    private:
        std::unique_ptr<ClassificationImpl> m_ModelType;
        ClassificationInfo m_InfoType;
    };

} // namespace ContAI
#endif // _MODEL_CLASSIFICATION_CLASSIFICATION_H_
