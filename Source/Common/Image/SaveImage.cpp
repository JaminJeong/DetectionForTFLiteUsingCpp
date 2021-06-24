#include "Common/Platform.h"
#include "Common/Image/Image8.h"
#include "Common/Image/SaveImage.h"

namespace ContAI {

#if defined(DEBUG_SAVE_IMAGE)
#if defined(_IOS_)
#if     defined(OPENCV)
    UIImage* UIImageFromCVMat(cv::Mat& cvMat)
    {
        NSData *data = [NSData dataWithBytes:cvMat.data length:cvMat.elemSize()*cvMat.total()];
        CGColorSpaceRef colorSpace;
        
        if (cvMat.elemSize() == 1) {
            colorSpace = CGColorSpaceCreateDeviceGray();
        } else {
            colorSpace = CGColorSpaceCreateDeviceRGB();
        }
        
        CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
        
        // Creating CGImage from cv::Mat
        CGImageRef imageRef = CGImageCreate(cvMat.cols,                                 //width
                                            cvMat.rows,                                 //height
                                            8,                                          //bits per component
                                            8 * cvMat.elemSize(),                       //bits per pixel
                                            cvMat.step[0],                            //bytesPerRow
                                            colorSpace,                                 //colorspace
                                            kCGImageAlphaNone|kCGBitmapByteOrderDefault,// bitmap info
                                            provider,                                   //CGDataProviderRef
                                            NULL,                                       //decode
                                            false,                                      //should interpolate
                                            kCGRenderingIntentDefault                   //intent
                                            );
        
        
        // Getting UIImage from CGImage
        UIImage *finalImage = [UIImage imageWithCGImage:imageRef];
        CGImageRelease(imageRef);
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpace);
        
        return finalImage;
    }
#endif // OPNECV
    
    UIImage* UIImageFromContAIImage(Image8& inputImage)
    {
        NSData *data = [NSData dataWithBytes:inputImage.data() length:inputImage.data_size_ * sizeof(ImagePixelType)];
        CGColorSpaceRef colorSpace;
        
//        if (cvMat.elemSize() == 1) {
//            colorSpace = CGColorSpaceCreateDeviceGray();
//        } else {
        colorSpace = CGColorSpaceCreateDeviceRGB();
//        }
        
        CGDataProviderRef provider = CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
        
        // Creating CGImage from cv::Mat
        CGImageRef imageRef = CGImageCreate(inputImage.GetWidth(),                          //width
                                            inputImage.GetHeight(),                         //height
                                            8,                                              //bits per component
                                            8 * sizeof(ImagePixelType),                     //bits per pixel
                                            inputImage.stride() * sizeof(ImagePixelType),   //bytesPerRow
                                            colorSpace,                                     //colorspace
                                            kCGImageAlphaNone|kCGBitmapByteOrderDefault,    // bitmap info
                                            provider,                                       //CGDataProviderRef
                                            NULL,                                           //decode
                                            false,                                          //should interpolate
                                            kCGRenderingIntentDefault                       //intent
                                            );
        
        
        // Getting UIImage from CGImage
        UIImage *finalImage = [UIImage imageWithCGImage:imageRef];
        CGImageRelease(imageRef);
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpace);
        
        return finalImage;
    }
    
    
#endif // _IOS_
#endif // DEBUG_SAVE_IMAGE

} // namespace ContAI
