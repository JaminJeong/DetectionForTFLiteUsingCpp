#ifndef _COMMON_UNCOPYABLE_H_
#define _COMMON_UNCOPYABLE_H_

namespace ContAI {

    class UnCopyable
    {
    protected:
        UnCopyable() {}
        ~UnCopyable() {}
    private:
        UnCopyable(const UnCopyable &other) = delete;
        UnCopyable& operator=(const UnCopyable &other) = delete;
    };

}

#endif // _COMMON_UNCOPYABLE_H_
