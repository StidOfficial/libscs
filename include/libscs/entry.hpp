#pragma once

#include <cinttypes>
#include <string>
#include <vector>
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

namespace SCS
{
    struct scs_entry_t
    {
        uint64_t hash;
        uint64_t offset;
        uint32_t type;
        uint32_t crc;
        uint32_t size;
        uint32_t compressed_size;
    };

    enum EntryType
    {
        UncompressedFile,
        UncompressedNames,
        CompressedFile,
        CompressedNames,
        UncompressedFileCopy,
        UncompressedNamesCopy,
        CompressedFileCopy,
        CompressedNamesCopy
    };

    inline std::string to_string(EntryType type)
    {
        switch(type)
        {
            case UncompressedFile:
                return "UncompressedFile";
            case UncompressedNames:
                return "UncompressedNames";
            case CompressedFile:
                return "CompressedFile";
            case CompressedNames:
                return "CompressedNames";
            case UncompressedFileCopy:
                return "UncompressedFileCopy";
            case UncompressedNamesCopy:
                return "UncompressedNamesCopy";
            case CompressedFileCopy:
                return "CompressedFileCopy";
            case CompressedNamesCopy:
                return "CompressedNamesCopy";
            default:
                return "Unknown (" + std::to_string(type) + ")";
        }
    }

    class Entry
    {
    public:
        Entry();
        Entry(scs_entry_t entry);
        ~Entry();

        void set_hash(uint64_t hash);
        uint64_t get_hash();
        void set_offset(uint64_t offset);
        uint64_t get_offset();
        void set_type(EntryType type);
        EntryType get_type();
        void set_crc(uint32_t crc);
        uint32_t get_crc();
        void set_size(uint32_t size);
        uint32_t get_size();
        void set_compressed_size(uint32_t compressed_size);
        uint32_t get_compressed_size();
        std::vector<std::string> &get_names();
        bool is_file();
        bool is_directory();
        bool is_compressed();
        bool is_root_directory();
    private:
        scs_entry_t m_entry;
        std::vector<std::string> m_names;
    };
}