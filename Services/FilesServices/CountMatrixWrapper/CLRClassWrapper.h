#pragma once

template<class T>
public ref class CLRClassWrapper
{
protected:
    T* obj;
    CLRClassWrapper(T* instance) : obj(instance)
    {

    }

    virtual ~CLRClassWrapper()
    {
        if (obj != nullptr)
        {
            delete obj;
        }
    }
    !CLRClassWrapper()
    {
        if (obj != nullptr)
        {
            delete obj;
        }
    }
    T* GetInstance()
    {
        return obj;
    }
};
