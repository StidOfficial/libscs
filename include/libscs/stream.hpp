#pragma once

#include <fstream>
#include <filesystem>

namespace SCS
{
    class Stream
    {
    public:
        Stream(std::filesystem::path file_path);
        Stream();
        virtual ~Stream();

        virtual void open(std::filesystem::path path);
        virtual void close();
        std::filesystem::path path();
    protected:
        std::filesystem::path m_path;
        std::fstream m_file;

        void read(uint16_t &value);
        void read(uint32_t &value);
        void read(uint64_t &value);
        void read(int32_t &value);
        void read(int64_t &value);
        void read(std::string &text);
        void read(char *data, std::streamsize size);
    };
}