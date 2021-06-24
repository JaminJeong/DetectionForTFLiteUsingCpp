#ifndef _COMMON_PARAMETER_PARAMETER_BAG_H_
#define _COMMON_PARAMETER_PARAMETER_BAG_H_

#include <string>
#include <map>
#include <mutex>

#include "Common/Parameter/ParameterKey.h"
#include "Common/Parameter/Singleton.h"

namespace ContAI {

    class ParameterBag : public Singleton<ParameterBag>
    {
    private:
        std::map<StringParam, std::string> stringMap;
        std::map<FloatParam, float> floatMap;
        std::map<UIntParam, unsigned int> uintMap;
        
    public:
        void Init(
                  unsigned int screen_width,
                  unsigned int screen_height,
                  unsigned int screen_channel,
                  const std::string& detection_model_file_name,
                  const std::string& detection_label_file_name,
                  const std::string& detection_input_layer_type,
                  const std::string& classification_model_file_name,
                  const std::string& classification_label_file_name,
                  const std::string& classification_input_layer_type,
                  unsigned int max_detection_box_num,
                  float detection_threshold,
                  float classification_threshold,
                  float non_maximum_suppression_threshold,
                  float detection_padding_rate,
                  float too_small_area,
                  unsigned int detection_image_width_size = 224,
                  unsigned int detection_image_height_size = 400,
                  unsigned int classification_image_size = 224
                  )
        {
            if (IsInit() == true)
            {
                return;
            }
            
            stringMap[StringParam::DETECTION_MODEL_FILE_NAME] = detection_model_file_name;
            stringMap[StringParam::DETECTION_LABEL_FILE_NAME] = detection_label_file_name;
            stringMap[StringParam::DETECTION_INPUT_LAYER_TYPE] = detection_input_layer_type;
            stringMap[StringParam::CLASSIFICATION_MODEL_FILE_NAME] = classification_model_file_name;
            stringMap[StringParam::CLASSIFICATION_LABEL_FILE_NAME] = classification_label_file_name;
            stringMap[StringParam::CLASSIFICATION_INPUT_LAYER_TYPE] = classification_input_layer_type;
            
            floatMap[FloatParam::DETECTION_THRESHOLD] = detection_threshold;
            floatMap[FloatParam::CLASSIFICATION_THRESHOLD] = classification_threshold;
            floatMap[FloatParam::NON_MAXIMUM_SUPPRESSION_THRESHOLD] = non_maximum_suppression_threshold;
            floatMap[FloatParam::DETECTION_PADDING_RATE] = detection_padding_rate;
            floatMap[FloatParam::TOO_SMALL_AREA] = too_small_area;
            
            uintMap[UIntParam::SCREEN_WIDTH] = screen_width;
            uintMap[UIntParam::SCREEN_HEIGHT] = screen_height;
            uintMap[UIntParam::SCREEN_CHANNEL] = screen_channel;
            uintMap[UIntParam::DETECTION_IMAGE_WIDTH_SIZE] = detection_image_width_size;
            uintMap[UIntParam::DETECTION_IMAGE_HEIGHT_SIZE] = detection_image_height_size;
            uintMap[UIntParam::CLASSIFICATION_IMAGE_SIZE] = classification_image_size;
            uintMap[UIntParam::MAX_DETECTION_BOX_NUM] = max_detection_box_num;
            
        }
        
        bool IsInit() const
        {
            if(stringMap.empty() == false
               && floatMap.empty() == false
               && uintMap.empty() == false
               )
            {
                return true;
            }
            
            return false;
        }
        
        void SetValue(UIntParam _key, unsigned int _value)
        {
            uintMap[_key] = _value;
        }
        
        std::string GetValue(StringParam _key)
        {
            return stringMap[_key];
        }
        float GetValue(FloatParam _key)
        {
            return floatMap[_key];
        }
        unsigned int GetValue(UIntParam _key)
        {
            return uintMap[_key];
        }
    };

} // namespace ContAI

#endif // _COMMON_PARAMETER_PARAMETER_BAG_H_
