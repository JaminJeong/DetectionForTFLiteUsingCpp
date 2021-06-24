#include "Model/Classification/Info/ClassificationInfo.h"
#include <algorithm>

namespace ContAI {

    ClassificationInfo::ClassificationInfo(
                                           const ImageShape& _inputImageShape,
                                           const ImageShape& _modelInputImageShape,
                                           const std::string& _modelPath,
                                           const std::string& _labelPath,
                                           float _classificationThreshold,
                                           const ClassificationInfo::CategoryContainer& _category)
    :ModelInfo(_inputImageShape,
               _modelInputImageShape,
               _modelPath),
    m_LabelPath(_labelPath),
    m_ClassificationThreshold(_classificationThreshold),
    m_Category(_category)
    {
    }

    ClassificationInfo::ClassificationInfo(
                                           const ModelInfo& _modelInfo,
                                           const std::string& _labelPath,
                                           float _classificationThreshold,
                                           const ClassificationInfo::CategoryContainer& _category
                                           )
    :ModelInfo(_modelInfo),
    m_LabelPath(_labelPath),
    m_ClassificationThreshold(_classificationThreshold),
    m_Category(_category)
    {
    }

    float ClassificationInfo::GetClassificationThreshold() const
    {
        return m_ClassificationThreshold;
    }

    bool ClassificationInfo::IsIncludeCategory(const std::string& _category) const
    {
        CategoryContainer::const_iterator itr = std::find(m_Category.begin(), m_Category.end(), _category);
        
        return (itr != m_Category.end());
    }
    
    const std::string& ClassificationInfo::GetLabelPath() const
    {
        return m_LabelPath;
    }


} // namespace ContAI
