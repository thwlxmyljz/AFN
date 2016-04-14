#ifndef TSINGLETON_H
#define TSINGLETON_H
#include <memory>
using namespace std;

#define DEFINE_SINGLETON(cls) \
    protected:\
        static std::auto_ptr<cls> m_pInstance;\
    protected:\
        cls(){}\
    public:\
        virtual ~cls(){}\
    static cls* Instance(){\
        if(!m_pInstance.get()){\
            m_pInstance = std::auto_ptr<cls>(new cls());\
            cls* p = m_pInstance.get(); \
			p->Init(); \
        }\
        return m_pInstance.get();\
    }
#define IMPLEMENT_SINGLETON(cls) \
std::auto_ptr<cls> cls::m_pInstance(NULL);
#endif
