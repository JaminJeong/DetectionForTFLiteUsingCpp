#include <vector>
#include <string>
#include <queue>

//add opencv library
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

#include "Common/File/FilePath.h"
//#include "Common/SaveImage.h"

#include "Common/Debug/DebugUtil.h"

//#include "Common/ParameterBag.h"
#include "Common/File/ParseLabel.h"
#include "Common/Common.h"
#include "Model/Common/TFLite/TFHeader.h"

#include "Model/Classification/Result/ClassificationResult.h"

#if defined(DEBUG_PRINT)
#   include <sstream>
#endif // DEBUG_PRINT


#if defined(DEBUG_SAVE_IMAGE)
#include "Common/Image/SaveImage.h"
#endif // DEBUG_SAVE_IMAGE

using std::vector;
using std::string;

#include "Model/Classification/TFLite/TFClassification.h"
#include "Model/Classification/ClassificationUtil.h"

namespace ContAI {

    TFClassification::TFClassification(
                                 const ImageShape& _inputImageShape,
                                 const ImageShape& _modelInputImageShape,
                                 const std::string & _ModelPath,
                                 const std::string & _LabelPath,
                                 float _classificationThreshold,
                                 const ClassificationInfo::CategoryContainer& _category,
                                 const std::string & _InputLayerType,
                                 unsigned int _NumThreads
                                 )
    :m_Info(
                _inputImageShape,
                _modelInputImageShape,
                _ModelPath,
                _LabelPath,
                _classificationThreshold,
                _category
            ),
            m_InputLayerType(_InputLayerType),
            m_NumThreads(_NumThreads)
    {}

    TFClassification::TFClassification(
                                 const ClassificationInfo& _Info,
                                 const std::string & _InputLayerType,
                                 unsigned int _NumThreads
                                 )
    :m_Info(_Info),
    m_InputLayerType(_InputLayerType),
    m_NumThreads(_NumThreads)
    {}

    bool TFClassification::Init()
    {
        m_ClassificationTensorSize = m_Info.GetModelInputImageShape().GetBHWC();
        std::string classification_graph_path = m_Info.GetModelPath();
        LoadClassificatonLabels(m_Info.GetLabelPath(), m_LabelList);
        
        if (m_LabelList.empty())
        {
            PrintLog("m_LabelList is empty!!");
            return false;
        }
        else
        {
            m_NumClassses = static_cast<unsigned int>(m_LabelList.size());
        }
        
        
        m_TFClassification = std::unique_ptr<tflite::FlatBufferModel>(
                                                                     tflite::FlatBufferModel::BuildFromFile(
                                                                     classification_graph_path.c_str()));
        if (m_TFClassification == nullptr) {
            std::stringstream stream_res;
            stream_res << "Failed to mmap model "<< m_Info.GetModelPath() << "\n";
            PrintLog(stream_res);
            return false;
        }
        {
            std::stringstream stream_res;
            stream_res << "Loaded model "<< m_Info.GetModelPath() << "\n";
            PrintLog(stream_res);
        }
        m_TFClassification->error_reporter();
        
        
#ifdef TFLITE_CUSTOM_OPS_HEADER
        tflite::MutableOpResolver resolver;
        RegisterSelectedOps(&resolver);
#else
        tflite::ops::builtin::BuiltinOpResolver resolver;
#endif
        tflite::InterpreterBuilder(*m_TFClassification, resolver)(&m_ClassificationInterpreter);
        if (!m_ClassificationInterpreter) {
            PrintLog("Failed to construct interpreter. \n" );
            return false;
        }
        if (m_NumThreads != -1) {
            m_ClassificationInterpreter->SetNumThreads(m_NumThreads);
        }

        if (m_ClassificationInterpreter->inputs().size() == 0)
        {
            PrintLog("input vector size is 0. \n");
            return false;
        }

        m_InputNum = m_ClassificationInterpreter->inputs()[0];
        if (m_InputLayerType != "string") {
            // { kTfLiteOk = 0, kTfLiteError = 1 } TfLiteStatus;
           TfLiteStatus status = m_ClassificationInterpreter->ResizeInputTensor(m_InputNum, m_ClassificationTensorSize);
            if (status == kTfLiteError) {
                PrintLog("Failed to ResizeInputTensor .\n");
                return false;
            }
        }
        
        if (m_ClassificationInterpreter->AllocateTensors() != kTfLiteOk) {
            PrintLog("Failed to allocate tensors.\n");
            return false;
        }
        
#if defined(DEBUG_PRINT)
        PrintInterpreterInfo(*m_ClassificationInterpreter);
#endif // DEBUG_PRINT
        
        return true;
    }

