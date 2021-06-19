#include <libscs/sii.hpp>

#include <iostream>
#include <cstdlib>
#include <filesystem>

int main(int argc, char **argv)
{
    std::string folder = "../../tests";

    if(argc > 1)
        folder = argv[1];

    for(const auto& entry : std::filesystem::directory_iterator(folder))
    {
        if(entry.path().extension().string() != ".sii")
            continue;

        try
        {
            SCS::SIIFile file(entry.path());

            std::cout << entry.path().c_str() << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;

            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}