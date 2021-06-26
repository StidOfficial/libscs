#include "filesystem.hpp"

#include <cstring>
#include <iostream>

namespace SCS
{
    Filesystem::Filesystem(std::filesystem::path base_path)
    {
        load_directory(base_path);
    }

    Filesystem::Filesystem()
    {
    }

    void Filesystem::load(std::filesystem::path path)
    {
        auto archive = new SCSFile(path);

        std::cout << "[hashfs] " << path.filename().string() << ": Mounted ok, " << archive->size() << " entries"  << std::endl;

        m_archives.emplace_back(archive);
    }

    void Filesystem::load_directory(std::filesystem::path path)
    {
        for(const auto& entry : std::filesystem::directory_iterator(path))
            if(entry.path().has_extension() &&
                entry.path().extension().string() == SCS_EXTENSION)
                load(entry.path());
    }

    std::optional<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> Filesystem::get_entry(std::filesystem::path path)
    {
        for(const auto& archive : m_archives)
        {
            auto entry = archive->find(path);
            if(entry != nullptr)
                return { std::make_pair(archive, entry) };
        }

        return std::nullopt;
    }

    bool Filesystem::exists(std::filesystem::path path)
    {
        auto entry = get_entry(path);

        return exists(entry);
    }

    bool Filesystem::is_file(std::filesystem::path path)
    {
        auto entry = get_entry(path);

        return is_file(entry);
    }

    bool Filesystem::is_directory(std::filesystem::path path)
    {
        auto entry = get_entry(path);

        return is_directory(entry);
    }

    std::vector<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> Filesystem::get_entries(std::filesystem::path path)
    {
        std::vector<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> entries;

        for(const auto& archive : m_archives)
        {
            auto entry = archive->find(path);
            if(entry != nullptr)
                entries.emplace_back(std::make_pair(archive, entry));
        }

        return entries;
    }

    std::vector<std::string> Filesystem::get_entries_names(std::filesystem::path path, bool directory_only)
    {
        std::vector<std::string> names;

        auto entries = get_entries(path);
        if(entries.empty())
            throw std::logic_error(std::strerror(ENOENT));

        for(const auto& entry : entries)
        {
            auto _entry = entry.second;
            if(!_entry->is_directory())
                throw std::logic_error(std::strerror(ENOTDIR));

            for(const auto& name : _entry->get_names())
                if(name[0] == '*')
                {
                    if(directory_only)
                        names.emplace_back(name.substr(1));
                }
                else
                {
                    if(!directory_only)
                        names.emplace_back(name);
                }
        }

        return names;
    }

    std::vector<std::string> Filesystem::get_directories(std::filesystem::path path)
    {
        return get_entries_names(path, true);
    }

    std::vector<std::string> Filesystem::get_files(std::filesystem::path path)
    {
        return get_entries_names(path, false);
    }

    std::ifstream Filesystem::get_file(std::filesystem::path path)
    {
        auto entry = get_entry(path);
        if(!entry.has_value())
            throw std::logic_error(std::strerror(ENOENT));

        return get_file(entry.value());
    }

    bool Filesystem::exists(std::optional<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> entry)
    {
        return entry.has_value();
    }

    bool Filesystem::is_file(std::optional<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> entry)
    {
        if(!entry.has_value())
            return false;

        return entry.value().second->is_file();
    }

    bool Filesystem::is_directory(std::optional<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> entry)
    {
        if(!entry.has_value())
            return false;

        return entry.value().second->is_directory();
    }

    std::ifstream Filesystem::get_file(std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*> entry)
    {
        if(!entry.second->is_file())
            throw std::logic_error(std::strerror(EISDIR));

        auto stream = std::ifstream(entry.first.get()->path(), std::ios::binary);
        stream.seekg(entry.second->get_offset(), stream.beg);
        stream.seekg(entry.second->get_offset() + entry.second->get_compressed_size(), stream.end);
        stream.seekg(entry.second->get_offset());

        return stream;
    }
}