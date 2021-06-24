#import <UIKit/UIKit.h>
#import <Accelerate/Accelerate.h>

#include "Common/File/FilePath.h"
#include "Common/Image/Image.h"
#include "SPLocationTracker.h"
#include <string>
#include <memory>

#include "Module/DetectionToClassification/DetectionToClassification.h"
#include "Module/DetectionToClassification/Result/DetectionToClassificationResult.h"
#include "Model/Detection/Detection.h"
#include "Model/Detection/Result/DetectionResult.h"

#define DETECT_TO_CLASSIFY_MODE 0
#define DETECT_MODE 1

using namespace ContAI;

int m_nWidth;
int m_nHeigth;
const unsigned int channel = 3;

std::unique_ptr<DetectionToClassification> detecToClass = nullptr;
std::unique_ptr<Detection> detection = nullptr;

std::string car_detection_model_file("tflite_graph");
std::string car_detection_label_file("99class_can_snow_label");
std::string object_detection_model_file("tflite_graph");
std::string object_detection_label_file("99class_can_snow_label");

std::string recog_model_file("frozen_quant_can");
std::string recog_label_file("cl_label");

tf_tracking::BoundingBox screenBox;
float screenBoxDiagonal;

const float alpha = 0.5;
const float beta = 0.8;

namespace {
    
    void PrintFilePath()
    {
        NSLog(@"recog_model_file : %s", GetPath(recog_model_file, "tflite").c_str());
        NSLog(@"recog_label_file : %s", GetPath(recog_label_file, "txt").c_str());
        NSLog(@"object_detection_model_file : %s", GetPath(object_detection_model_file, "tflite").c_str());
        NSLog(@"object_detection_label_file : %s", GetPath(object_detection_label_file, "txt").c_str());
    }
    
    std::string DetectionResultToJson(const DetectionResult::Container& elementList)
    {
        std::stringstream strStream;
        
        strStream << "{\"Recog\":[";
        
        for (DetectionResult::ConstIterator itr = elementList.begin();
             itr != elementList.end();
             ++itr)
        {
            strStream << "{\"ModelName\":\"" << itr->LabelName << "\"," ;
            // if you want Score of Detection
            //strStream << "\"score\":" << itr->m_DetectionElement.Score << ",";
            strStream << "\"x\":" << itr->DetectBox.left_ << ",";
            strStream << "\"y\":" << itr->DetectBox.top_ << ",";
            strStream << "\"w\":" << itr->DetectBox.GetWidth() << ",";
            strStream << "\"h\":" << itr->DetectBox.GetHeight();
            DetectionResult::Container::const_iterator isEnd = itr + 1;
            if (isEnd != elementList.end()) {
                strStream << "},";
            }
            else {
                strStream << "}]}";
            }
        }
        
        return strStream.str();
    }
    
    // 결과 출력 형식을 조정 할수 있습니다
    // LabelName, Score, Detection 위치 정보
    // Detection 정보는 left top right bottom 정보와 정중앙 값음 이용할수 있습니다
    // Score 는 classificatino Score와 Detection Score를 쓸수 있습니다.
    std::string DetectionToClassificationResultToJson(const DetectionToClassificationResult::Container& elementList)
    {
        std::stringstream strStream;
        
        strStream << "{\"Recog\":[";
        
        for (DetectionToClassificationResult::ConstIterator itr = elementList.begin();
             itr != elementList.end();
             ++itr)
        {
            std::string modelName = itr->m_DetectionElement.LabelName;
            if (itr->m_ClassificationResult.size() != 0) {
                modelName = itr->m_ClassificationResult[0].LabelName;
                // Score of Classification
                // itr->m_ClassificationResult[0].Score
            }
            strStream << "{\"ModelName\":\"" << modelName << "\"," ;
            // if you want Scores of Detection
            //strStream << "\"score\":" << itr->m_DetectionElement.Score << ",";
            strStream << "\"x\":" << itr->m_DetectionElement.DetectBox.left_ << ",";
            strStream << "\"y\":" << itr->m_DetectionElement.DetectBox.top_ << ",";
            strStream << "\"w\":" << itr->m_DetectionElement.DetectBox.GetWidth() << ",";
            strStream << "\"h\":" << itr->m_DetectionElement.DetectBox.GetHeight();
            DetectionToClassificationResult::ConstIterator isEnd = itr + 1;
            if (isEnd != elementList.end()) {
                strStream << "},";
            }
            else {
                strStream << "}]}";
            }
        }
        
        return strStream.str();
    }
    
