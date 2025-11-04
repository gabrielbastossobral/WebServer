/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_Parser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:31:29 by gcosta-m          #+#    #+#             */
/*   Updated: 2025/11/04 10:33:03 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Config_Parser.hpp"

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
	size_t key_start;
	size_t value_end;

	size_t pre = content.find_first_not_of(" \n\t", *index);
	if (pre == std::string::npos || content[pre] != '{')
		exit(print_parse_error());

	pre++;
	size_t cur = content.find_first_not_of(" \n\t", pre);
	while(cur != std::string::npos)
	{
		if((pre = content.find_first_not_of(" \n\t", cur)) == std::string::npos)
			exit(print_parse_error());
		key_start = pre;
		if ((cur = content.find_first_of(" \n\t", key_start)) == std::string::npos)
			exit(print_parse_error());
		std::string key = content.substr(key_start, cur - key_start);
		if (key == "}")
		{
			*index = content.find_first_not_of(" \n\t", cur + 1);
			break;
		}
		if (key == "location")
		{
			server.add_location(parse_location(&cur));
		}
		else
		{
			if ((pre = content.find_first_not_of(" \n\t", cur)) == std::string::npos)
				exit(print_parse_error());
			if ((cur = content.find_first_of(" \n\t;", pre)) == std::string::npos)
				exit(print_parse_error());
			if ((value_end = content.find_first_not_of(" \n\t", cur + 1)) == std::string::npos)
				exit(print_parse_error());
			if ((int)value_end == EMPTY)
				continue;
			std::string value = content.substr(pre, value_end - pre + key_start + 1);
			if (set_server_value(&server, key, value) == FAILURE)
				exit(print_parse_error());
		}
	}
	return server;
}

Location Config_Parser::parse_location(size_t *index)
{
	Location location;
	size_t key_start;
	size_t value_end;

	size_t pre = content.find_first_not_of(" \n\t", *index);
	size_t cur = content.find_first_of(" \n\t", pre);
	location.path = content.substr(pre, cur - pre);

	pre = content.find_first_not_of(" \n\t", cur);
	if (pre == std::string::npos || content[pre] != '{')
		exit(print_parse_error());
	pre++;
	cur = content.find_first_not_of(" \n\t", pre);
	while(cur != std::string::npos)
	{
		if ((pre = content.find_first_not_of(" \n\t", cur)) == std::string::npos)
			exit(print_parse_error());
		key_start = pre;
		if ((cur = content.find_first_of(" \n\t", key_start)) == std::string::npos)
			exit(print_parse_error());
		std::string key = content.substr(pre, cur - pre);
		if (key == "}")
		{
			*index = cur;
			break;
		}else
		{
			if ((pre = content.find_first_not_of(" \n\t", cur)) == std::string::npos)
				exit(print_parse_error());
			if ((cur = content.find_first_of(" \n\t;", pre)) == std::string::npos)
				exit(print_parse_error());
			if ((value_end = check_line_syntax(content.substr(key_start, cur - key_start))) == FAILURE)
				exit(print_parse_error());
			if ((int)value_end == EMPTY)
				continue;
			std::string value = content.substr(pre, value_end - pre + key_start + 1);
			if (set_location_value(&location, key, value) == FAILURE)
				exit(print_parse_error());
		}
	}
	return location;
}