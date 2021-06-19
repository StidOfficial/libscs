#pragma once

#include <filesystem>
#include <fstream>

#define MAGIC_MARK "SiiNunit"

namespace SCS
{
    class SIIFile
    {
    public:
        SIIFile(std::filesystem::path file_path);
        SIIFile();

        void open(std::filesystem::path file_path);
    private:
        std::fstream m_file;

        void parse();
    };
}