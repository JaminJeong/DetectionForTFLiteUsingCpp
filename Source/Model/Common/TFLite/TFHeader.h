#ifndef _MODEL_COMMON_TFLITE_TFHEADER_H_
#define _MODEL_COMMON_TFLITE_TFHEADER_H_

#include "Common/Common.h"

#if defined(TENSORFLOW_1_10)
#   include "tensorflow/contrib/lite/kernels/register.h"
#   include "tensorflow/contrib/lite/model.h"
#   include "tensorflow/contrib/lite/string_util.h"
#   include "tensorflow/contrib/lite/op_resolver.h"
#elif defined(TENSORFLOW_1_13)
#   include "tensorflow/lite/kernels/register.h"
#   include "tensorflow/lite/model.h"
#   include "tensorflow/lite/string_util.h"
#   include "tensorflow/lite/op_resolver.h"
#endif // TENSORFLOW_VERSION

    
#endif // _MODEL_COMMON_TFLITE_TFHEADER_H_