    bool DetectionInit(uint nWidth,uint nHeight)
    {
        screenBox = tf_tracking::BoundingBox(tf_tracking::Point2f(0, 0),tf_tracking::Point2f(nWidth, nHeight));
        screenBoxDiagonal = tf_tracking::Point2f(0, 0).Distance(tf_tracking::Point2f(nWidth, nHeight));
        
        detection = std::unique_ptr<Detection>(new Detection(
                                                             DetectionInfo(
                                                                           ImageShape(nWidth, nHeight, channel),
                                                                           ImageShape(224, 400, channel),
                                                                           GetPath(object_detection_model_file, "tflite"), // if you want to input Native Path
                                                                           GetPath(object_detection_label_file, "txt"), // if you want to input Native Path
                                                                           10, // number of Detection object
                                                                           0.1, // detection Threshold
                                                                           10, // number of Detection object
                                                                           0.6, // dont use now : nonmaxiuum threshold
                                                                           0.01 // dont use now : paramter for removing small area
                                                                           )
                                                             ));
        if (detection->Init() == false)
        {
            NSLog(@"detection->Init() failed !! ");
            return false;
        }
        return true;
    }
    
    bool DetectionRun(void* pTexImage)
    {
        const int imageSize = m_nWidth * m_nHeigth * 3;
        ImagePixelType* data = new ImagePixelType[imageSize];
        memcpy(data, pTexImage, imageSize);
        Image image(m_nWidth,
                    m_nHeigth,
                    3,
                    data,
                    true);
        //        std::unique_ptr<Image> image = std::unique_ptr<Image>(
        //                                                              Image::CopyMemory(
        //                                                         (const ImagePixelType*)pTexImage,
        //                                                         (const int)m_nWidth,
        //                                                         (const int)m_nHeigth,
        //                                                         (const int)3)
        //                                                              );
        
        std::unique_ptr<DetectionResult> result = detection->Run(image);
        if (result != nullptr) {
            result->Sort();
            result->ChangeRectCoordinate(DetectionResult::RectCoord::ImageInputSize);
            
            if (result->IsEmpty() == false) {
                // 람다를 이용하여
                // 물체가 가운데에 있을때 가중치를 줄지
                // 물체의 정확도가 높은 경우 가중치를 줄지 결정할수 있습니다
                std::sort(result->GetElementList().begin(), result->GetElementList().end(),
                          [](const DetectionResult::Element& left, const DetectionResult::Element& right) {
                              float leftValue = alpha *left.Score + beta * (1.0 - left.DetectBox.GetCenter().Distance(screenBox.GetCenter()) / screenBoxDiagonal);
                              float rightValue = alpha * right.Score + beta * (1.0 - right.DetectBox.GetCenter().Distance(screenBox.GetCenter()) / screenBoxDiagonal);
                              return leftValue > rightValue;
                          });
            }
#if defined(DEBUG_PRINT)
            {
                std::stringstream stream;
                stream << *result;
                NSLog(@"%s", stream.str().c_str());
            }
#endif // DEBUG_PRINT
            //            NSLog(@"ToJson : %s", result->ToJson().c_str());
            NSLog(@"ToJson : %s", DetectionResultToJson(result->GetElementList()).c_str());
            
            NSString *szJson2 = [NSString stringWithFormat:@"%s", DetectionResultToJson(result->GetElementList()).c_str()];
            UnitySendMessage("iOSManager", "NativeCallSetJsonData",[szJson2 UTF8String]);
        }
        else {
            //            NSString *szJson2 = [NSString stringWithFormat:@""];
            //            UnitySendMessage("iOSManager", "NativeCallSetJsonData",[szJson2 UTF8String]);
            NSLog(@"result is nullptr !! ");
            return false;
        }
        
        return true;
    }
    
    bool DetectionToClassificationInit(uint nWidth,uint nHeight)
    {
        screenBox = tf_tracking::BoundingBox(tf_tracking::Point2f(0, 0),tf_tracking::Point2f(nWidth, nHeight));
        screenBoxDiagonal = tf_tracking::Point2f(0, 0).Distance(tf_tracking::Point2f(nWidth, nHeight));
        ClassificationInfo::CategoryContainer category = {"can"};
        
        detecToClass = std::unique_ptr<DetectionToClassification>(new DetectionToClassification(
                                                                                                
                                                                                                ClassificationInfo(
                                                                                                                   ImageShape(nWidth, nHeight, channel),
                                                                                                                   ImageShape(224, 224, channel),
                                                                                                                   GetPath(recog_model_file, "tflite"), // if you want to input Native Path
                                                                                                                   GetPath(recog_label_file, "txt"), // if you want to input Native Path
                                                                                                                   0.1,
                                                                                                                   category
                                                                                                                   ),
                                                                                                DetectionInfo(
                                                                                                              ImageShape(nWidth, nHeight, channel),
                                                                                                              ImageShape(224, 400, channel),
                                                                                                              GetPath(object_detection_model_file, "tflite"), // if you want to input Native Path
                                                                                                              GetPath(object_detection_label_file, "txt"), // if you want to input Native Path
                                                                                                              10,
                                                                                                              0.1,
                                                                                                              10,
                                                                                                              0.6,
                                                                                                              0.01
                                                                                                              )
                                                                                                ));
        if (detecToClass->Init() == false)
        {
            NSLog(@"detecToClass->Init() failed !! ");
            return false;
        }
        
        return true;
    }
    
