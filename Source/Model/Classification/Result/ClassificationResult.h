
#ifndef _MODEL_CLASSIFICATION_RESULT_CLASSIFICATIONRESULT_H_
#define _MODEL_CLASSIFICATION_RESULT_CLASSIFICATIONRESULT_H_

#include <vector>
#include <string>
#include "Model/Common/Result/Result.h"
#include "Common/Common.h"

#if defined(DEBUG_PRINT)
#   include <sstream>
#endif // DEBUG_PRINT

namespace ContAI {

    class ClassificationResult : public Result
    {
    public:
        class Element
        {
        public:
            Element(float _score, const std::string& _labelName);
            
            Element(const Element& other);
            Element& operator = (const Element& other);
            // order score
            bool operator <(const Element& other) const;
            // order area
            // bool operator <(const Element& other) const
            bool operator >(const Element& other) const;

        public:
            float Score;
            std::string LabelName;
        };
        
        // if Element is too long vector or too heavy Element,
        // chage Container to priority_queue, not vector
        typedef std::vector<Element> Container;
        typedef Container::const_iterator ConstIterator;
        typedef Container::iterator Iterator;
        
    public:
        ClassificationResult(){}

    private:
        // Don't allow Copy Constructor
        ClassificationResult(const ClassificationResult& other) = delete;
        // Don't allow Copy Operator
        ClassificationResult& operator =(const ClassificationResult& other) = delete;

    public:
        void Add(const Element& _element);
        void Add(float _score, const std::string& _labelName);
        const Element& At(unsigned int index) const;
        const Container& ContainerClone() const;
        void Clean();
        void Sort();
        const Container& GetElementList();
        
    private:
        Container m_ElementList;
        
#if defined(DEBUG_PRINT)
    public:
        friend std::ostream& operator <<(std::ostream& stream, const ClassificationResult& other);
#endif // DEBUG_PRINT
    };

} // namespace ContAI

#endif // _MODEL_CLASSIFICATION_RESULT_CLASSIFICATIONRESULT_H_
