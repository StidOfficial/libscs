#include "entry.hpp"

#include "scs.hpp"

namespace SCS
{
    Entry::Entry()
    {

    }

    void Entry::set_hash(uint64_t hash)
    {
        m_hash = hash;
    }

    uint64_t Entry::get_hash()
    {
        return m_hash;
    }

    void Entry::set_offset(uint64_t offset)
    {
        m_offset = offset;
    }

    uint64_t Entry::get_offset()
    {
        return m_offset;
    }

    void Entry::set_type(EntryType type)
    {
        m_type = type;
    }
    
    EntryType Entry::get_type()
    {
        return m_type;
    }
    
    void Entry::set_crc(uint32_t crc)
    {
        m_crc = crc;
    }
    
    uint32_t Entry::get_crc()
    {
        return m_crc;
    }
    
    void Entry::set_size(uint32_t size)
    {
        m_size = size;
    }
    
    uint32_t Entry::get_size()
    {
        return m_size;
    }
    
    void Entry::set_compressed_size(uint32_t compressed_size)
    {
        m_compressed_size = compressed_size;
    }
    
    uint32_t Entry::get_compressed_size()
    {
        return m_compressed_size;
    }

    std::vector<std::string> &Entry::get_names()
    {
        return m_names;
    }

    bool Entry::is_file()
    {
        switch(m_type)
        {
            case EntryType::UncompressedFile:
            case EntryType::CompressedFile:
            case EntryType::UncompressedFileCopy:
            case EntryType::CompressedFileCopy:
                return true;
            default:
                return false;
        }
    }

    bool Entry::is_directory()
    {
        return !is_file();
    }

    bool Entry::is_compressed()
    {
        switch(m_type)
        {
            case EntryType::CompressedFile:
            case EntryType::CompressedFileCopy:
            case EntryType::CompressedNames:
            case EntryType::CompressedNamesCopy:
                return true;
            default:
                return false;
        }
    }

    bool Entry::is_root_directory()
    {
        return get_hash() == SCSFile::root_path_hash;
    }

    void Entry::set_root_path(bool root_path)
    {
        m_root_path = root_path;
    }

    bool Entry::is_root_path()
    {
        return m_root_path;
    }

    void Entry::set_locale_root_path(bool locale_root_path)
    {
        m_locale_root_path = locale_root_path;
    }

    bool Entry::is_locale_root_path()
    {
        return m_locale_root_path;
    }

    void Entry::set_path(std::string path)
    {
        m_path = path;
    }

    std::string Entry::get_path()
    {
        return m_path;
    }

    Entry::~Entry()
    {
        
    }
}