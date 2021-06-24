#include <vector>
#include <string>
#include <queue>
#include <iostream>

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

#include "Model/PoseEstimation/Result/PoseEstimationResult.h"

#if defined(DEBUG_PRINT)
#   include <sstream>
#endif // DEBUG_PRINT


#if defined(DEBUG_SAVE_IMAGE)
#include "Common/Image/SaveImage.h"
#endif // DEBUG_SAVE_IMAGE

using std::vector;
using std::string;

#include "Model/PoseEstimation/TFLite/TFPoseEstimation.h"

namespace ContAI{

    TFPoseEstimation::TFPoseEstimation(
                                const ImageShape&  _inputImageShape,
                                const ImageShape&  _modelInputImageShape,
                                const std::string& _modelPath,
                                const ImageShape&  _modelOutputShape,
                                const PoseEstimationInfo::PoseThresholdContainer& _thresholds,
                                const float _inputMean,
                                const float _inputStd,
                                const int _windowSize,
                                const PoseEstimationInfo::PostProcessingType& _PostProcessingType,
                                const std::string & _InputLayerType,
                                unsigned int _NumThreads
                                )
    :m_Info(
                _inputImageShape,
                _modelInputImageShape,
                _modelPath,
                _modelOutputShape,
                _thresholds,
                _inputMean,
                _inputStd,
                _windowSize,
                _PostProcessingType
            ),
    m_InputLayerType(_InputLayerType),
    m_NumThreads(_NumThreads)
    {}

    TFPoseEstimation::TFPoseEstimation(
                                const PoseEstimationInfo& _Info,
                                const std::string & _InputLayerType,
                                unsigned int _NumThreads
                                )
    :m_Info(_Info),
    m_InputLayerType(_InputLayerType),
    m_NumThreads(_NumThreads)
    {}

    bool TFPoseEstimation::Init()
    {
        m_PoseEstimationTensorSize = m_Info.GetModelInputImageShape().GetBHWC();
        std::string PoseEstimation_graph_path = m_Info.GetModelPath();

        m_TFPoseEstimation = std::unique_ptr<tflite::FlatBufferModel>(
                                                                     tflite::FlatBufferModel::BuildFromFile(
                                                                     PoseEstimation_graph_path.c_str()));

        if (m_TFPoseEstimation == nullptr) {
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
        m_TFPoseEstimation->error_reporter();
        
#ifdef TFLITE_CUSTOM_OPS_HEADER
        tflite::MutableOpResolver resolver;
        RegisterSelectedOps(&resolver);
#else
        tflite::ops::builtin::BuiltinOpResolver resolver;
#endif
        tflite::InterpreterBuilder(*m_TFPoseEstimation, resolver)(&m_PoseEstimationInterpreter);
        if (!m_PoseEstimationInterpreter) {
            PrintLog("Failed to construct interpreter. \n" );
            return false;
        }
        if (m_NumThreads != -1) {
            m_PoseEstimationInterpreter->SetNumThreads(m_NumThreads);
        }
        
        m_InputNum = m_PoseEstimationInterpreter->inputs()[0];
        if (m_InputLayerType != "string") {
            // { kTfLiteOk = 0, kTfLiteError = 1 } TfLiteStatus;
            TfLiteStatus status = m_PoseEstimationInterpreter->ResizeInputTensor(m_InputNum, m_PoseEstimationTensorSize);
            if (status == kTfLiteError) {
                PrintLog("Failed to ResizeInputTensor .\n");
                return false;
            }
        }
        
        if (m_PoseEstimationInterpreter->AllocateTensors() != kTfLiteOk) {
            PrintLog("Failed to allocate tensors.\n");
            return false;
        }

#if defined(DEBUG_PRINT)
        PrintInterpreterInfo(*m_PoseEstimationInterpreter);
#endif // DEBUG_PRINT
        
        return true;
    }

    std::unique_ptr<PoseEstimationResult> TFPoseEstimation::Run(Image8& imageData)
    {
#if defined(DEBUG_TIME_PRINT)
        std::stringstream ssTime;
        CaTime time;
        time.start();
#endif // Start Time
        Image32 inputTensor(m_Info.GetModelInputImageShape().GetWidth(),
                            m_Info.GetModelInputImageShape().GetHeight(),
                            m_Info.GetModelInputImageShape().GetChannel(),
                            ChannelType::RGB,
                            m_PoseEstimationInterpreter->typed_tensor<float>(m_InputNum),
                            false // own_data
                            );
        
        inputTensor.Resample(imageData, m_Info.GetPoseInputMean(), m_Info.GetPoseInputStd());


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
            
            const float* input = inputTensor.data();
            
            for (int idxW = 0; idxW < m_Info.GetModelInputImageShape().GetWidth(); ++idxW) {
                for (int idxH = 0; idxH < m_Info.GetModelInputImageShape().GetHeight(); ++idxH) {
                    for (int idxC = 0; idxC < m_Info.GetModelInputImageShape().GetChannel(); ++idxC) {
                        image.data[idxW * m_Info.GetModelInputImageShape().GetHeight() * m_Info.GetModelInputImageShape().GetChannel()
                                   + idxH  * m_Info.GetModelInputImageShape().GetChannel()
                                   + idxC]
                        = (uint8_t) ((input[idxW * m_Info.GetModelInputImageShape().GetHeight() * m_Info.GetModelInputImageShape().GetChannel()
                                           + idxH  * m_Info.GetModelInputImageShape().GetChannel()
                                           + idxC] * m_Info.GetPoseInputStd() + m_Info.GetPoseInputMean()));
                    }
                }
            }
            
            {
                UIImage *finalImage = UIImageFromCVMat(image);
                UIImageWriteToSavedPhotosAlbum(finalImage, nil, nil, nil);
            }
        }
        
#endif //  DEBUG_SAVE_IMAGE
        
        
        if (m_PoseEstimationInterpreter->Invoke() != kTfLiteOk) {
            PrintLog("Failed to invoke! \n");
            return nullptr;;
        }
        
        float* output = m_PoseEstimationInterpreter->typed_output_tensor<float>(0);
        std::unique_ptr<PoseEstimationResult> poseEstimationResult = std::unique_ptr<PoseEstimationResult>(new PoseEstimationResult());

        if (m_Info.GetPostProcessingType() == PoseEstimationInfo::PostProcessingType::CONFIDENCEMAP)
        {
            poseEstimationResult -> OuputClone(output, m_Info.GetModelOutputShape());
        }

        if (m_Info.GetPostProcessingType() == PoseEstimationInfo::PostProcessingType::COORDINATE)
        {
            poseEstimationResult -> GetCoordinates(output, m_Info.GetModelOutputShape(), m_Info.GetPoseThresholdList());
        }

        if (m_Info.GetPostProcessingType() == PoseEstimationInfo::PostProcessingType::WEIGHTEDAVERAGE)
        {
            poseEstimationResult -> WeightedAverage(output, m_Info.GetModelOutputShape(),
                                                    m_Info.GetPoseThresholdList(), m_Info.GetWindowSize() / 2);
        }
#if defined(DEBUG_TIME_PRINT)
        time.end();
        ssTime << time;
        PrintLog(ssTime);
#endif // End Time & Print
       
        
        return poseEstimationResult;
    }

} // namespace ContAI