    bool DetectionToClassificationRun(void* pTexImage)
    {
        const int imageSize = m_nWidth * m_nHeigth * 3;
        ImagePixelType* data = new ImagePixelType[imageSize * sizeof(ImagePixelType)];
        memcpy(data, pTexImage, imageSize * sizeof(ImagePixelType));
        Image image(m_nWidth,
                    m_nHeigth,
                    3,
                    data,
                    true);
        //        std::unique_ptr<Image> image = std::unique_ptr<Image>(
        //                                                              Image::CopyMemory(
        //                                                         (const ImagePixelType*)pTexImage,
        //                                                         (const int)m_nWidth,
        //                                                         (const int)m_nHeigth,
        //                                                         (const int)3)
        //                                                              );
        
        
        std::unique_ptr<DetectionToClassificationResult> result = detecToClass->Run(image);
        if (result != nullptr) {
            result->Sort();
            result->ChangeRectCoordinate(DetectionResult::RectCoord::ImageInputSize);
            
            if (result->IsEmpty() == false) {
                // 람다를 이용하여
                // 물체가 가운데에 있을때 가중치를 줄지
                // 물체의 정확도가 높은 경우 가중치를 줄지 결정할수 있습니다
                std::sort(result->GetElementList().begin(), result->GetElementList().end(),
                          [](const DetectionToClassificationResult::Element& left, const DetectionToClassificationResult::Element& right) {
                              float leftValue = alpha * left.m_DetectionElement.Score + beta * (1.0 - left.m_DetectionElement.DetectBox.GetCenter().Distance(screenBox.GetCenter()) / screenBoxDiagonal);
                              float rightValue = alpha * right.m_DetectionElement.Score + beta * (1.0 - right.m_DetectionElement.DetectBox.GetCenter().Distance(screenBox.GetCenter()) / screenBoxDiagonal);
                              return leftValue > rightValue;
                          });
            }
            
            // 결과를 하나만 가져오고 싶을. 때
            //            result->Top1();
#if defined(DEBUG_PRINT)
            {
                std::stringstream stream;
                stream << *result;
                NSLog(@"%s", stream.str().c_str());
            }
#endif // DEBUG_PRINT
            //            NSLog(@"ToJson : %s", result->ToJson().c_str());
            NSLog(@"ToJson : %s", DetectionToClassificationResultToJson(result->GetElementList()).c_str());
            
            NSString *szJson2 = [NSString stringWithFormat:@"%s", DetectionToClassificationResultToJson(result->GetElementList()).c_str()];
            UnitySendMessage("iOSManager", "NativeCallSetJsonData",[szJson2 UTF8String]);
            return true;
        }
        else {
            //            NSString *szJson2 = [NSString stringWithFormat:@""];
            //            UnitySendMessage("iOSManager", "NativeCallSetJsonData",[szJson2 UTF8String]);
            NSLog(@"result is nullptr !! ");
            return false;
        }
        
    }
}



extern "C"
{
    static SPLocationTracker * m_pLocation = nil;
    
    void iOSPluginInitPipeline(uint nWidth,uint nHeight , int nDetectMode)
    {
        m_nWidth = nWidth;
        m_nHeigth = nHeight;
        
        if (nDetectMode == DETECT_MODE) {
            DetectionInit(nWidth, nHeight);
        }
        else if (nDetectMode == DETECT_TO_CLASSIFY_MODE) {
            DetectionToClassificationInit(nWidth, nHeight);
        }
        
    }
    
    void iOSPluginGetPipelineInfo(void* pTexImage, int nDetectMode)
    {
        
        if (nDetectMode == DETECT_MODE) {
            DetectionRun(pTexImage);
        }
        else if (nDetectMode == DETECT_TO_CLASSIFY_MODE) {
            DetectionToClassificationRun(pTexImage);
        }
    }
    
    void iOSPluginFinish()
    {
        detecToClass = nullptr;
        detection = nullptr;
    }
    
    
    void iOSPluginInitPipelineTF(uint nWidth,uint nHeight, int nDetectMode)
    {
        iOSPluginInitPipeline(nWidth, nHeight, nDetectMode);
    }
    
    void iOSPluginGetPipelineInfoTF(void* pTexImage,int nDetectMode)
    {
        iOSPluginGetPipelineInfo(pTexImage, nDetectMode);
    }
    
    
    void iOSPluginFinishTF()
    {
        iOSPluginFinish();
    }
    
    
    void iOSPluginGetLocation()
    {
        if( m_pLocation == nil )
        {
            m_pLocation = [SPLocationTracker alloc];
        }
        [m_pLocation Init];
    }
}

