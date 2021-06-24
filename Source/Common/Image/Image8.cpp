#include "Image8.h"
#include <memory>

namespace ContAI {
    
    Image8::Image8(const int width,   const int height,
                   const int channel, const ChannelType channelType)
    : width_less_one_(width - 1),
    height_less_one_(height - 1),
    data_size_(width * height * channel),
    own_data_(true),
    width_(width),
    height_(height),
    channel_(channel),
    channelType_(channelType),
    stride_(width * channel) {
        Allocate();
    }
    
    Image8::Image8(const tf_tracking::Size& size, const int channel,
                   const ChannelType channelType)
    : width_less_one_(size.width - 1),
    height_less_one_(size.height - 1),
    data_size_(size.width * size.height * channel),
    own_data_(true),
    width_(size.width),
    height_(size.height),
    channel_(channel),
    channelType_(channelType),
    stride_(size.width * channel) {
        Allocate();
    }
    
    Image8::Image8(const int width,
                   const int height,
                   const int channel,
                   const ChannelType channelType,
                   ImagePixelType* const image_data,
                   const bool own_data):
    width_less_one_(width - 1),
    height_less_one_(height - 1),
    data_size_(width * height * channel),
    own_data_(own_data),
    width_(width),
    height_(height),
    channel_(channel),
    channelType_(channelType),
    stride_(width * channel) {
        image_data_ = image_data;
        SCHECK(image_data_ != NULL, "Can't create image with NULL data!");
    }
    
    Image8::~Image8() {
        if (own_data_) {
            delete[] image_data_;
        }
        image_data_ = NULL;
    }
    
    Image8* Image8::Crop(
                   const int left,
                   const int top,
                   const int right,
                   const int bottom
                   ) const {
        SCHECK(left >= 0 && left < width_, "out of bounds at %d!", left);
        SCHECK(right >= 0 && right < width_, "out of bounds at %d!", right);
        SCHECK(top >= 0 && top < height_, "out of bounds at %d!", top);
        SCHECK(bottom >= 0 && bottom < height_, "out of bounds at %d!", bottom);
        
        SCHECK(left <= right, "mismatch!");
        SCHECK(top <= bottom, "mismatch!");
        
        const int new_width = right - left + 1;
        const int new_height = bottom - top + 1;
        const int new_channel = channel_;
        
        Image8* const cropped_image = new Image8(new_width, new_height,
                                                 new_channel, channelType_);
        
        for (int x = 0; x < new_width; ++x) {
            for (int y = 0; y < new_height; ++y) {
                for (int c = 0; c < new_channel; ++c) {
                    cropped_image->At(x, y, c) = At(x + left, y + top, c);
                }
            }
        }
        
        return cropped_image;
    }
    
    // Perform a basic 'pixel' enlarging resample.
    void Image8::Resample(const Image8& original)
    {
        if(IsSameSize(original))
        {
            memcpy(image_data_, original.data(), data_size_);
            return;
        }
        else
        {
            WriteInputTensorFromImageRawData(original);
            return;
        }
    }
    
    void Image8::Flip(const FlipType flip)
    {
        if (flip == FlipType::Horizontality)
        {
            for(int cy = 0; cy < height_ / 2.0f; ++cy)
            {
                for(int cx = 0; cx < width_; ++cx)
                {
                    for(int cc = 0; cc < channel_; ++cc)
                    {
                        std::swap(At(cx, cy, cc), At(cx, height_ - cy, cc));
                    }
                }
            }
        }
        else if (flip == FlipType::Vertical)
        {
            for(int cy = 0; cy < height_; ++cy)
            {
                for(int cx = 0; cx < width_ / 2.0f; ++cx)
                {
                    for(int cc = 0; cc < channel_; ++cc)
                    {
                        std::swap(At(cx, cy, cc), At(width_ - cx, cy, cc));
                    }
                }
            }
        }
    }
    
    inline int Image8::GetWidth() const { return width_; }
    inline int Image8::GetHeight() const { return height_; }
    inline int Image8::GetChannel() const { return channel_; }
    inline const ChannelType& Image8::GetChannelType() const { return channelType_; }
    
    const ImagePixelType* Image8::data() const { return image_data_; }
    inline int Image8::stride() const { return stride_; }
    
    // Clears image to a single value.
    inline void Image8::Clear(const ImagePixelType& val) {
        memset(image_data_, val, sizeof(*image_data_) * data_size_);
    }

