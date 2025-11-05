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

int Config_Parser::set_server_value(Server *server, const std::string key, const std::string value)
{
	if (key == "server_name")
	{
		server->server_name = value;
	}
	else if (key == "listen")
	{
		std::vector<std::string> tmp = split(value, ':');
		if (server->host != "" && server->host != tmp[0])
			return FAILURE;
		server->host = tmp[0];
		server->port = tmp[1];
	}
	else if (key == "root")
	{
		server->root = value;
	}
	else if (key == "index")
	{
		std::vector<std::string> tmp = split(value, ' ');
		for (unsigned long i = 0; i != tmp.size(); i++)
			server->index.push_back(tmp[i]);
	}
	else if (key == "allow_methods")
	{
		std::vector<std::string> tmp = split(value, ' ');
		for (unsigned long i = 0; i != tmp.size(); i++)
			server->allow_methods.push_back(Server::s_to_methodtype(tmp[i]));
	}
	else if (key == "autoindex")
	{
		server->autoindex = value == "on" ? true : false;
	}
	else if (key == "client_body_limit")
	{
		server->client_body_limit = atoi(value.c_str());
	}
	else if (key == "recv_timeout")
	{
		server->recv_timeout.tv_sec = atoi(value.c_str());
	}
	else if (key == "send_timeout")
	{
		server->send_timeout.tv_sec = atoi(value.c_str());
	}
	else if (key == "return")
	{
		std::vector<std::string> tmp = split(value, ' ');
		server->redirect_status = atoi(tmp[0].c_str());
		server->redirect_url = tmp[1];
	}
	else if (key == "error_page")
	{
		std::vector<std::string> tmp = split(value, ' ');
		std::string path = tmp[tmp.size() - 1];
		for (unsigned long i = 0; i != tmp.size() - 1; i++)
		{
			int status_code = atoi(tmp[i].c_str());
			if (server->error_pages.find(status_code) != server->error_pages.end())
				continue;
			server->error_pages[status_code] = path;
		}
	}
	else
	{
		return FAILURE;
	}
	return SUCCESS;
}

int Config_Parser::set_location_value(Location *location, const std::string key, const std::string value)
{
	if (key == "root")
	{
		location->root = value;
	}
	else if (key == "index")
	{
		std::vector<std::string> tmp = split(value, ' ');
		for (unsigned long i = 0; i != tmp.size(); i++)
			location->index.push_back(tmp[i]);
	}
	else if (key == "allow_methods")
	{
		std::vector<std::string> tmp = split(value, ' ');
		for (unsigned long i = 0; i != tmp.size(); i++)
			location->allow_methods.push_back(Location::s_to_methodtype(tmp[i]));
	}
	else if (key == "cgi_info")
	{
		unsigned long i = value.find_first_of(" ");
		if (i == std::string::npos)
			return FAILURE;
		int j = value.find_first_not_of(" ", i);
		location->cgi_info[value.substr(0, i)] = value.substr(j, value.length());
	}
	else if (key == "client_body_limit")
	{
		location->client_body_limit = atoi(value.c_str());
	}
	else
	{
		return FAILURE;
	}
	return SUCCESS;
}

int Config_Parser::check_line_syntax(const std::string line)
{
	size_t sharp;
	sharp = line.find('#');
	if (sharp != std::string::npos)
	{
		line.erase(sharp);
		if (line.find_first_not_of(" \t\n") != std::string::npos)
			return EMPTY;
	}

	size_t semicol;
	size_t find;
	semicol = line.find_first_of(';');
	if (semicol == std::string::npos)
		return FAILURE;
	find = line.find_first_not_of(" \t\n", semicol + 1, line.length() - semicol - 1);
	if (find != std::string::npos)
		return FAILURE;
	find = line.find_last_not_of(" \t\n", semicol - 1);
	return find;
}

int Config_Parser::print_parse_error()
{
	std::cout << RED "[ERROR]: Config parsing failed." RESET << std::endl;
	return FAILURE;
}