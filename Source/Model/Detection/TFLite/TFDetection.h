#ifndef _MODEL_DETECTION_TFLITE_TFDETECTION_H_
#define _MODEL_DETECTION_TFLITE_TFDETECTION_H_

#include "Common/Common.h"

#include <vector>
#include <map>
#include <memory.h>

#include "Common/Image/Image8.h"
#include "Model/Detection/Info/DetectionInfo.h"
#include "Model/Detection/Impl/DetectionImpl.h"

namespace tflite {
    class FlatBufferModel;
    class Interpreter;
} // tflite
    
namespace ContAI {

    class DetectionResult;

    class TFDetection : public DetectionImpl
    {
    public:
        explicit TFDetection(
                       const ImageShape& _inputImageShape,
                       const ImageShape& _modelInputImageShape,
                       const std::string& _modelPath,
                       const std::string& _labelPath,
                       unsigned int _NumResults,
                       float _detectionThreshold,
                       unsigned int _maxDetectionBoxNum,
                       float _nonMaximumThreshold,
                       float _tooSmallArea,
                       const std::string& _InputLayerType = "uint8",
                       unsigned int _NumThreads = 1
                       );

        explicit TFDetection(
                       const DetectionInfo& _DetectionInfo,
                       const std::string& _InputLayerType = "uint8",
                       unsigned int _NumThreads = 1
                       );
        virtual ~TFDetection() {}

    public:
        virtual bool Init() override ;
        virtual std::unique_ptr<DetectionResult> Run(Image8& imageData) override ;
        
    public:
        unsigned int GetLabelSize() const
        {
            return static_cast<unsigned int>(m_LabelIndexMap.size());
        }
        
    private:
        void decodeCenterSizeBoxes(
                                   float* predictions,
                                   float boxPriors[4][NUM_RESULTS],
                                   float detectionPaddingRate
                                   );
        
        static void Softmax(
                     float* outputClasses,
                     unsigned int numResults,
                     unsigned int outputClassesIndex[NUM_RESULTS],
                     float outputClassesScore[NUM_RESULTS]
                     );
        static void Sigmoid(
                     float* outputClasses,
                     unsigned int numResults,
                     unsigned int outputClassesIndex[NUM_RESULTS],
                     float outputClassesScore[NUM_RESULTS]
                     );
        
    private:
        DetectionInfo m_Info;
        
        unsigned int m_NumThreads;
        unsigned int m_NumClassses;
        
        std::string m_InputLayerType;

        std::vector<int> m_DetectionTensorSizes;
        
        LabelIndexMapType m_LabelIndexMap;
        LabelStringMapType m_LabelStringMap;

        std::unique_ptr<tflite::FlatBufferModel> m_TFDetection;
        std::unique_ptr<tflite::Interpreter> m_DetectionInterpreter;
        
        int m_DetectionInput;
    };
} // namespace ContAI

#endif // _MODEL_DETECTION_TFLITE_TFDETECTION_H_
