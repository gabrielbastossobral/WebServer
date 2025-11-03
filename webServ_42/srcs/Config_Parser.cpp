/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_Parser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:31:29 by gcosta-m          #+#    #+#             */
/*   Updated: 2025/11/03 14:26:18 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	}
	else
	{
		std::cout << RED "Error: Could not open config file." RESET << std::endl;
		exit(1);
	}
}

Config_Parser::~Config_Parser(){}

std::vector<Server> *Config_Parser::parse()
{
	std::vector<Server> *servers = new std::vector<Server>;

	std::cout << "Starting config parsing..." << std::endl;
	size_t pre = 0;
	size_t cur = content.find_first_not_of(" \n\t");

	while (cur != std::string::npos)
	{
		pre = content.find_first_not_of(" \n\t", cur);
		cur = content.find_first_of(" \n\t", pre);
		std::string token = content.substr(pre, cur - pre);
		if (token != "server")
		{
			print_parse_error();
			delete servers;
			return NULL;
		}
		Server server = parse_server(&cur);
		servers->push_back(server);
	}
	std::cout << "Config parsing completed successfully." << std::endl;
	return servers;
}

Server Config_Parser::parse_server(size_t *index)
{
	Server server;
	size_t pre = content.find_first_not_of(" \n\t", *index);
	if (pre == std::string::npos || content[pre] != '{')
	{
		print_parse_error();
		return server;
	}
	*index++;
	while (true)
	{
		pre = content.find_first_not_of(" \n\t", *index);
		if (pre == std::string::npos)
		{
			print_parse_error();
			return server;
		}
		if (content[pre] == '}')
		{
			*index = content.find_first_not_of(" \n\t", pre + 1);
			break;
		}
		size_t key_end = content.find_first_of(" \n\t;", pre);
		if (key_end == std::string::npos)
		{
			print_parse_error();
			return server;
		}
		std::string key = content.substr(pre, key_end - pre);
		pre = key;
		if (key == "location")
		{
			server.locations.push_back(parse_location(index));
			continue;
		}
		pre = content.find_first_not_of(" \n\t", pre);
		if (pre == )
	}
	
}