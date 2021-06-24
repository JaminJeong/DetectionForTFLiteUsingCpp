#ifndef _COMMON_PARAMETER_SINGLETON_H_
#define _COMMON_PARAMETER_SINGLETON_H_

#include <mutex>

namespace ContAI {

static std::mutex getInstanceMutex;

    template<typename T>
    class Singleton
    {
    public:
        static T* GetInstance();
        static void destroy();
        
    private:
        
        Singleton(Singleton const&){};
        Singleton& operator=(Singleton const&){};
        
    protected:
        static T* m_instance;
        
        Singleton(){ m_instance = static_cast <T*> (this); };
        ~Singleton()
        {
            destroy();
        };
    };

    template<typename T>
    T* Singleton<T>::m_instance = 0;

    template<typename T>
    T* Singleton<T>::GetInstance()
    {
        std::lock_guard<std::mutex> lock(getInstanceMutex);
        if(!m_instance)
        {
            Singleton<T>::m_instance = new T();
        }
        
        return m_instance;
    }

    template<typename T>
    void Singleton<T>::destroy()
    {
        delete Singleton<T>::m_instance;
        Singleton<T>::m_instance = 0;
    }

} // namespace ContAI

#endif // _COMMON_PARAMETER_SINGLETON_H_
