/***************************************************************************************************

Copyright (c) 2018 Intellectual Ventures Property Holdings, LLC (IVPH) All rights reserved.

EMOD is licensed under the Creative Commons Attribution-Noncommercial-ShareAlike 4.0 License.
To view a copy of this license, visit https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode

***************************************************************************************************/

#pragma once

#include <string>
#include <vector>
#include "CajunIncludes.h"

const std::vector<std::string> getSimTypeList();

void writeInputSchemas( const char* dll_path, const char* output_path, const char* exe_name );

namespace json
{
    class SchemaUpdater : public Visitor
    {
        public:
            SchemaUpdater(Object& idmt_schema)
                : m_idmt_schema(idmt_schema)
            { }

        private:
            virtual void Visit(Array& array);
            virtual void Visit(Object& object);
            virtual void Visit(Number& number);
            virtual void Visit(Uint64& number);
            virtual void Visit(String& string);
            virtual void Visit(Boolean& boolean);
            virtual void Visit(Null& null);

            Object& m_idmt_schema;
    };
}
