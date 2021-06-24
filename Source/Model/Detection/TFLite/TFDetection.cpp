#include <string>
#include <sstream>
#include <queue>

#include "Common/File/FilePath.h"
#include "Common/Debug/DebugUtil.h"
#include "Common/File/ParseLabel.h"

#include "Model/Detection/Result/DetectionResult.h"
#include "Model/Common/TFLite/TFHeader.h"
#include "Model/Detection/TFLite/TFDetection.h"

#if defined(DEBUG_SAVE_IMAGE)
#include "Common/Image/SaveImage.h"
#endif // DEBUG_SAVE_IMAGE

using std::string;

namespace ContAI {

    TFDetection::TFDetection(
                   const ImageShape& _inputImageShape,
                   const ImageShape& _modelInputImageShape,
                   const std::string& _modelPath,
                   const std::string& _labelPath,
                   unsigned int _NumResults,
                   float _detectionThreshold,
                   unsigned int _maxDetectionBoxNum,
                   float _nonMaximumThreshold,
                   float _tooSmallArea,
                   const std::string& _InputLayerType,
                   unsigned int _NumThreads
                   )
    :
    m_Info(
              ModelInfo(
                        _inputImageShape,
                        _modelInputImageShape,
                        _modelPath
                        ),
               _labelPath,
               _InputLayerType,
              _NumResults,
              _detectionThreshold,
              _maxDetectionBoxNum,
              _nonMaximumThreshold,
              _tooSmallArea
              ),
    m_InputLayerType(_InputLayerType),
    m_NumThreads(_NumThreads)
    {
    }

    TFDetection::TFDetection(
                   const DetectionInfo& _DetectionInfo,
                   const std::string& _InputLayerType,
                   unsigned int _NumThreads
                   )
    :m_Info(_DetectionInfo),
    m_InputLayerType(_InputLayerType),
    m_NumThreads(_NumThreads)
    {}


    bool TFDetection::Init()
    {
        m_DetectionTensorSizes = m_Info.GetModelInputImageShape().GetBHWC();

        LoadDetectionLabels(m_Info.GetLabelPath(), m_LabelIndexMap, m_LabelStringMap);
        if (m_LabelIndexMap.empty())
        {
            PrintLog("m_LabelIndexMap is empty!! \n");
            return false;
        }
        if (m_LabelStringMap.empty())
        {
            PrintLog("m_LabelStringMap is empty!! \n");
            return false;
        }

        m_TFDetection = std::unique_ptr<tflite::FlatBufferModel>
        (tflite::FlatBufferModel::BuildFromFile(m_Info.GetModelPath().c_str()));
        if (m_TFDetection == nullptr) {
            std::stringstream stream_res;
            stream_res << "Failed to mmap model" << m_Info.GetModelPath() << "\n";
            PrintLog(stream_res);
            return false;
        }
        
        {
            std::stringstream stream_res;
            stream_res << "DetectionModel Loaded model" << m_Info.GetModelPath() << "\n";
            PrintLog(stream_res);
        }
        m_TFDetection->error_reporter();

        m_NumClassses = static_cast<unsigned int>(m_LabelStringMap.size()) + 1;
        
#ifdef TFLITE_CUSTOM_OPS_HEADER
        tflite::MutableOpResolver detection_resolver;
        RegisterSelectedOps(&detection_resolver);
#else
        tflite::ops::builtin::BuiltinOpResolver detection_resolver;
#endif
        tflite::InterpreterBuilder(*m_TFDetection, detection_resolver)(&m_DetectionInterpreter);
        if (!m_DetectionInterpreter) {
            PrintLog("Failed to construct interpreter. \n");
            return false;
        }
        if (m_NumThreads != -1) {
            m_DetectionInterpreter->SetNumThreads(m_NumThreads);
        }

        if (m_DetectionInterpreter->inputs().size() == 0)
        {
            PrintLog("input vector size is 0. \n");
            return false;
        }
        m_DetectionInput = m_DetectionInterpreter->inputs()[0];

        if (m_InputLayerType != "string") {
            TfLiteStatus status = m_DetectionInterpreter->ResizeInputTensor(m_DetectionInput, m_DetectionTensorSizes);

            if (status == kTfLiteError) {
                PrintLog("Failed to ResizeInputTensor .\n");
                return false;
            }
        }

        if (m_DetectionInterpreter->AllocateTensors() != kTfLiteOk) {
            PrintLog("Failed to allocate tensors. \n");
            return false;
        }

#if defined(DEBUG_PRINT)
        PrintInterpreterInfo(*m_DetectionInterpreter);
#endif // DEBUG_PRINT
        
        return true;
    }

