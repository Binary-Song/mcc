#pragma once
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <fmt/core.h>
namespace mcc_utilites {



    struct IOError
    {
        std::string path;
        std::string why_str;
        enum class Type
        {
            Open,
            Read,
            Write,
            NotOpened,
            Unknown,
        } why;

        IOError(std::string path = "", IOError::Type why = IOError::Type::Unknown)
        {
            this->path = path;
            switch (why) {
            case IOError::Type::Open:
                why_str = "opening";
                break;
            case IOError::Type::Read:
                why_str = "reading";
                break;
            case IOError::Type::Write:
                why_str = "writing";
                break;
            case IOError::Type::Unknown:
                why_str = "doing IO operation on";
                break;
            }
        }

        std::string what() const
        {
            return "Error occurred when " + why_str + " file " + path;
        }
    };

    inline std::string file_to_string(std::filesystem::path path)
    {
        std::ifstream stream;
        stream.exceptions(stream.failbit | stream.badbit);
        try {
            stream.open(path);
            return std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        }
        catch (std::ifstream::failure&) {
            throw IOError(path.string(), IOError::Type::Read);
        }
        return "";
    }

   inline std::string file_to_string_with_fallback(std::filesystem::path path, std::string fallback) noexcept
    {
        try {
            return file_to_string(path);
        }
        catch (...) {
            return fallback;
        }
        return fallback;
    }

   inline void overwrite_file(std::string str, std::filesystem::path path)
    {
        std::ofstream stream;
        stream.exceptions(stream.failbit | stream.badbit);
        try {
            stream.open(path);
            stream << str;
        }
        catch (std::ifstream::failure& e) {
            throw IOError(path.string(), IOError::Type::Write);
        }
    }

    class OutputFile
    {
        FILE* file = nullptr;
        std::string path;

    public:
        OutputFile()
        {}

        ~OutputFile()
        {
            close();
        }

        OutputFile(OutputFile const&) = delete;
        OutputFile(OutputFile&& other)
        {
            *this = std::move(other);
        }
        OutputFile& operator= (OutputFile&& other)
        {
            this->file = other.file;
            this->path = other.path;
            other.file = nullptr;
            other.path = "";
            return *this;
        }
        OutputFile& operator= (OutputFile const&) = delete;

        OutputFile(std::string_view path)
        {
            open(path);
        }

        void open(std::string_view path, const char* mode = "wb")
        {
            close();
            this->path = path;
            file = fopen(std::string(path).c_str(), "wb");
            if (!file) {
                throw IOError(std::string(path), IOError::Type::Open);
            }
        }

        void close()
        {
            if (file) {
                fclose(file);
                file = nullptr;
            }
        }

        void write(char data)
        {
            if (file) {
                auto written = fwrite(&data, 1, 1, file);
                if (written != 1) {
                    throw IOError(path, IOError::Type::Write);
                }
            } else {
                throw IOError(path, IOError::Type::NotOpened);
            }
        }

        void write(unsigned char data)
        {
            write(std::vector<unsigned char>{data});
            /*
            if (file) {
                auto written = fwrite(&data, 1, 1, file);
                if (written != 1) {
                    throw IOError(path, IOError::Type::Write);
                }
            } else {
                throw IOError(path, IOError::Type::Write);
            }*/
        }

        void write(std::vector<unsigned char> const& data)
        {
            if (file) {
                auto written = fwrite(data.data(), 1, data.size(), file);
                if (written != data.size()) {
                    throw IOError(path, IOError::Type::Write);
                }
            } else {
                throw IOError(path, IOError::Type::Write);
            }
        }

        void write(std::vector<char> const& data)
        {
            if (file) {
                auto written = fwrite(data.data(), 1, data.size(), file);
                if (written != data.size()) {
                    throw IOError(path, IOError::Type::Write);
                }
            } else {
                throw IOError(path, IOError::Type::Write);
            }
        }

        template<class... T>
        void print(std::string_view fmt, T&&...  args)
        {
            if (file) {
                fmt::print(file, fmt, args...);
            } else {
                throw IOError(path, IOError::Type::Write);
            }
        }
    };
}