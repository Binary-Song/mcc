#pragma once 
#include "common/precompiled.hpp"

namespace mcc {
    struct TranslationError
    {
        String _what;
        int line = 0;
        int column = 0;
        int extend = 1;

        String what()
        {
            return _what;
        }

        TranslationError(StringView what = "Unknown Error", int line = 0, int column = 0, int extend = 1)
        {
            this->_what = what;
            this->line = line;
            this->column = column;
            this->extend = 1;
        }
    };
}