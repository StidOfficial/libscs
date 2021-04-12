#pragma once

#include <vector>
#include <memory>
#include <fstream>
#if __has_include(<filesystem>)
#include <filesystem>
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace std {
	namespace filesystem = std::experimental::filesystem;
}
#else
#error "No filesystem header found !"
#endif
#include <city.h>

#include "entry.hpp"

#define MAGIC       0x23534353 // SCS#
#define VERSION     0x0001

namespace SCS
{
    enum HashMethod
    {
        CityHash = 0x59544943 // CITY
    };

    class SCSFile : public std::vector<std::shared_ptr<Entry>>
    {
    public:
        SCSFile(std::filesystem::path file_path);
        SCSFile();
        ~SCSFile();

        Entry *find(uint64_t hash);
        Entry *find(std::string path);
        Entry *get_root();
        Entry *get_locale_root();

        void unpack();
        void pack();
    private:
        std::filesystem::path m_path;
        std::fstream m_file;

        uint16_t m_version;
        uint16_t m_salt;
        uint32_t m_hash_method;
        uint32_t m_num_entries;
        uint32_t m_start_offset;

        uint64_t m_locale_root_path_hash;
        uint64_t m_root_path_hash;

        void open(std::filesystem::path file_path);
        void read(uint16_t &value);
        void read(uint32_t &value);
        void read(uint64_t &value);
        void read(int32_t &value);
        void read(int64_t &value);
        void read(Entry* &entry);
        void read(std::string &text);
        void read(char *data, std::streamsize size);

        void populateDirectory(Entry *entry);
    };
}