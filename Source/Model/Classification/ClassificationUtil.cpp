#include "Model/Classification/ClassificationUtil.h"

#include <memory>
#include <queue>
#include <algorithm>

namespace ContAI {

    // Returns the top N confidence values over threshold in the provided vector,
    // sorted by confidence in descending order.
    void ClassificationUtil::GetTopN(
                         const float* prediction,
                         const int prediction_size,
                         const int num_results,
                         const float threshold,
                         std::vector<std::pair<float, int> >* top_results)
    {
        // Will contain top N results in ascending order.
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int> >,
        std::greater<std::pair<float, int> > >
        top_result_pq;
        
        const long count = prediction_size;
        for (int i = 0; i < count; ++i) {
            const float value = prediction[i];
            
            // Only add it if it beats the threshold and has a chance at being in
            // the top N.
            if (value < threshold) {
                continue;
            }
            
            top_result_pq.push(std::pair<float, int>(value, i));
            
            // If at capacity, kick the smallest value out.
            if (top_result_pq.size() > num_results) {
                top_result_pq.pop();
            }
        }
        
        // Copy to output vector and reverse into descending order.
        while (!top_result_pq.empty()) {
            top_results->push_back(top_result_pq.top());
            top_result_pq.pop();
        }
        std::reverse(top_results->begin(), top_results->end());
    }
    
    
    // Returns the top N confidence values over threshold in the provided vector,
    // sorted by confidence in descending order.
    void ClassificationUtil::GetTopN(
                         const uint8_t* prediction,
                         const int prediction_size,
                         const int num_results,
                         const float threshold,
                         std::vector<std::pair<float, int> >* top_results)
    {
        // Will contain top N results in ascending order.
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int> >,
        std::greater<std::pair<float, int> > >
        top_result_pq;
        
        const long count = prediction_size;
        for (int i = 0; i < count; ++i) {
            const float value = prediction[i] / 255.f;
            
            // Only add it if it beats the threshold and has a chance at being in
            // the top N.
            if (value < threshold) {
                continue;
            }
            
            top_result_pq.push(std::pair<float, int>(value, i));
            
            // If at capacity, kick the smallest value out.
            if (top_result_pq.size() > num_results) {
                top_result_pq.pop();
            }
        }
        
        // Copy to output vector and reverse into descending order.
        while (!top_result_pq.empty()) {
            top_results->push_back(top_result_pq.top());
            top_result_pq.pop();
        }
        std::reverse(top_results->begin(), top_results->end());
    }

} // namespace ContAI
