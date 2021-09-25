#pragma once
#include <string>
#include <string_view>
#include <filesystem>
namespace mcc_utilites {

    struct FileNotOpenedException
    {
        std::string path;

        IOException(std::string path)
        {
            this->path = path;
        }

        std::string Message()
        {
            return "Cannot open file at " + path + ".";
        }
    };

    std::string read_file_as_string(std::filesystem::path path)
    {
        std::ifstream stream(path);
        if (stream.is_open()) {
            return std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        }
        throw FileNotOpenedException(path);
    }
}