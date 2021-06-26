#pragma once

#include <fstream>
#include <filesystem>

namespace SCS
{
    class Stream
    {
    public:
        Stream(std::filesystem::path path);
        Stream();
        virtual ~Stream();

        virtual void open(std::filesystem::path path);
        virtual void close();
        std::filesystem::path path();
    protected:
        std::filesystem::path m_path;
        std::fstream m_file;

        template<typename T>
        void read(T &value)
        {
            read(reinterpret_cast<char*>(&value), sizeof(value));
        }
        void read(std::string &text);
        void read(char *data, std::streamsize size);
    };
}