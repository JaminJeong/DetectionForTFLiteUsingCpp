/* This sample code implements PoseEstimation which estimates 14 joints from a image */

#include "Common/File/FilePath.h"
#include "Common/Image/Image8.h"

#include <string>
#include <memory>
#include <sstream>

#include "Model/PoseEstimation/PoseEstimation.h"
#include "Model/PoseEstimation/Result/PoseEstimationResult.h"

using namespace ContAI;

int m_nWidth;
int m_nHeigth;
const unsigned int channel = 3;

std::unique_ptr<PoseEstimation> poseEstimation = nullptr;
std::string pose_model_file("optimized_sa_mobile_yaxis_192_1stack_bilinear_discount_structure");

namespace {
    
    enum JointType
    {
        // 14 joint names
        RightAnkle = 0, RightKnee, RightHip,
        LeftHip, LeftKnee, LeftAnkle,
        RightWrist, RightElbow, RightShoulder,
        LeftShoulder, LeftElbow, LeftWrist,
        Neck, HeadTop
    };
    
    bool PoseEstimationInit(uint nWidth,uint nHeight)
    {
        // thresholds mean that probability of each joints should be greater than threshold values
        PoseEstimationInfo::PoseThresholdContainer thresholds = {0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3, 0.3,0.3, 0.3};
        
        poseEstimation = std::unique_ptr<PoseEstimation>(new PoseEstimation(
                                                                            PoseEstimationInfo(
                                                                                               ImageShape(nWidth, nHeight, channel),
                                                                                               ImageShape(144, 256, channel),
                                                                                               GetPath(pose_model_file, "lite"), // if you want to input Native Path
                                                                                               ImageShape(36, 64, 14),
                                                                                               thresholds, // if confidence score is less than threshold, then output -99 instead of coordinates
                                                                                               127.5,
                                                                                               127.5,
                                                                                               7, // window size for weighted average
                                                                                               PoseEstimationInfo::PostProcessingType::WEIGHTEDAVERAGE // select in [CONFIDENCEMAP, COORDINATE, WEIGHTEDAVERAGE]
                                                                                               )
                                                                            ));
        if (poseEstimation->Init() == false)
        {
            NSLog(@"poseEstimation->Init() failed !! ");
            return false;
        }
        return true;
    }
    
    bool PoseEstimationRun(void* pTexImage)
    {
        std::unique_ptr<Image8> image = Image8::CopyMemory(
                                                           (ImagePixelType*) pTexImage,
                                                           m_nWidth,
                                                           m_nHeigth,
                                                           3,
                                                           ChannelType::RGB // if you get BGR image from iPhone, change this to ChannelType::BGR
                                                           );
        
        std::unique_ptr<PoseEstimationResult> result = poseEstimation->Run(*image);
        if (result != nullptr) {
            std::stringstream stream;
            const PoseEstimationResult::Container& resultVector = result->GetElementList();
            const PoseEstimationResult::Element& rAnkle = resultVector[RightAnkle];
            
            stream << "Right Ankle PoseEstimationResult \n";
            stream << "Score : " << rAnkle.Score << "\n";
            // The coordinates obtained frome Element class have range [0,1) so that developer should rescale range of coordinates based on real resolution.
            stream << "X Coordinate : " << rAnkle.X << "\n";
            stream << "Y Coordinate : " << rAnkle.Y << "\n";
            stream << "\n";
            NSLog(@"%s", stream.str().c_str());
        }
        else {
            NSLog(@"result is nullptr !! ");
            return false;
        }
        
        return true;
    }
}
