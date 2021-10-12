#ifndef DYNAMICLIB_H
#define DYNAMICLIB_H
#include <dlfcn.h>
#include <QDir>
#define GetProcAddr(h,x)    (x)dlsym(h,#x)

class DynamicLib
{
public:
    void *pHandle = nullptr;
public:
    DynamicLib()
    {}
    virtual ~DynamicLib()
    {
        if (pHandle)
        {
            dlclose(pHandle);
            pHandle = nullptr;
        }
    }
    bool operator()()
    {
        return pHandle != nullptr;
    }
};

#endif // DYNAMICLIB_H
