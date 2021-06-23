#include <cstdlib>
#include <iostream>
#include <libscs/scs.hpp>

void print(SCS::SCSFile& file, std::filesystem::path base_path, SCS::Entry* root_entry);
void usage();

int main(int argc, char **argv)
{
    std::string file_path;

	for(int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		if(arg == "-h" || arg == "--help")
		{
			usage();
			return EXIT_SUCCESS;
		}
		else if(arg.find("-", 0, 2) != std::string::npos)
			std::cerr << "scsinfo: invalid option « " << arg << " »" << std::endl;
		else
			file_path = arg;
	}

	if(file_path.empty())
	{
		usage();
		return EXIT_SUCCESS;
	}

    try
    {
        SCS::SCSFile scs_file(file_path);

        std::cout << "Hash method: " << SCS::to_string(scs_file.get_hash_method()) << std::endl;
        std::cout << scs_file.size() << " entries" << std::endl;

        std::string root_path = "";
        SCS::Entry *root = scs_file.get_root();
        if(root == nullptr)
        {
            root_path = "locale";
            root = scs_file.get_locale();
        }

        if(root == nullptr)
        {
            std::cerr << "No root entry found !" << std::endl;
            return EXIT_FAILURE;
        }

        print(scs_file, root_path, root);
    }
	catch(std::exception const &e)
	{
		std::cerr << "scsinfo: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}

void print(SCS::SCSFile& file, std::filesystem::path base_path, SCS::Entry* root_entry)
{
    std::cout << "Hash: " << root_entry->get_hash() << std::endl;
    std::cout << "Offset: " << root_entry->get_offset() << std::endl;
    std::cout << "Type: " << SCS::to_string(root_entry->get_type()) << std::endl;
    std::cout << "CRC: " << std::hex << root_entry->get_crc() << std::endl;
    std::cout << "Size: " << std::dec << root_entry->get_size() << " bytes" << std::endl;
    std::cout << "Compressed size: " << root_entry->get_compressed_size() << " bytes" << std::endl;
    std::cout << "Is root directory: " << (root_entry->is_root_directory() ? "True" : "False") << std::endl;

    if(root_entry->is_directory())
    {
        std::cout << "Names :" << std::endl;
        for(auto name : root_entry->get_names())
            std::cout << "- " << name << std::endl;
    }
    else if(root_entry->is_file())
        std::cout << "Path: " << base_path.string() << std::endl;

    std::cout << std::endl;

    if(root_entry->is_file())
        return;

    bool is_recursive;
    std::string entry_name;
    std::filesystem::path path;
    SCS::Entry* entry;

    for(auto name : root_entry->get_names())
    {
        is_recursive = name[0] == '*';
        entry_name = is_recursive ? name.substr(1) : name;
        path = base_path.empty() ? std::filesystem::path(entry_name) : base_path / entry_name;

        entry = file.find(path);
        if(entry)
            print(file, path, entry);
        else
            std::cerr << "No entry found for " << path << std::endl;
    }
}

void usage()
{
	std::cout << "Usage: scsinfo [OPTION]... [FILE]" << std::endl;
}