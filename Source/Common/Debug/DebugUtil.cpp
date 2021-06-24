#include "Common/Debug/DebugUtil.h"
#include "Common/Parameter/ParameterBag.h"

#if defined(DEBUG_PRINT)
#   include "Model/Common/TFLite/TFHeader.h"
#endif // DEBUG_PRINT

#include <sstream>

namespace ContAI {
    
    void PrintLog(const std::string& str)
    {
    #if defined(_IOS_)
            NSLog(@"%s", str.c_str());
    #elif defined(_ANDROID_)
            __android_log_print(ANDROID_LOG_DEBUG, "TFLite Log", "%s", str.c_str());
    #endif // _ANDROID_

    }

    void PrintLog(const std::stringstream& sstream)
    {
        PrintLog(sstream.str());
    }

    #if defined(DEBUG_PRINT)
    void PrintInterpreterInfo(const tflite::Interpreter& interpreter)
    {
        std::stringstream stream_res;
        stream_res << "interpreter.tensors_size() : " << interpreter.tensors_size() << "\n";
        stream_res << "interpreter.nodes_size() : " << interpreter.nodes_size() << "\n";
        stream_res << "interpreter size() : " << interpreter.inputs().size() << "\n";
        for (int idx = 0; idx < interpreter.inputs().size(); ++idx)
        {
            stream_res << "interpreter.GetInputName(" << idx << ") : " << interpreter.GetInputName(idx) << "\n";
            const TfLiteTensor* tensor_ptr = interpreter.tensor(interpreter.inputs()[idx]);
            TfLiteIntArray* dims = tensor_ptr->dims;
            if (dims) {
                stream_res << "inputs" << idx << " : dims " << " : " ;
                for (int i = 0; i < dims->size; ++i) {
                    stream_res << "( " << dims->data[i] << ", ";
                }
                stream_res << ") \n";
            }
        }
        stream_res << "interpreter size() : " << interpreter.outputs().size() << "\n";
        for (int idx = 0; idx < interpreter.outputs().size(); ++idx)
        {
            stream_res << "interpreter.GetOutputtName(" << idx << ") : " << interpreter.GetOutputName(idx) << "\n";
            const TfLiteTensor* tensor_ptr = interpreter.tensor(interpreter.outputs()[idx]);
            TfLiteIntArray* dims = tensor_ptr->dims;
            if (dims) {
                stream_res << "output" << idx << " : dims " << " : " ;
                for (int i = 0; i < dims->size; ++i) {
                    stream_res << "( " << dims->data[i] << ", ";
                }
                stream_res << ") \n";
            }
        }

        PrintLog(stream_res);
        
    }

    void PrintParameterBagInfo()
    {
        std::stringstream stream_res;
     
        stream_res << "DETECTION_MODEL_FILE_NAME : " << Singleton<ParameterBag>::GetInstance()->GetValue(StringParam::DETECTION_MODEL_FILE_NAME) << "\n";
        stream_res << "DETECTION_LABEL_FILE_NAME : " << Singleton<ParameterBag>::GetInstance()->GetValue(StringParam::DETECTION_LABEL_FILE_NAME) << "\n";
        stream_res << "DETECTION_INPUT_LAYER_TYPE : " << Singleton<ParameterBag>::GetInstance()->GetValue(StringParam::DETECTION_INPUT_LAYER_TYPE) << "\n";
        stream_res << "CLASSIFICATION_MODEL_FILE_NAME : " << Singleton<ParameterBag>::GetInstance()->GetValue(StringParam::CLASSIFICATION_MODEL_FILE_NAME) << "\n";
        stream_res << "CLASSIFICATION_LABEL_FILE_NAME : " << Singleton<ParameterBag>::GetInstance()->GetValue(StringParam::CLASSIFICATION_LABEL_FILE_NAME) << "\n";
        stream_res << "CLASSIFICATION_INPUT_LAYER_TYPE : " << Singleton<ParameterBag>::GetInstance()->GetValue(StringParam::CLASSIFICATION_INPUT_LAYER_TYPE) << "\n";
        
        stream_res << "DETECTION_THRESHOLD : " << Singleton<ParameterBag>::GetInstance()->GetValue(FloatParam::DETECTION_THRESHOLD) << "\n";
        stream_res << "CLASSIFICATION_THRESHOLD : " << Singleton<ParameterBag>::GetInstance()->GetValue(FloatParam::CLASSIFICATION_THRESHOLD) << "\n";
        stream_res << "NON_MAXIMUM_SUPPRESSION_THRESHOLD : " << Singleton<ParameterBag>::GetInstance()->GetValue(FloatParam::NON_MAXIMUM_SUPPRESSION_THRESHOLD) << "\n";
        stream_res << "DETECTION_PADDING_RATE : " << Singleton<ParameterBag>::GetInstance()->GetValue(FloatParam::DETECTION_PADDING_RATE) << "\n";
        
        stream_res << "SCREEN_WIDTH : " << Singleton<ParameterBag>::GetInstance()->GetValue(UIntParam::SCREEN_WIDTH) << "\n";
        stream_res << "SCREEN_HEIGHT : " << Singleton<ParameterBag>::GetInstance()->GetValue(UIntParam::SCREEN_HEIGHT) << "\n";
        stream_res << "DETECTION_IMAGE_WIDTH : " << Singleton<ParameterBag>::GetInstance()->GetValue(UIntParam::DETECTION_IMAGE_WIDTH_SIZE) << "\n";
        stream_res << "DETECTION_IMAGE_HEIGHT : " << Singleton<ParameterBag>::GetInstance()->GetValue(UIntParam::DETECTION_IMAGE_HEIGHT_SIZE) << "\n";
        stream_res << "DETECTION_NUM_CLASSES : " << Singleton<ParameterBag>::GetInstance()->GetValue(UIntParam::DETECTION_NUM_CLASSES) << "\n";
        stream_res << "CLASSIFICATION_IMAGE_SIZE : " << Singleton<ParameterBag>::GetInstance()->GetValue(UIntParam::CLASSIFICATION_IMAGE_SIZE) << "\n";
        stream_res << "CLASSIFICATION_NUM_CLASSES : " << Singleton<ParameterBag>::GetInstance()->GetValue(UIntParam::CLASSIFICATION_NUM_CLASSES) << "\n";
        stream_res << "DETECT_MODE : " << Singleton<ParameterBag>::GetInstance()->GetValue(UIntParam::DETECT_MODE) << "\n";
        PrintLog(stream_res);
    }

#endif // DEBUG_PRINT
    
} // namesapce ContAI
