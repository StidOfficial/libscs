#include <cstdlib>
#include <iostream>
#include <libscs/scs.hpp>
#include <libscs/filesystem.hpp>
#include <zlib.h>

void extract(SCS::Filesystem& filesystem, std::filesystem::path output_path, std::filesystem::path base_path);
void usage();

int main(int argc, char **argv)
{
    std::filesystem::path file_path;
	std::filesystem::path output_path;

	if(argc == 3)
	{
        file_path = argv[1];
        output_path = argv[2];

        SCS::Filesystem filesystem;
        try
        {
            filesystem.load(file_path);
        }
        catch(std::exception)
        {
            std::cerr << "*** ERROR *** : Failed to mount the archive!" << std::endl;
            return EXIT_FAILURE;
        }

        if(filesystem.exists(""))
            extract(filesystem, output_path, "");
        else if(filesystem.exists("locale"))
            extract(filesystem, output_path, "locale");
        else
        {
            std::cerr << "No root entry found !" << std::endl;
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

void extract(SCS::Filesystem& filesystem, std::filesystem::path output_path, std::filesystem::path base_path)
{
    auto entry = filesystem.get_entry(base_path);

    if(SCS::Filesystem::is_directory(entry))
    {
        for(const auto& name : filesystem.get_directories(base_path))
        {
            auto path = base_path.empty() ? std::filesystem::path(name) : base_path / name;

            extract(filesystem, output_path, path);
        }

        for(const auto& name : filesystem.get_files(base_path))
        {
            auto path = base_path.empty() ? std::filesystem::path(name) : base_path / name;

            extract(filesystem, output_path, path);
        }

        return;
    }

    if(!SCS::Filesystem::exists(entry))
    {
        std::cerr << "No entry found : " << base_path << std::endl;
        return;
    }

    std::filesystem::path output_file_path = output_path / base_path;

    std::filesystem::create_directories(output_file_path.parent_path());

    auto root_entry = entry.value();

    std::ifstream input = filesystem.get_file(root_entry);
    std::ofstream output(output_file_path, std::ios_base::binary);
    
    if(root_entry.second->is_compressed())
    {
        std::vector<char> compressed_buffer(root_entry.second->get_compressed_size());
        input.read(compressed_buffer.data(), compressed_buffer.size());

        std::vector<char> buffer(root_entry.second->get_size());
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
        std::vector<char> buffer(root_entry.second->get_size());
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