#ifndef _MODEL_COMMON_INFO_MODELINFO_H_
#define _MODEL_COMMON_INFO_MODELINFO_H_

#include "Common/Image/ImageShape.h"

namespace ContAI {

    class ModelInfo
    {
    public:
        ModelInfo(const ImageShape& _inputImageShape,
                  const ImageShape& _modelInputImageShape,
                  const std::string& _modelPath
                  );
        
        ModelInfo(const ModelInfo& _modelInfo);

        virtual ~ModelInfo()
        {}
        

    // public:
    //     // Default Constructor
    //     ModelInfo(const ModelInfo& other);
    //     // Default operator=
    //     ModelInfo& operator=(const ModelInfo& other);

    public:
        const ImageShape& GetInputImageShape() const;
        
        // input shape 으로 변경 -  vector<unsigned int> size == 4
        const ImageShape& GetModelInputImageShape() const;
        
        // ModelPath 조정 가능 하도록 수정.
        const std::string& GetModelPath() const;
        
    protected:
        ImageShape m_InputImageShape;
        ImageShape m_ModelInputImageShape;
        std::string m_ModelPath;
        std::string m_LabelPath;
        
    };
    
} // namespace ContAI

#endif // _MODEL_COMMON_INFO_MODELINFO_H_
