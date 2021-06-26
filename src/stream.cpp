#include "stream.hpp"

#include <cstring>
#include <cerrno>

namespace SCS
{
    Stream::Stream(std::filesystem::path path)
        : Stream()
    {
        open(path);
    }

    Stream::Stream()
    {
    }

    void Stream::open(std::filesystem::path path)
    {
        if(std::filesystem::is_directory(path))
			throw std::logic_error(std::strerror(EISDIR));

        m_path = path;

        m_file.open(m_path, std::fstream::in | std::fstream::binary);
		if(!m_file.is_open())
			throw std::logic_error(std::strerror(errno));
    }

    void Stream::close()
    {
        
    }

    std::filesystem::path Stream::path()
    {
        return m_path;
    }

    void Stream::read(std::string &text)
    {
        std::getline(m_file, text, '\0');
    }

    void Stream::read(char *data, std::streamsize size)
	{
		m_file.read(data, size);
	}

    Stream::~Stream()
    {
        m_file.close();
    }
}