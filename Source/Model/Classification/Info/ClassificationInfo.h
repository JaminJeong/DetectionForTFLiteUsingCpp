#ifndef _MODEL_CLASSIFICATION_INFO_CLASSIFICATIONINFO_H_
#define _MODEL_CLASSIFICATION_INFO_CLASSIFICATIONINFO_H_

#include "Model/Common/Info/ModelInfo.h"

namespace ContAI {

    class ClassificationInfo : public ModelInfo
    {
    public:
        typedef std::vector<std::string> CategoryContainer;
        
    public:
        explicit ClassificationInfo(const ImageShape& _inputImageShape,
                                    const ImageShape& _modelInputImageShape,
                                    const std::string& _modelPath,
                                    const std::string& _labelPath,
                                    float _classificationThreshold,
                                    const CategoryContainer& _category);
        
        explicit ClassificationInfo(const ModelInfo& _modelInfo,
                                    const std::string& _labelPath,
                                    float _classificationThreshold,
                                    const CategoryContainer& _category);
        
        ~ClassificationInfo()
        {}

    // public:
    //     // Default Constructor
    //     ClassificationInfo(const ClassificationInfo& other);
    //     // Default operator=
    //     ClassificationInfo& operator=(const ClassificationInfo& other);
        
    public:
        float GetClassificationThreshold() const;
        bool IsIncludeCategory(const std::string& _category) const;
        const std::string& GetLabelPath() const;
        
    private:
        std::string m_LabelPath;
        float m_ClassificationThreshold;
        CategoryContainer m_Category;
    };

} // namespace ContAI

#endif // _MODEL_CLASSIFICATION_INFO_CLASSIFICATIONINFO_H_
