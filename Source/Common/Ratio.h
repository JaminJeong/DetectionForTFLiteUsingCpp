#ifndef _COMMON_COMMON_RATIO_H_
#define _COMMON_COMMON_RATIO_H_

namespace ContAI {
    // 0 ~ 1
    class Ratio
    {
    public:
        explicit Ratio(float other);
        
    private:
        float rate;
        
    public:
        inline float GetValue()
        {return rate;}
    };

    typedef Ratio Accuracy;
} // namespace ContAI

#endif // _COMMON_COMMON_RATIO_H_
