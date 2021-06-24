#ifndef _COMMON_IMAGE_IMAGE_H_
#define _COMMON_IMAGE_IMAGE_H_

#include "Common/Common.h"

#include "Common/Debug/DebugUtil.h"
#include "Common/Image/ImageShape.h"
#include "Tracking/GoogleObjectTracker/Common/geom.h"
#include <memory>

namespace ContAI {
    
    enum class FlipType
    {
        Horizontality = 0,
        Vertical
    };

    template <typename T>
    class Image
    {
    public:
        Image(const int width, const int height, const int channel)
        : width_less_one_(width - 1),
        height_less_one_(height - 1),
        data_size_(width * height * channel),
        own_data_(true),
        width_(width),
        height_(height),
        channel_(channel),
        stride_(width * channel) {
            Allocate();
        }
        explicit Image(const tf_tracking::Size& size, const int channel)
        : width_less_one_(size.width - 1),
        height_less_one_(size.height - 1),
        data_size_(size.width * size.height * channel),
        own_data_(true),
        width_(size.width),
        height_(size.height),
        channel_(channel),
        stride_(size.width * channel) {
            Allocate();
        }
        
        // Constructor that creates an image from preallocated data.
        // Note: The image takes ownership of the data lifecycle, unless own_data is
        // set to false.
        Image(const int width,
                     const int height,
                     const int channel,
                     T* const image_data,
                     const bool own_data) :
        width_less_one_(width - 1),
        height_less_one_(height - 1),
        data_size_(width * height * channel),
        own_data_(own_data),
        width_(width),
        height_(height),
        channel_(channel),
        stride_(width * channel) {
            image_data_ = image_data;
            SCHECK(image_data_ != NULL, "Can't create image with NULL data!");
        }
        
        ~Image() {
            if (own_data_) {
                delete[] image_data_;
            }
            image_data_ = NULL;
        }
        
        Image<T>* Crop(
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
            
            Image* const cropped_image = new Image(new_width, new_height, new_channel);
            
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
        void Resample(const Image<T>& original)
        {
            if(IsSameSize(original))
            {
                memcpy(image_data_, original.data(), data_size_);
                return;
            }
            
            const int _width = original.GetWidth();
            const int _height = original.GetHeight();
            
            double scaleWidth =  (double)width_ / (double)_width;
            double scaleHeight = (double)height_ / (double)_height;

            for(int cy = 0; cy < height_; ++cy)
            {
                for(int cx = 0; cx < width_; ++cx)
                {
//                    std::string output("Resample : ");
//                    char buf[256];
//                    sprintf(buf, "%d %d", cy, cx);
//                    output.append(buf);
//                    PrintLog(output);

                    int pixel = (cy * (width_ * channel_)) + (cx * channel_);
                    int nearestMatch =  (((int)(cy / scaleHeight) * (_width * channel_)) + ((int)(cx / scaleWidth) * channel_) );
                    
                    for(int cc = 0; cc < channel_; ++cc)
                    {
                        image_data_[pixel + cc] =  original.data()[nearestMatch + cc];
                    }
                }
            }

            return;
        }
        
        void Flip(const FlipType flip)
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
        
        inline int GetWidth() const { return width_; }
        inline int GetHeight() const { return height_; }
        inline int GetChannel() const { return channel_; }
        
        inline T* operator[](const int row) {
            return image_data_ + row * stride_;
        }
        
        inline const T* operator[](const int row) const {
            return image_data_ + row * stride_;
        }
        
        const T* data() const { return image_data_; }
        inline int stride() const { return stride_; }
        
        // Clears image to a single value.
        inline void Clear(const T& val) {
            memset(image_data_, val, sizeof(*image_data_) * data_size_);
        }
        inline T& At(int x, int y, int c)
        {
            // prevent out of range
            x = std::max(x, 0);
            x = std::min(x, width_less_one_);
            y = std::max(y, 0);
            y = std::min(y, height_less_one_);
            
            return image_data_[y * stride_ + x * channel_ + c];
        }
        inline const T& At(int x, int y, int c) const
        {
            // prevent out of range
            x = std::max(x, 0);
            x = std::min(x, width_less_one_);
            y = std::max(y, 0);
            y = std::min(y, height_less_one_);
            
            return image_data_[y * stride_ + x * channel_ + c];
        }
        inline bool IsSameSize(const Image<T>& other) const
        {
            return (GetWidth() == other.GetWidth()
                    && GetHeight() == other.GetHeight()
                    && GetChannel() == other.GetChannel());
        }
        
        static std::unique_ptr<Image<T>> CopyMemory(
                                          const T* pixels,
                                          const int width,
                                          const int height,
                                          const int channel
                                          )
        {
            const int imageSize = width * height * channel;
            T* data = new T[imageSize * sizeof(T)];
            if (data == nullptr)
                return nullptr;
            
            memcpy(data, pixels, static_cast<size_t>(imageSize * sizeof(T)));
            std::unique_ptr<Image<T>> image = std::unique_ptr<Image<T>>(new Image<T>(
                                                                            width,
                                                                            height,
                                                                            channel,
                                                                            data,
                                                                            true)
                                                                  );
            
            return image;
        }
        
        // Precompute these for efficiency's sake as they're used by a lot of
        // clipping code and loop code.
        // TODO(andrewharp): make these only accessible by other Images.
        const int width_less_one_;
        const int height_less_one_;
        
        // The raw size of the allocated data.
        const int data_size_;
        
    private:
        inline void Allocate() {
            image_data_ = new T[data_size_];
            if (image_data_ == nullptr) {
                PrintLog("Couldn't allocate image data!");
            }
        }
        
        T* image_data_;
        
        bool own_data_;
        
        const int width_;
        const int height_;
        const int channel_;
        
        // The image stride (offset to next row).
        // TODO(andrewharp): Make sure that stride is honored in all code.
        const int stride_;
        
#if defined(DEBUG_PRINT)
    public:
        friend std::ostream& operator<<(std::ostream& stream, const Image<T>& image);
#endif // DEBUG_PRINT
    };

#if defined(DEBUG_PRINT)
    template <typename T>
    inline std::ostream& operator<<(std::ostream& stream, const Image<T>& image)
    {
        for (int y = 0; y < image.GetHeight(); ++y) {
            for (int x = 0; x < image.GetWidth(); ++x) {
                for (int z = 0; z < image.GetChannel(); ++z) {
                    stream << image.At(x, y, z) << " ";
                }
                stream << " ";
            }
            stream << std::endl;
        }
        return stream;
    }
#endif // DEBUG_PRINT

    typedef Image<ImagePixelType> ImageRGB8;

} // ContAI

#endif // _COMMON_IMAGE_IMAGE_H_
