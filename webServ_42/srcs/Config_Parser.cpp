#include "Config_Parser.hpp"

Config_Parser::Config_Parser(const char *filename)
{
    std::string read;
    std::ifstream file;

    content.clear();
    file.open(filename);
    if (!file.is_open())
    {
        while (!file.eof())
        {
            std::getline(file, read);
            content.append(read + "\n");
        }
        file.close();
    }else
    {
        std::cout << RED "Error: Could not open config file." RESET << std::endl;
        exit(1);
    }
}

Config_Parser::~Config_Parser(){}