    inline ImagePixelType& Image8::At(int x, int y, int c)
    {
        // prevent out of range
        x = std::max(x, 0);
        x = std::min(x, width_less_one_);
        y = std::max(y, 0);
        y = std::min(y, height_less_one_);
        
        return image_data_[y * stride_ + x * channel_ + c];
    }

    inline const ImagePixelType& Image8::At(int x, int y, int c) const
    {
        // prevent out of range
        x = std::max(x, 0);
        x = std::min(x, width_less_one_);
        y = std::max(y, 0);
        y = std::min(y, height_less_one_);
        
        return image_data_[y * stride_ + x * channel_ + c];
    }

    inline bool Image8::IsSameSize(const Image8& other) const
    {
        return (GetWidth() == other.GetWidth()
                && GetHeight() == other.GetHeight()
                && GetChannel() == other.GetChannel());
    }
    
    std::unique_ptr<Image8> Image8::CopyMemory(
                                               const ImagePixelType* pixels,
                                               const int width,
                                               const int height,
                                               const int channel,
                                               const ChannelType channelType
                                               )
    {
        const int imageSize = width * height * channel;
        ImagePixelType* data = new ImagePixelType[imageSize * sizeof(ImagePixelType)];
        if (data == nullptr)
            return nullptr;
        
        memcpy(data, pixels, static_cast<size_t>(imageSize * sizeof(ImagePixelType)));
        std::unique_ptr<Image8> image = std::unique_ptr<Image8>(new Image8(
                                                                           width,
                                                                           height,
                                                                           channel,
                                                                           channelType,
                                                                           data,
                                                                           true)
                                                                );
        
        return image;
    }

    
    void Image8::WriteInputTensorFromImageRawData(const Image8& original){
        ImageSizeType image_width(original.GetWidth());
        ImageSizeType image_height(original.GetHeight());
        ImageSizeType image_channels(original.GetChannel());
        ImageSizeType wanted_width(width_);
        ImageSizeType wanted_height(height_);
        ImageSizeType wanted_channels(channel_);
        const ChannelType& channel_type(original.GetChannelType());
        
        const ImagePixelType* in = original.data();
        ImagePixelType* out = image_data_;

        if (channel_type == ChannelType::RGB){
            for (int y = 0; y < wanted_height; ++y) {
                const int in_y = (y * image_height) / wanted_height;
                const ImagePixelType* in_row = in + (in_y * image_width * image_channels);
                ImagePixelType* out_row = out + (y * wanted_width * wanted_channels);
                for (int x = 0; x < wanted_width; ++x) {
                    const int in_x = (x * image_width) / wanted_width;
                    const ImagePixelType* in_pixel = in_row + (in_x * image_channels);
                    ImagePixelType* out_pixel = out_row + (x * wanted_channels);
                    for (int c = 0; c < wanted_channels; ++c) {
                        out_pixel[c] = in_pixel[c];
                    }
                }
            }
        }
        else if (channel_type == ChannelType::BGR){
            for (int y = 0; y < wanted_height; ++y) {
                const int in_y = (y * image_height) / wanted_height;
                const ImagePixelType* in_row = in + (in_y * image_width * image_channels);
                ImagePixelType* out_row = out + (y * wanted_width * wanted_channels);
                for (int x = 0; x < wanted_width; ++x) {
                    const int in_x = (x * image_width) / wanted_width;
                    const ImagePixelType* in_pixel = in_row + (in_x * image_channels);
                    ImagePixelType* out_pixel = out_row + (x * wanted_channels);
                    for (int c = 0; c < wanted_channels; ++c) {
                        out_pixel[c] = in_pixel[wanted_channels - c - 1];
                    }
                }
            }
        }
        return;
    }
    
    inline void Image8::Allocate() {
        image_data_ = new ImagePixelType[data_size_];
        if (image_data_ == nullptr) {
            PrintLog("Couldn't allocate image data!");
        }
    }
    
#if defined(DEBUG_PRINT)
    std::ostream& operator<<(std::ostream& stream, const Image8& image){
        for (int y = 0; y < image.height_; ++y) {
            for (int x = 0; x < image.width_; ++x) {
                for (int z = 0; z < image.channel_; ++z) {
                    stream << image.At(x, y, z) << " ";
                }
                stream << " ";
            }
            stream << std::endl;
        }
        return stream;
    }
#endif // DEBUG_PRINT
} // ContAI
