#include "entry.hpp"

#include "scs.hpp"

namespace SCS
{
    Entry::Entry()
    {

    }

    Entry::Entry(scs_entry_t entry)
     : m_entry(entry)
    {
    }

    void Entry::set_hash(uint64_t hash)
    {
        m_entry.hash = hash;
    }

    uint64_t Entry::get_hash()
    {
        return m_entry.hash;
    }

    void Entry::set_offset(uint64_t offset)
    {
        m_entry.offset = offset;
    }

    uint64_t Entry::get_offset()
    {
        return m_entry.offset;
    }

    void Entry::set_type(EntryType type)
    {
        m_entry.type = type;
    }
    
    EntryType Entry::get_type()
    {
        return static_cast<EntryType>(m_entry.type);
    }
    
    void Entry::set_crc(uint32_t crc)
    {
        m_entry.crc = crc;
    }
    
    uint32_t Entry::get_crc()
    {
        return m_entry.crc;
    }
    
    void Entry::set_size(uint32_t size)
    {
        m_entry.size = size;
    }
    
    uint32_t Entry::get_size()
    {
        return m_entry.size;
    }
    
    void Entry::set_compressed_size(uint32_t compressed_size)
    {
        m_entry.compressed_size = compressed_size;
    }
    
    uint32_t Entry::get_compressed_size()
    {
        return m_entry.compressed_size;
    }

    std::vector<std::string> &Entry::get_names()
    {
        return m_names;
    }

    bool Entry::is_file()
    {
        switch(get_type())
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
        switch(get_type())
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

    Entry::~Entry()
    {
        
    }
}