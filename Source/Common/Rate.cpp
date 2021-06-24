#include "Common/Ratio.h"

namespace ContAI {
    Ratio::Ratio(float other)
    :rate(other)
    {
        if (!(0 < rate && rate < 1))
        {
            rate = 0.1;
        }
    }
    
} // namespace ContAI
