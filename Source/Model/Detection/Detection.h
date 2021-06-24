
#ifndef _MODEL_DETECTION_DETECTION_H_
#define _MODEL_DETECTION_DETECTION_H_

#include "Common/Image/Image8.h"
#include "Model/Common/Model.h"
#include "Model/Detection/Info/DetectionInfo.h"
#include "Model/Detection/Impl/DetectionImpl.h"

namespace ContAI {

    class DetectionResult;
    
    class Detection : public Model<Image8, DetectionResult>
    {
    public:
        explicit Detection(const DetectionInfo& detectionInfo);
        virtual ~Detection() {}
        virtual bool Init() override ;
        virtual std::unique_ptr<DetectionResult> Run(Image8& imageData) override ;

    private:
        std::unique_ptr<DetectionImpl> m_ModelType;
        DetectionInfo m_InfoType;
    };

} // namespace ContAI
    
#endif // _MODEL_DETECTION_DETECTION_H_