    std::unique_ptr<DetectionResult> TFDetection::Run(Image8& imageData)
    {
        Image8 inputTensor(m_Info.GetModelInputImageShape().GetWidth(),
                          m_Info.GetModelInputImageShape().GetHeight(),
                          m_Info.GetModelInputImageShape().GetChannel(),
                          m_DetectionInterpreter->typed_tensor<ImagePixelType>(m_DetectionInput),
                          false // own_data
                          );
#if defined(DEBUG_PRINT)
        if (m_DetectionInterpreter->typed_tensor<ImagePixelType>(m_DetectionInput) == nullptr)
        {
            PrintLog("m_DetectionInterpreter->typed_tensor<ImagePixelType>(m_DetectionInput) : Nullptr");
        }
        if (m_DetectionInterpreter->typed_tensor<float>(m_DetectionInput) == nullptr)
        {
            PrintLog("m_DetectionInterpreter->typed_tensor<float>(m_DetectionInput) : Nullptr");
        }
#endif // DEBUG_PRINT

        inputTensor.Resample(imageData);
        
#if defined(DEBUG_PRINT)
        std::stringstream stream;
        stream << "m_Info.GetInputImageShape() : " << m_Info.GetInputImageShape() << "\n";
        stream << "m_Info.GetModelInputImageShape() : " << m_Info.GetModelInputImageShape() << "\n";
        PrintLog(stream);
#endif // DEBUG_PRINT
        

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

        if (m_DetectionInterpreter->Invoke() != kTfLiteOk) {
            PrintLog("Failed to invoke! \n");
            return nullptr;
        }
        
        
        std::unique_ptr<float[]> outputLocations = std::unique_ptr<float[]> (new float[m_Info.GetMaxDetectionBoxNum() * 4]);
        std::unique_ptr<float[]> outputClasses = std::unique_ptr<float[]> (new float[m_Info.GetMaxDetectionBoxNum()]);
        std::unique_ptr<float[]> outputScores = std::unique_ptr<float[]> (new float[m_Info.GetMaxDetectionBoxNum()]);
        float numDetections[1];

        float* outputTensorLocations = m_DetectionInterpreter->typed_output_tensor<float>(0);
        float* outputTensorClasses = m_DetectionInterpreter->typed_output_tensor<float>(1);
        float* outputTensorScores = m_DetectionInterpreter->typed_output_tensor<float>(2);
        float* outputTensornumDetections = m_DetectionInterpreter->typed_output_tensor<float>(3);
        
        memcpy(outputLocations.get(), outputTensorLocations, m_Info.GetMaxDetectionBoxNum() * 4 * sizeof(float));
        memcpy(outputClasses.get(), outputTensorClasses, m_Info.GetMaxDetectionBoxNum() * sizeof(float));
        memcpy(outputScores.get(), outputTensorScores, m_Info.GetMaxDetectionBoxNum() * sizeof(float));
        memcpy(numDetections, outputTensornumDetections, sizeof(numDetections));

#if defined(DEBUG_PRINT)

        if (m_DetectionInterpreter->EnsureTensorDataIsReadable(m_DetectionInterpreter->outputs()[0]) != kTfLiteOk) {
            PrintLog("Failed to EnsureTensorDataIsReadable[0]! \n");
            return nullptr;
        }
        if (m_DetectionInterpreter->EnsureTensorDataIsReadable(m_DetectionInterpreter->outputs()[1]) != kTfLiteOk) {
            PrintLog("Failed to EnsureTensorDataIsReadable[1]! \n");
            return nullptr;
        }

        {
            std::stringstream stream_res;
            stream_res << "sizeof(ouputLocation) : " << (int) sizeof(outputLocations) << "\n";
            stream_res << "sizeof(outputClasses) : " << (int) sizeof(outputClasses) << "\n";
            PrintLog(stream_res);
        }

#endif // DEBUG_PRINT
        
        std::unique_ptr<DetectionResult> detectionResult = std::unique_ptr<DetectionResult> (new DetectionResult(m_Info));
        
#if defined(DEBUG_PRINT)
        {
            std::stringstream stream_res;
            stream_res << "m_LabelIndexMap.size() : " << m_LabelIndexMap.size() << "\n";

            for(int index =0; index < m_Info.GetMaxDetectionBoxNum(); index++)
            {
                stream_res << "outputClasses[0][" << index << "] : " << outputClasses[index] << "\n";
                stream_res << "outputScores[0][" << index << "] : " << outputScores[index] << "\n";
            }
            PrintLog(stream_res);
        }

#endif // DEBUG_PRINT
        
        for(int index = 0; index < m_Info.GetMaxDetectionBoxNum(); ++index)
        {
           
            if (outputScores[index] > m_Info.GetDetectionThreshold()) {
#if defined(DEBUG_PRINT)
                {
                    std::stringstream stream_res;
                    stream_res << "m_LabelIndexMap[fmax(outputClasses[0][" << index << "], 0)]  : " << m_LabelIndexMap[fmax(outputClasses[index], 0)].c_str() << "\n";
                    stream_res << "ouputScores[0][" << index << "] : " << outputScores[index] << "\n";
                    PrintLog(stream_res);
                }
#endif // DEBUG_PRINT
                detectionResult->Add(
                                     outputScores[index],
                                     m_LabelIndexMap[fmax(outputClasses[index], 0)],
                                     tf_tracking::BoundingBox(
                                         tf_tracking::Point2f(
                                                            outputLocations[index * 4 + 1],
                                                            outputLocations[index * 4 + 0]),
                                         tf_tracking::Point2f(
                                                             outputLocations[index * 4 + 3],
                                                            outputLocations[index * 4 + 2])
                                         )
                                     );
            }
        }
        
        if (detectionResult->IsEmpty() == true)
        {
            return nullptr;
        }
        
        detectionResult->Sort();
        
        return detectionResult;
    }

