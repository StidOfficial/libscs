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
#include "stream.hpp"

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

    struct scs_header_t
    {
        uint32_t magic;
        uint16_t version;
        uint16_t salt;
        uint32_t hash_method;
        uint32_t num_entries;
        uint32_t start_offset;
    };

    class SCSFile : public Stream, public std::vector<std::shared_ptr<Entry>>
    {
    public:
        static inline std::string locale_root_path = "locale";
        static inline const uint64_t locale_root_path_hash = CityHash64(locale_root_path.c_str(), locale_root_path.length());
        static inline std::string root_path = "";
        static inline const uint64_t root_path_hash = CityHash64(root_path.c_str(), root_path.length());

        SCSFile(std::filesystem::path file_path);
        SCSFile();
        virtual ~SCSFile();

        std::string path();
        void set_hash_method(HashMethod hash_method);
        HashMethod get_hash_method();
        Entry *find(uint64_t hash);
        Entry *find(std::string path);
        Entry *get_root();
        Entry *get_locale();

        void unpack();
        void pack();
    private:
        scs_header_t m_header;

        void read(scs_header_t &value);
        void read(scs_entry_t &value);
        void read(Entry* &entry);
    };
}