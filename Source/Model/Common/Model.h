#ifndef _MODEL_COMMON_MODEL_H_
#define _MODEL_COMMON_MODEL_H_

#include <memory>
#include "Common/UnCopyable.h"
#include "Common/Common.h"

namespace ContAI {

    template <class InputType, class OutType>
    class Model : public UnCopyable
    {
    protected:
        virtual ~Model() {}
        virtual bool Init() = 0;
        virtual std::unique_ptr<OutType> Run(InputType& image_data) = 0;

    };
    
} // namespace ContAI

#endif // _MODEL_COMMON_MODEL_H_
