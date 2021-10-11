#pragma once
#include "common/precompiled.hpp"
#include <fmt/color.h>
#include <cstdio>
namespace mcc {
    class ErrorVisualizer
    {
    public:
        ErrorVisualizer() = default;

        ErrorVisualizer(StringView code)
        {
            fit(code);
        }

        void fit(StringView code)
        {
            line_begins.clear();
            line_begins.push_back(0);
            int index = 0;
            for (auto&& c : code) {
                if (c == '\n') {
                    line_begins.push_back(index + 1);
                }
                index++;
            }
            line_begins.push_back(index);
        }

        void print_line(StringView code, int line)
        {
            int begin = line_begins[line - 1];
            int end = line_begins[line];

            String s(code.begin() + begin, code.begin() + end);
            fmt::print(stderr, fg(fmt::color::orange), "{}", s);
        }

        void print_line_emph(StringView code, int line, int column, int extend = 1)
        {
            int begin = line_begins[line - 1];
            int emp_begin = begin + column - 1;
            int emp_end = emp_begin + extend;
            int end = line_begins[line];

            String pt1 = get_substring_if_exists(code, begin, emp_begin);//| bg(fmt::color::slate_gray)
            String pt2 = get_substring_if_exists(code, emp_begin, emp_end);//| bg(fmt::color::slate_gray)
            String pt3 = get_substring_if_exists(code, emp_end, end);//| bg(fmt::color::slate_gray)

            fmt::print(stderr, fg(fmt::color::beige), "{}", pt1);
            fmt::print(stderr, fg(fmt::color::beige) | bg(fmt::color::dark_orange), "{}", pt2);
            fmt::print(stderr, fg(fmt::color::beige), "{}", pt3);
        }
        void print_error(StringView error, StringView file, StringView code, int line)
        {
            print_error(error, file, code, line, 1, 0);
        }
        void print_error_location(StringView file, int line = 0)
        {
            if (line == 0) {
                fmt::print(stderr, "{}: ", file, line);
            } else {
                fmt::print(stderr, "{}({}): ", file, line);
            }
        }
        void print_error(StringView error, StringView file, StringView code, int line, int column, int extend = 1)
        {
            print_error_location(file, line);
            fmt::print(stderr, fg(fmt::color::red), "Error");
            if (error.size() > 0 && error.back() == '.') {
                fmt::print(stderr, ": {}\n", error);
            } else {
                fmt::print(stderr, ": {}.\n", error);
            }
            if (line != 0) {
                print_error_location(file, line);
                fmt::print(stderr, fg(fmt::color::beige), "See:\n");
                print_error_location(file, line);
                fmt::print(stderr, fg(fmt::color::gray), "    {}| ", line);
                try {
                    print_line_emph(code, line, column, extend);
                }
                catch (...) {
                }
                fmt::print(stderr, "\n");
            }
        }

        void print_error(StringView error, StringView file)
        {
            print_error(error, file, "", 0);
        }

    private:
        Vector<int> line_begins;

        String get_substring_if_exists(StringView str, int begin, int end)
        {
            if (begin >= 0 && begin < str.size() && end >= 0 && end <= str.size()) {
                return String(str.begin() + begin, str.begin() + end);
            }
            return String();
        }
    };
} // namespace mcc
