#include "scs.hpp"

#include <cstring>
#include <cerrno>
#include <zlib.h>

#include <iostream>

namespace SCS
{
    SCSFile::SCSFile(std::filesystem::path file_path)
        : SCSFile()
    {
        open(file_path);
    }

    SCSFile::SCSFile()
        : Stream(), std::vector<std::shared_ptr<Entry>>()
    {
    }

    void SCSFile::open(std::filesystem::path path)
    {
        Stream::open(path);

        Stream::read(m_header);

        if(m_header.magic != MAGIC)
            throw std::logic_error("Invalid magic code");

        if(m_header.version != VERSION)
            throw std::logic_error("Invalid version");

        if(m_header.salt != 0x0000)
            throw std::logic_error("Invalid salt");

        if(m_header.hash_method != HashMethod::CityHash)
            throw std::logic_error("Invalid hash method");

        m_file.seekg(m_header.start_offset, m_file.beg);

        uLongf buffer_size;

        Entry *entry = nullptr;
        for(uint32_t i = 0; i < m_header.num_entries; i++)
        {
            read(entry);

            if(entry->is_directory())
            {
                std::streampos old_pos = m_file.tellg();
                m_file.seekg(entry->get_offset(), m_file.beg);

                std::string names(entry->get_size(), '\0');
                if(entry->is_compressed())
                {
                    std::vector<char> compress_buffer(entry->get_compressed_size());
                    m_file.read(compress_buffer.data(), entry->get_compressed_size());

                    std::vector<char> buffer(entry->get_size());
                    buffer_size = buffer.size();
                    uncompress(reinterpret_cast<unsigned char*>(buffer.data()), &buffer_size, reinterpret_cast<unsigned char*>(compress_buffer.data()), entry->get_compressed_size());

                    names.assign(buffer.data(), buffer.size());
                }
                else
                    m_file.read(&names[0], names.length());

                m_file.seekg(old_pos, m_file.beg);

                // List all names
                std::string name;
                std::istringstream namesStream(names);
                while(std::getline(namesStream, name, '\n'))
                    entry->get_names().emplace_back(name);
            }

            emplace_back(entry);
        }
    }

    void SCSFile::read(Entry* &entry)
    {
        scs_entry_t file_entry;
        Stream::read(file_entry);

        entry = new Entry(file_entry);
    }

    std::string SCSFile::path()
    {
        return m_path;
    }

    void SCSFile::set_hash_method(HashMethod hash_method)
    {
        m_header.hash_method = hash_method;
    }

    HashMethod SCSFile::get_hash_method()
    {
        return static_cast<HashMethod>(m_header.hash_method);
    }

    Entry *SCSFile::find(uint64_t hash)
    {
        for(auto &entry : *this)
            if(entry->get_hash() == hash)
                return entry.get();

        return nullptr;
    }

    Entry *SCSFile::find(std::string path)
    {
        return find(CityHash64(path.c_str(), path.size()));
    }

    Entry *SCSFile::get_root()
    {
        return find(root_path);
    }

    Entry * SCSFile::get_locale()
    {
        return find(locale_root_path);
    }

    SCSFile::~SCSFile()
    {
    }
}