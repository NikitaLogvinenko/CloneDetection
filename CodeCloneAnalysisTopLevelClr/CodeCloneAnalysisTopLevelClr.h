#pragma once
#include <vector>
#include <vcclr.h>
#include "funcs_clones_analysis_through_cm_app.h"

using namespace System;

namespace CLR 
{
    public ref class funcs_clones_analysis_through_cm_app_clr
    {
    public:
        static void Exec(int argc, array<String^>^ argv) 
        {
            pin_ptr<const wchar_t> wchars = nullptr;
            std::vector<const char*> argv_chars(argc);

            for (int i = 0; i < argc; ++i) 
            {
                wchars = PtrToStringChars(argv[i]);
                size_t length = wcslen(wchars);
                argv_chars[i] = new char[length + 1];
                size_t converted = 0;
                wcstombs_s(&converted, const_cast<char*>(argv_chars[i]), length + 1, wchars, _TRUNCATE);
            }

            try
            {
                code_clones_analysis_top_level::funcs_clones_analysis_through_cm_app::exec(argc, argv_chars.data());
            }
            catch (const std::exception& e) 
            {
                throw gcnew System::Exception(gcnew System::String(e.what()));
            }

            for (int i = 0; i < argc; ++i) 
            {
                delete[] argv_chars[i];
            }
        }
    };
}