    std::unique_ptr<ClassificationResult> TFClassification::Run(Image8& imageData)
    {
        Image8 inputTensor(m_Info.GetModelInputImageShape().GetWidth(),
                          m_Info.GetModelInputImageShape().GetHeight(),
                          m_Info.GetModelInputImageShape().GetChannel(),
                          m_ClassificationInterpreter->typed_tensor<uint8_t>(m_InputNum),
                          false // own_data
                          );
        inputTensor.Resample(imageData);
        
        
#if defined(DEBUG_SAVE_IMAGE) && !defined(OPENCV)
        UIImage* saveImage = UIImageFromContAIImage(inputTensor);
        UIImageWriteToSavedPhotosAlbum(saveImage, nil, nil, nil);
#endif // DEBUG_SAVE_IMAGE
        
        
#if defined(DEBUG_SAVE_IMAGE) && defined(OPENCV)
        {
            // save test image
            cv::Mat image(
                          m_Info.GetModelInputImageShape().GetHeight(),
                          m_Info.GetModelInputImageShape().GetWidth(),
                          (m_Info.GetModelInputImageShape().GetChannel() == 3)?CV_8UC3:CV_8UC1
                          );
            
            const uint8_t* input = inputTensor.data();
            
            for (int idxW = 0; idxW < m_Info.GetModelInputImageShape().GetWidth(); ++idxW) {
                for (int idxH = 0; idxH < m_Info.GetModelInputImageShape().GetHeight(); ++idxH) {
                    for (int idxC = 0; idxC < m_Info.GetModelInputImageShape().GetChannel(); ++idxC) {
                        image.data[idxW * m_Info.GetModelInputImageShape().GetHeight() * m_Info.GetModelInputImageShape().GetChannel()
                                   + idxH  * m_Info.GetModelInputImageShape().GetChannel()
                                   + idxC]
                        = (uint8_t) (input[
                                            idxW * m_Info.GetModelInputImageShape().GetHeight() * m_Info.GetModelInputImageShape().GetChannel()
                                            + idxH  * m_Info.GetModelInputImageShape().GetChannel()
                                            + idxC]);
                    }
                }
            }
//            memcpy(image.data, input, imageData.data_size_);
            
            {
                UIImage *finalImage = UIImageFromCVMat(image);
                UIImageWriteToSavedPhotosAlbum(finalImage, nil, nil, nil);
            }
        }
        
#endif //  DEBUG_SAVE_IMAGE
        
        if (m_ClassificationInterpreter->Invoke() != kTfLiteOk) {
            PrintLog("Failed to invoke! \n");
            return nullptr;;
        }
        
        uint8_t* output = m_ClassificationInterpreter->typed_output_tensor<uint8_t>(0);
        const int kNumResults = 5;
        std::vector<std::pair<float, int> > top_results;
        ClassificationUtil::GetTopN(output,
                        m_NumClassses,
                        kNumResults,
                        m_Info.GetClassificationThreshold(),
                        &top_results);
        
        std::unique_ptr<ClassificationResult> classificationResult = std::unique_ptr<ClassificationResult>(new ClassificationResult());
        for (std::vector<std::pair<float, int> >::const_iterator itr = top_results.begin();
             itr != top_results.end();
             ++itr)
        {
            classificationResult->Add(itr->first, m_LabelList[itr->second]);
        }
        
        return classificationResult;
    }

} // namespace ContAI
