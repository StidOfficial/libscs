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

    inline std::string to_string(HashMethod hash_method)
    {
        switch(hash_method)
        {
            case CityHash:
                return "CityHash";
            default:
                return "Unknown (" + std::to_string(hash_method) + ")";
        }
    }

    class SCSFile : public std::vector<std::shared_ptr<Entry>>
    {
    public:
        static inline std::string locale_root_path = "locale";
        static inline const uint64_t locale_root_path_hash = CityHash64(locale_root_path.c_str(), locale_root_path.length());
        static inline std::string root_path = "";
        static inline const uint64_t root_path_hash = CityHash64(root_path.c_str(), root_path.length());

        SCSFile(std::filesystem::path file_path);
        SCSFile();
        ~SCSFile();

        void set_hash_method(HashMethod hash_method);
        HashMethod get_hash_method();
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