    void TFDetection::decodeCenterSizeBoxes(float* predictions, float boxPriors[4][NUM_RESULTS], float detectionPaddingRate)
    {
        const float Y_SCALE = 10.0f;
        const float X_SCALE = 10.0f;
        const float H_SCALE = 5.0f;
        const float W_SCALE = 5.0f;
        
        for (int idx = 0; idx < NUM_RESULTS; ++idx) {
            float* predictions_idx_0 = (predictions + idx * 4 + 0);
            float* predictions_idx_1 = (predictions + idx * 4 + 1);
            float* predictions_idx_2 = (predictions + idx * 4 + 2);
            float* predictions_idx_3 = (predictions + idx * 4 + 3);
            
            float ycenter = *predictions_idx_0 / Y_SCALE * boxPriors[2][idx] + boxPriors[0][idx];
            float xcenter = *predictions_idx_1 / X_SCALE * boxPriors[3][idx] + boxPriors[1][idx];
            float h = (float) exp(*predictions_idx_2 / H_SCALE) * boxPriors[2][idx];
            float w = (float) exp(*predictions_idx_3 / W_SCALE) * boxPriors[3][idx];
            
            float ymin = ycenter - h / 2.f;
            float xmin = xcenter - w / 2.f;
            float ymax = ycenter + h / 2.f;
            float xmax = xcenter + w / 2.f;
            
            *predictions_idx_0 = fmin(fmax(0, ymin * m_Info.GetModelInputImageShape().GetHeight()), m_Info.GetModelInputImageShape().GetHeight());
            *predictions_idx_1 = fmin(fmax(0, xmin * m_Info.GetModelInputImageShape().GetWidth()), m_Info.GetModelInputImageShape().GetWidth());
            *predictions_idx_2 = fmin(fmax(0, ymax * m_Info.GetModelInputImageShape().GetHeight()), m_Info.GetModelInputImageShape().GetHeight());
            *predictions_idx_3 = fmin(fmax(0, xmax * m_Info.GetModelInputImageShape().GetWidth()), m_Info.GetModelInputImageShape().GetWidth());
            
            // 65를 빼줌.
            *predictions_idx_1 -= 45.f;
            *predictions_idx_3 -= 45.f;
            *predictions_idx_0 += 37.f;
            *predictions_idx_2 += 38.f;
            
            // 나올 때 범위 안으로 해줌.
            *predictions_idx_1 = fmin(fmax(0, *predictions_idx_1 ), 210.f);
            *predictions_idx_3 = fmin(fmax(0, *predictions_idx_3 ), 210.f);
            *predictions_idx_0 = fmin(fmax(0, *predictions_idx_0 ), 375.f);
            *predictions_idx_2 = fmin(fmax(0, *predictions_idx_2 ), 375.f);
        }
    }

    void TFDetection::Softmax(
                 float* outputClasses,
                unsigned int numResults,
                 unsigned int outputClassesIndex[NUM_RESULTS],
                 float outputClassesScore[NUM_RESULTS]
                 )
    {
        // Scale them back to the input size.
        for (int i = 0; i < numResults; ++i) {
            float topClassScore = -1000.F;
            int topClassScoreIndex = -1;
            
            float sum_logit = 0.F;
            for (int idx = 1; idx < numResults; ++idx)
            {
                sum_logit += exp(*(outputClasses + i * numResults + idx));
            }
            
            // Skip the first catch-all class.
            for (int j = 1; j < numResults; ++j) {
                float score = exp(*(outputClasses + i * numResults + j)) / sum_logit;
                
                if (score > topClassScore) {
                    topClassScoreIndex = j;
                    topClassScore = score;
                }
            }
            outputClassesIndex[i] = topClassScoreIndex;
            outputClassesScore[i] = topClassScore;
        }
    }

    void TFDetection::Sigmoid(
                 float* outputClasses,
                 unsigned int numResults,
                 unsigned int outputClassesIndex[NUM_RESULTS],
                 float outputClassesScore[NUM_RESULTS]
                 )
    {
        // Scale them back to the input size.
        for (int i = 0; i < numResults; ++i) {
            float topClassScore = -1000.F;
            int topClassScoreIndex = -1;
            
            // Skip the first catch-all class.
            for (int j = 1; j < numResults; ++j) {
                float score = 1 / (1 + exp(-(*(outputClasses + i * numResults + j))));
                
                if (score > topClassScore) {
                    topClassScoreIndex = j;
                    topClassScore = score;
                }
            }
            outputClassesIndex[i] = topClassScoreIndex;
            outputClassesScore[i] = topClassScore;
        }
    }

} // namespace ContAI
