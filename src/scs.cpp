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
        : std::vector<std::shared_ptr<Entry>>(), m_file()
    {
        std::string locale = "locale";
        std::string root = "";

        m_locale_root_path_hash = CityHash64(locale.c_str(), locale.size());
        m_root_path_hash = CityHash64(root.c_str(), root.size());
    }

    void SCSFile::open(std::filesystem::path file_path)
    {
        if(std::filesystem::is_directory(m_path))
			throw std::logic_error(std::strerror(EISDIR));

        m_path = file_path;
    }

    void SCSFile::read(uint16_t &value)
	{
		read(reinterpret_cast<char*>(&value), sizeof(value));
	}

    void SCSFile::read(uint32_t &value)
	{
		read(reinterpret_cast<char*>(&value), sizeof(value));
	}

    void SCSFile::read(uint64_t &value)
    {
        read(reinterpret_cast<char*>(&value), sizeof(value));
    }

    void SCSFile::read(int32_t &value)
    {
        read(reinterpret_cast<char*>(&value), sizeof(value));
    }

    void SCSFile::read(int64_t &value)
    {
        read(reinterpret_cast<char*>(&value), sizeof(value));
    }

    void SCSFile::read(std::string &text)
    {
        std::getline(m_file, text, '\0');
    }

    void SCSFile::read(char *data, std::streamsize size)
	{
		m_file.read(data, size);
	}

    void SCSFile::read(Entry* &entry)
    {
        uint64_t hash;
        int32_t offset;
        int32_t unknown1;
        int32_t type;
        uint32_t crc;
        int32_t size;
        int32_t compressed_size;

        read(hash);
        read(offset);
        read(unknown1);
        read(type);
        read(crc);
        read(size);
        read(compressed_size);

        entry = new Entry();

        entry->set_hash(hash);
        entry->set_offset(offset);
        entry->set_unknown1(unknown1);
        entry->set_type(static_cast<EntryType>(type));
        entry->set_crc(crc);
        entry->set_size(size);
        entry->set_compressed_size(compressed_size);
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
        return find(m_root_path_hash);
    }

    Entry *SCSFile::get_locale_root()
    {
        return find(m_locale_root_path_hash);
    }

    void SCSFile::unpack()
    {
        m_file.open(m_path, std::fstream::in | std::fstream::binary);
		if(!m_file.is_open())
			throw std::logic_error(std::strerror(errno));

        uint32_t magic;
        read(magic);

        if(magic != MAGIC)
            throw std::logic_error("Invalid magic code");

        read(m_version);

        if(m_version != VERSION)
            throw std::logic_error("Invalid version");

        read(m_salt);

        if(m_salt != 0x0000)
            throw std::logic_error("Invalid salt");

        read(m_hash_method);

        if(m_hash_method != HASH_METHOD)
            throw std::logic_error("Invalid hash method");

        read(m_num_entries);
        read(m_start_offset);

        m_file.seekg(m_start_offset, m_file.beg);

        char* compressBuffer;
        Bytef* buffer;
        uLongf buffer_size;

        Entry *entry = nullptr;
        for(uint32_t i = 0; i < m_num_entries; i++)
        {
            read(entry);

            if(entry->is_directory())
            {
                std::streampos old_pos = m_file.tellg();
                m_file.seekg(entry->get_offset(), m_file.beg);

                std::string names(entry->get_size(), '\0');
                if(entry->is_compressed())
                {
                    compressBuffer = new char[entry->get_compressed_size()];
                    m_file.read(compressBuffer, entry->get_compressed_size());

                    buffer = new Bytef[entry->get_size()];
                    uLongf buffer_size = entry->get_size();
                    uncompress(buffer, &buffer_size, (unsigned char*)compressBuffer, entry->get_compressed_size());

                    names.assign(reinterpret_cast<char*>(buffer), entry->get_size());

                    delete compressBuffer;
                    delete buffer;
                }
                else
                    m_file.read(&names[0], entry->get_size());

                m_file.seekg(old_pos, m_file.beg);

                // List all names
                std::string name;
                std::istringstream namesStream(names);
                while(std::getline(namesStream, name, '\n'))
                    entry->get_names().emplace_back(name);
            }

            entry->set_root_path(entry->get_hash() == m_root_path_hash);
            entry->set_locale_root_path(entry->get_hash() == m_locale_root_path_hash);

            emplace_back(entry);
        }

        Entry *root = get_root();
        if(root == nullptr)
            root = get_locale_root();

        if(root == nullptr)
        {
            std::cerr << "[WARNING] No root entry found!" << std::endl;
            return;
        }

        populateDirectory(root);
    }

    void SCSFile::pack()
    {

    }

    void SCSFile::populateDirectory(Entry *entry)
    {
        if(entry->get_hash() == m_root_path_hash)
            entry->set_path("");
        else if(entry->get_hash() == m_locale_root_path_hash)
            entry->set_path("locale");

        bool is_recursive;
        std::string entry_name;
        std::string path;
        uint64_t hash;
        Entry *next;
        for(auto name : entry->get_names())
        {
            is_recursive = name[0] == '*';
            entry_name = (is_recursive) ? name.substr(1) : name;
            path = (!entry->get_path().empty()) ? entry->get_path() + "/" + entry_name : entry_name;
            hash = CityHash64(path.c_str(), path.size());

            next = find(path);
            if(next == nullptr)
            {
                std::cerr << "No entry found for " << path << " : " << hash  << " - " << is_recursive << std::endl;
                continue;
            }

            next->set_path(path);

            std::cout << path << std::endl;

            if(is_recursive)
                populateDirectory(next);
        }
    }

    SCSFile::~SCSFile()
    {
        m_file.close();
    }
}