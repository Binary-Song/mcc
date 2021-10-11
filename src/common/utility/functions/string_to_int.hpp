#pragma once 
#include <string> 
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

    /// 字符串转int，支持二进制（0b）、八进制(0o)、十进制和十六进制(0x)。
    /// 字符串格式参考正则：[+-]?(0[box])?[0-9A-Fa-f]+
    /// @exception ConversionError 无法转换时报错
    int string_to_int(std::string str)
    {
        str = '^' + str + '\0';
        int sign = 1;
        int radix = 10;

        auto is_n0digit = [&radix](char x)
        {
            if (radix <= 10)
                return x >= '1' && x < '0' + radix;
            else
                return x >= '1' && x <= '9' || x >= 'a' && x <= 'f' || x >= 'A' && x <= 'F';
        };
        auto is_digit = [is_n0digit](char x) {return x == '0' || is_n0digit(x);};
        auto is_pm = [](char x) {return x == '+' || x == '-';};

        auto digit_conv = [](char x)
        {
            if (x <= '9')
                return x - '0';
            else if (x <= 'f')
                return x - 'a' + 10;
            else
                return x - 'A' + 10;
        };

        auto cur = str.begin();

    S1:
        cur++;
        if (is_n0digit(*cur)) {
            goto S3;
        } else if (is_pm(*cur)) {
            if (*cur == '-') {
                sign = -1;
            }
            goto S4;
        } else if (*cur == '0') {
            goto S2;
        } else {
            throw ConversionError(str);
        }
    S2:
        cur++;
        if (*cur == 'b') {
            radix = 2;
            goto S5;
        } else if (*cur == 'o') {
            radix = 8;
            goto S5;
        } else if (*cur == 'x') {
            radix = 16;
            goto S5;
        } else if (is_digit(*cur)) {
            goto S3;
        } else if (*cur == '\0') {
            goto ACC;
        } else {
            throw ConversionError(str);
        }
    S3:
        cur++;
        if (is_digit(*cur)) {
            goto S3;
        } else if (*cur == '\0') {
            goto ACC;
        } else {
            throw ConversionError(str);
        }
    S4:
        cur++;
        if (*cur == '0') {
            goto S2;
        } else if (is_n0digit(*cur)) {
            goto S3;
        } else {
            throw ConversionError(str);
        }
    S5:
        cur++;
        if (is_digit(*cur)) {
            goto S3;
        } else {
            throw ConversionError(str);
        }
    ACC:
        int ans = 0;
        int exp = 1; 
        for (int i = (int)str.size() - 2; i >= 0 && is_digit(str[i]);i--) {
            int digit = digit_conv(str[i]);
            ans += digit * exp;
            exp *= radix;
        }
        return sign * ans;
    }
}