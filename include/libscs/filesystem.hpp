#pragma once

#include "scs.hpp"

#include <filesystem>
#include <vector>
#include <utility>
#include <optional>

#define SCS_EXTENSION ".scs"
#define STEAM_ETS2_DEFAULT_PATH "/home/steam/.steam/steam/steamapps/common/Euro Truck Simulator 2/"

namespace SCS
{
    class Filesystem
    {
    public:
        Filesystem(std::filesystem::path base_path);
        Filesystem();

        void load(std::filesystem::path path);
        void load_directory(std::filesystem::path path);
        std::optional<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> get_entry(std::filesystem::path path);
        bool exists(std::filesystem::path path);
        bool is_file(std::filesystem::path path);
        bool is_directory(std::filesystem::path path);
        std::vector<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> get_entries(std::filesystem::path path);
        std::vector<std::string> get_entries_names(std::filesystem::path path, bool directory_only);
        std::vector<std::string> get_directories(std::filesystem::path path);
        std::vector<std::string> get_files(std::filesystem::path path);
        std::ifstream get_file(std::filesystem::path path);

        static bool exists(std::optional<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> entry);
        static bool is_file(std::optional<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> entry);
        static bool is_directory(std::optional<std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*>> entry);
        static std::ifstream get_file(std::pair<std::shared_ptr<SCS::SCSFile>, SCS::Entry*> entry);
    private:
        std::vector<std::shared_ptr<SCSFile>> m_archives;
    };
}