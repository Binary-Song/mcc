#pragma once 
#include <string>
#include <string_view> 
namespace mcc_utilites {

    struct ConversionError
    {
        std::string target;

        ConversionError(std::string const& target = "")
        {
            this->target = target;
        }

        std::string what() const
        {
            return "Cannot convert `" + target + "` to string.";
        }
    };

    inline bool begins_with(std::string_view str, std::string_view prefix)
    {
        if (str.size() >= prefix.size()) {
            return str.substr(0, prefix.size()) == prefix;
        }
        return false;
    }

    /// 字符串转int，支持二进制（0b）、八进制(0o)、十进制和十六进制(0x)。
    /// 字符串格式参考正则：[+-]?(0[box])?[0-9A-Fa-f]+
    /// @exception ConversionError 无法转换时报错
    inline int string_to_int(std::string_view str)
    {
        int sign = 1;
        if (begins_with(str, "-")) {
            sign = -1;
            str.remove_prefix(1);
        } else if (begins_with(str, "+")) {
            str.remove_prefix(1);
        }

        int base = 10;
        if (begins_with(str, "0b")) {
            base = 2;
            str.remove_prefix(2);
        } else if (begins_with(str, "0x")) {
            base = 16;
            str.remove_prefix(2);
        } else if (begins_with(str, "0o")) {
            base = 8;
            str.remove_prefix(2);
        }

        int result = 0;
        if (base == 2 || base == 10 || base == 8) {
            for (char c : str) {
                int off = c - '0';
                result *= base;
                result += off;
                if (off >= base || off < 0) {
                    throw ConversionError(std::string(str));
                }
            }
        }   // base == 16
        else {
            for (char c : str) { 
                int off;
                if (c >= '0' && c <= '9') {
                    off = c - '0';
                } else if (c >= 'a' && c <= 'f') {
                    off = c - 'a' + 10;
                } else if (c >= 'A' && c <= 'F') {
                    off = c - 'A' + 10;
                } else {
                    throw ConversionError(std::string(str));
                }
                result *= base;
                result += off;
            }
        }
        return sign * result;
    }
}