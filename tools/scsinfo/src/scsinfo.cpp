#include <cstdlib>
#include <iostream>
#include <libscs/scs.hpp>

void usage();

int main(int argc, char **argv)
{
    std::string file_path;
	bool signed_file = true;

	for(int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];
		if(arg == "-h" || arg == "--help")
		{
			usage();
			return EXIT_SUCCESS;
		}
		else if(arg.find("-", 0, 2) != std::string::npos)
			std::cerr << "pboinfo: invalid option « " << arg << " »" << std::endl;
		else
			file_path = arg;
	}

	if(file_path.empty())
	{
		usage();
		return EXIT_SUCCESS;
	}

    SCS::SCSFile scs_file(file_path);
    try
    {
        scs_file.unpack();
        std::cout << "Hash method: " << SCS::to_string(scs_file.get_hash_method()) << std::endl;
        std::cout << scs_file.size() << " entries" << std::endl;

        SCS::Entry *root = scs_file.get_root();

        for(auto entry : scs_file)
        {
            std::cout << "Hash: " << entry->get_hash() << std::endl;
            std::cout << "Offset: " << entry->get_offset() << std::endl;
            std::cout << "Type: " << SCS::to_string(entry->get_type()) << std::endl;
            std::cout << "CRC: " << std::hex << entry->get_crc() << std::endl;
            std::cout << "Size: " << std::dec << entry->get_size() << " bytes" << std::endl;
            std::cout << "Compressed size: " << entry->get_compressed_size() << " bytes" << std::endl;
            std::cout << "Is root directory: " << (entry->is_root_directory() ? "True" : "False") << std::endl;
            std::cout << "Is root path: " << (entry->is_root_path() ? "True" : "False") << std::endl;
            std::cout << "Is locale root path: " << (entry->is_locale_root_path() ? "True" : "False") << std::endl;

            if(entry->is_directory())
            {
                std::cout << "Names :" << std::endl;
                for(auto name : entry->get_names())
                    std::cout << "- " << name << std::endl;
            }

            std::cout << std::endl;
        }
    }
	catch(std::exception const &e)
	{
		std::cerr << "scsinfo: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

    return EXIT_SUCCESS;
}

void usage()
{
	std::cout << "Usage: scsinfo [OPTION]... [FILE]" << std::endl;
}