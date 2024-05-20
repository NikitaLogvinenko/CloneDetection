#pragma once
#include <vector>
#include <vcclr.h>
#include "funcs_clones_analysis_through_cm_app.h"
#include <msclr/marshal.h> 

using namespace System;
using namespace System::Runtime::InteropServices;
using namespace msclr::interop;

namespace CLR 
{
    public ref class funcs_clones_analysis_through_cm_app_clr
    {
    public:
        static void Exec(int argc, array<String^>^ argv) 
        {
            std::vector<const char*> nativeArgs;
            marshal_context context;

            for each (String ^ arg in argv)
            {
                const char* nativeArg = context.marshal_as<const char*>(arg);
                nativeArgs.push_back(nativeArg);
            }

            const char* const* argv_ = nativeArgs.data();

            try
            {
                code_clones_analysis_top_level::funcs_clones_analysis_through_cm_app::exec(argc, argv_);
            }
            catch (const std::exception& e) 
            {
                throw gcnew System::Exception(gcnew System::String(e.what()));
            }
        }
    };
}
