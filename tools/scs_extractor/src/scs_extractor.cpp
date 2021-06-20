#include <cstdlib>
#include <iostream>
#include <libscs/scs.hpp>
#include <zlib.h>

void extract(SCS::SCSFile& file, std::filesystem::path output_path, std::filesystem::path base_path, SCS::Entry* root_entry);
void usage();

int main(int argc, char **argv)
{
    std::filesystem::path file_path;
	std::filesystem::path output_path;

	if(argc == 3)
	{
        file_path = argv[1];
        output_path = argv[2];

        SCS::SCSFile scs_file(file_path);
        try
        {
            scs_file.unpack();
            std::cout << "[hashfs] " << file_path.filename().string() << ": Mounted ok, " << scs_file.size() << " entries"  << std::endl;

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

            extract(scs_file, output_path, root_path, root);
        }
        catch(std::exception)
        {
            std::cerr << "*** ERROR *** : Failed to mount the archive!" << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
    else
    {
		usage();
		return EXIT_SUCCESS;
	}
}

void extract(SCS::SCSFile& file, std::filesystem::path output_path, std::filesystem::path base_path, SCS::Entry* root_entry)
{
    if(root_entry->is_directory())
    {
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
                extract(file, output_path, path, entry);
            else
                std::cerr << "No entry found for " << path << std::endl;
        }

        return;
    }

    std::filesystem::path output_file_path = output_path / base_path;

    std::filesystem::create_directories(output_file_path.parent_path());

    std::ifstream input(file.path(), std::ios::binary);
    std::ofstream output(output_file_path, std::ios_base::binary);

    input.seekg(root_entry->get_offset());

    if(root_entry->is_compressed())
    {
        std::vector<char> compressed_buffer(root_entry->get_compressed_size());
        input.read(compressed_buffer.data(), compressed_buffer.size());

        std::vector<char> buffer(root_entry->get_size());
        uLongf buffer_size = buffer.size();
        int result = uncompress(reinterpret_cast<Bytef*>(buffer.data()), &buffer_size, reinterpret_cast<Bytef*>(compressed_buffer.data()), compressed_buffer.size());
        if(Z_OK != result)
        {
            std::cerr << "*** ERROR *** : Failed to uncompress for " << output_file_path.string() << std::endl;

            output.close();
            input.close();

            return;
        }

        output.write(buffer.data(), buffer.size());
    }
    else
    {
        std::vector<char> buffer(root_entry->get_size());
        input.read(buffer.data(), buffer.size());
        output.write(buffer.data(), buffer.size());
    }

    output.close();
    input.close();
}

void usage()
{
	std::cout << "scs_extractor: usage: scs_extractor archive.scs [output_path]" << std::endl;
}