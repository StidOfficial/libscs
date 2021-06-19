#include "sii.hpp"

#include <cstring>
#include <limits>
#include <iostream>

namespace SCS
{
    SIIFile::SIIFile(std::filesystem::path file_path)
        : SIIFile()
    {
        open(file_path);

        parse();
    }

    SIIFile::SIIFile()
        : m_file()
    {

    }

    void SIIFile::open(std::filesystem::path file_path)
    {
        if(!std::filesystem::exists(file_path))
            throw std::logic_error(std::strerror(ENOENT));

        if(std::filesystem::is_directory(file_path))
            throw std::logic_error(std::strerror(EISDIR));

        m_file.open(file_path, std::fstream::in);
        if(!m_file.is_open())
            throw std::logic_error(std::strerror(EACCES));
    }

    void SIIFile::parse()
    {
        std::string magicMark;

        std::getline(m_file, magicMark);
        if(magicMark != MAGIC_MARK)
            throw std::logic_error("Invalid magic mark");

        bool isGlobalScope = false;
        bool isUnitScope = false;
        bool isClassName = false;
        bool isUnitSeparator = false;
        bool isUnitName = false;
        bool isAttribute = false;
        bool isInstruction = false;
        bool isInstructionArg = false;
        bool isAttributeName = false;
        bool isAttributeValue = false;

        std::string className;
        std::string unitName;
        std::string attributeName;
        std::string attributeValue;
        std::string instruction;
        std::string instructionArg;

        char character;
        while(!m_file.eof())
        {
            character = m_file.get();
            if(m_file.eof())
                break;

            if(character == '/')
            {
                character = m_file.get();
                if(character == '/')
                {
                    m_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    if(isAttribute)
                    {
                        isAttributeName = true;
                        isAttributeValue = false;

                        std::cout << "Attribute : " << attributeName << " - " << attributeValue << std::endl;

                        attributeName = "";
                        attributeValue = "";
                    }
                }
                else if(character == '*')
                {
                    while(true)
                    {
                        m_file.ignore(std::numeric_limits<std::streamsize>::max(), '*');

                        character = m_file.get();
                        if(character == '/')
                            break;
                        else
                            m_file.unget();
                    }
                }
            }
            else if(character == '#')
            {
                m_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                m_file.unget();
            }
            else if(character == '{')
                if(!isGlobalScope)
                {
                    isGlobalScope = true;
                    isClassName = true;
                }
                else if(!isUnitScope)
                {
                    isUnitScope = true;
                    isAttribute = true;
                    isAttributeName = true;
                }
                else
                    throw std::logic_error("Invalid {");
            else if(character == '}')
                if(isUnitScope)
                {
                    isUnitScope = false;
                    isAttribute = false;
                    isAttributeValue = false;

                    unitName = "";
                    className = "";
                }
                else if(isGlobalScope)
                    isGlobalScope = false;
                else
                    throw std::logic_error("Invalid }");
            else if(isClassName)
                if(std::isspace(character) && className.length() == 0)
                    continue;
                else if(std::isspace(character) || character == ':')
                {
                    isClassName = false;
                    if(character == ':')
                        isUnitName = true;
                    else
                        isUnitSeparator = true;
                }
                else
                    className += character;
            else if(isUnitSeparator)
                if(std::isspace(character))
                    continue;
                else if(character == ':')
                {
                    isUnitSeparator = false;
                    isUnitName = true;
                }
                else
                    throw std::logic_error("Invalid :");
            else if(isUnitName)
                if(std::isspace(character) && unitName.length() == 0)
                    continue;
                else if(std::isspace(character))
                {
                    std::cout << "Unit / Class name : " << className << " - " << unitName << std::endl;

                    isUnitName = false;
                }
                else
                    unitName += character;
            else if(isAttribute)
                if(isAttributeName && character == '@')
                {
                    isAttribute = false;
                    isInstruction = true;
                }
                else if(character == ':')
                {
                    isAttributeName = false;
                    isAttributeValue = true;
                }
                else if(character == '\n' && isAttributeValue)
                {
                    isAttributeName = true;
                    isAttributeValue = false;

                    std::cout << "Attribute : " << attributeName << " - " << attributeValue << std::endl;

                    attributeName = "";
                    attributeValue = "";
                }
                else if(std::isspace(character))
                    continue;
                else
                    if(isAttributeName)
                        attributeName += character;
                    else
                        attributeValue += character;
            else if(isInstruction)
                if(std::isspace(character))
                {
                    isInstruction = false;
                    isInstructionArg = true;
                }
                else
                    instruction += character;
            else if(isInstructionArg)
                if(character == '\n')
                {
                    isInstructionArg = false;

                    std::cout << "Instruction / Instruction arg : " << instruction << " - " << instructionArg << std::endl;

                    instruction = "";
                    instructionArg = "";
                }
                else
                    instructionArg += character;
            else if(std::isspace(character))
                continue;
            else
                throw std::logic_error("Invalid state");
        }
    }
}