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
        void set_root_path(bool root_path);
        bool is_root_path();
        void set_locale_root_path(bool locale_root_path);
        bool is_locale_root_path();
        void set_path(std::filesystem::path path);
        std::filesystem::path get_path();
    private:
        uint64_t m_hash;
        uint64_t m_offset;
        EntryType m_type;
        uint32_t m_crc;
        uint32_t m_size;
        uint32_t m_compressed_size;
        std::vector<std::string> m_names;
        bool m_root_path;
        bool m_locale_root_path;
        std::filesystem::path m_path;
    };
}