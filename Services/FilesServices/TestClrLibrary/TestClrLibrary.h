#pragma once
#include "TestClrLibrary.h"
#include "../TestLib/TestLib.h"

using namespace System;

namespace CLR
{
	template<class T>
	public ref class WrapperClass
	{
	protected:
		T* obj;
		WrapperClass(T* instance): obj(instance)
		{
			
		}

        virtual ~WrapperClass()
        {
            if (obj != nullptr)
            {
                delete obj;
            }
        }
        !WrapperClass()
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

    inline int IncrementWr(int a)
    {
        return Increment(a);
    }

    public ref class AlreadyCSharp: WrapperClass<TestClass>
    {
    public:
	    AlreadyCSharp(int a) : WrapperClass(new TestClass(a))
        {}

        int GetA()
	    {
            return obj->GetA();
	    }

	    Text::StringBuilder^ GetName()
	    {
		    return gcnew Text::StringBuilder(gcnew String(obj->GetName().c_str()));
	    }
    };
}
