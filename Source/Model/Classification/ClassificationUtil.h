
#ifndef _MODEL_CLASSIFICATION_CLASSIFICATIONUTIL_H_
#define _MODEL_CLASSIFICATION_CLASSIFICATIONUTIL_H_

#include <vector>
#include <cstdint>

namespace ContAI {

    class ClassificationUtil
    {
    public:
        static void GetTopN(
                         const float* prediction,
                         const int prediction_size,
                         const int num_results,
                         const float threshold,
                         std::vector<std::pair<float, int> >* top_results);
        static void GetTopN(
                         const uint8_t* prediction,
                         const int prediction_size,
                         const int num_results,
                         const float threshold,
                         std::vector<std::pair<float, int> >* top_results);
    };
} // namespace ContAI
#endif // _MODEL_CLASSIFICATION_CLASSIFICATIONUTIL_H_
