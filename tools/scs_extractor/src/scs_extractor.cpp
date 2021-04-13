#include <cstdlib>
#include <iostream>
#include <libscs/scs.hpp>
#include <zlib.h>

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

            SCS::Entry *root = scs_file.get_root();
            for(auto entry : scs_file)
                if(entry->is_root_directory())
                    continue;
                else if(entry->is_directory())
                    std::filesystem::create_directories(output_path / entry->get_path());
                else if(entry->is_file())
                {
                    
                    std::ifstream input(file_path, std::ios::binary);
                    std::ofstream output(output_path / entry->get_path(), std::ios_base::binary);

                    input.seekg(entry->get_offset());

                    if(entry->is_compressed())
                    {
                        std::vector<char> compressed_buffer(entry->get_compressed_size());
                        input.read(compressed_buffer.data(), compressed_buffer.size());

                        std::vector<char> buffer(entry->get_size());
                        uLongf buffer_size = buffer.size();
                        int result = uncompress(reinterpret_cast<Bytef*>(buffer.data()), &buffer_size, reinterpret_cast<Bytef*>(compressed_buffer.data()), compressed_buffer.size());
                        if(Z_OK != result)
                        {
                            std::cerr << "*** ERROR *** : Failed to uncompress for " << entry->get_path() << std::endl;

                            output.close();
                            input.close();
                            break;
                        }

                        output.write(buffer.data(), buffer.size());
                    }
                    else
                    {
                        std::vector<char> buffer(entry->get_size());
                        input.read(buffer.data(), buffer.size());
                        output.write(buffer.data(), buffer.size());
                    }

                    output.close();
                    input.close();
                }
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

void usage()
{
	std::cout << "scs_extractor: usage: scs_extractor archive.scs [output_path]" << std::endl;
}