#pragma once

#include <cinttypes>
#include <string>
#include <vector>

#define ROOT_HASH 0x9AE16A3B2F90404F

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
                return "Unknown";
        }
    }

    class Entry
    {
    public:
        Entry();
        ~Entry();

        void set_hash(uint64_t hash);
        uint64_t get_hash();
        void set_offset(int32_t offset);
        int32_t get_offset();
        void set_unknown1(int32_t unknown1);
        int32_t get_unknown1();
        void set_type(EntryType type);
        EntryType get_type();
        void set_crc(uint32_t crc);
        uint32_t get_crc();
        void set_size(int32_t size);
        int32_t get_size();
        void set_compressed_size(int32_t compressed_size);
        int32_t get_compressed_size();
        std::vector<std::string> &get_names();
        bool is_file();
        bool is_directory();
        bool is_compressed();
        bool is_root_directory();
        void set_root_path(bool root_path);
        bool is_root_path();
        void set_locale_root_path(bool locale_root_path);
        bool is_locale_root_path();
        void set_path(std::string path);
        std::string get_path();
    private:
        uint64_t m_hash;
        int32_t m_offset;
        int32_t m_unknown1;
        EntryType m_type;
        uint32_t m_crc;
        int32_t m_size;
        int32_t m_compressed_size;
        std::vector<std::string> m_names;
        bool m_root_path;
        bool m_locale_root_path;
        std::string m_path;
    };
}