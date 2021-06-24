#include "Model/Common/Info/ModelInfo.h"

namespace ContAI {

    ModelInfo::ModelInfo(const ImageShape& _inputImageShape,
                             const ImageShape& _modelInputImageShape,
                             const std::string& _modelPath)
    : m_InputImageShape(_inputImageShape),
     m_ModelInputImageShape(_modelInputImageShape),
     m_ModelPath(_modelPath)
    {}

    ModelInfo::ModelInfo(const ModelInfo& _modelInfo)
    : m_InputImageShape(_modelInfo.m_InputImageShape),
    m_ModelInputImageShape(_modelInfo.m_ModelInputImageShape),
    m_ModelPath(_modelInfo.m_ModelPath)
    {}

    const ImageShape& ModelInfo::GetInputImageShape() const
    {
        return m_InputImageShape;
    }

    const ImageShape& ModelInfo::GetModelInputImageShape() const
    {
        return m_ModelInputImageShape;
    }

    const std::string& ModelInfo::GetModelPath() const
    {
        return m_ModelPath;
    }

} // namespace ContAI
