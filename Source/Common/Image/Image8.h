#ifndef _COMMON_IMAGE_IMAGE8_H_
#define _COMMON_IMAGE_IMAGE8_H_

#include "Common/Common.h"

#include "Common/Debug/DebugUtil.h"
#include "Common/Image/ImageShape.h"
#include "Tracking/GoogleObjectTracker/Common/geom.h"

namespace ContAI {
    
    enum class FlipType
    {
        Horizontality = 0,
        Vertical
    };
    
    enum class ChannelType
    {
        RGB = 0,
        BGR
    };
    
    class Image8
    {
    public:
        // Constructor that creates an image from preallocated data.
        // Note: The image takes ownership of the data lifecycle, unless own_data is
        // set to false.
        Image8(const int width, const int height,
               const int channel, const ChannelType channelType);
        explicit Image8(const tf_tracking::Size& size, const int channel,
                        const ChannelType channelType);

        explicit Image8(const int width,
                        const int height,
                        const int channel,
                        const ChannelType channelType,
                        ImagePixelType* const image_data,
                        const bool own_data);
        
        ~Image8();
        
        Image8* Crop(
                    const int left,
                    const int top,
                    const int right,
                    const int bottom
                    ) const;
        
        // Perform a basic 'pixel' enlarging resample.
        void Resample(const Image8& original);
        void Flip(const FlipType flip);
        
        inline int GetWidth() const;
        inline int GetHeight() const;
        inline int GetChannel() const;
        inline const ChannelType& GetChannelType() const;
        
        const ImagePixelType* data() const;
        inline int stride() const;
        
        // Clears image to a single value.
        inline void Clear(const ImagePixelType& val);
        inline ImagePixelType& At(int x, int y, int c);
        
        inline const ImagePixelType& At(int x, int y, int c) const;
        
        static std::unique_ptr<Image8> CopyMemory(
                                                    const ImagePixelType* pixels,
                                                    const int width,
                                                    const int height,
                                                    const int channel,
                                                    const ChannelType channelType
                                                  );
        
        // Precompute these for efficiency's sake as they're used by a lot of
        // clipping code and loop code.
        // TODO(andrewharp): make these only accessible by other Images.
        const int width_less_one_;
        const int height_less_one_;
        
        // The raw size of the allocated data.
        const int data_size_;

    private:
        inline bool IsSameSize(const Image8& other) const;
        void WriteInputTensorFromImageRawData(const Image8& original);

    private:
        inline void Allocate();
        ImagePixelType* image_data_;
        
        bool own_data_;
        
        const int width_;
        const int height_;
        const int channel_;
        const ChannelType channelType_;
        
        // The image stride (offset to next row).
        // TODO(andrewharp): Make sure that stride is honored in all code.
        const int stride_;
        
#if defined(DEBUG_PRINT)
    public:
        friend std::ostream& operator<<(std::ostream& stream, const Image8& image);
#endif // DEBUG_PRINT
    };
} // ContAI

#endif // _COMMON_IMAGE_IMAGE_H_
