/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_Parser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:31:29 by gcosta-m          #+#    #+#             */
/*   Updated: 2025/10/21 10:31:30 by gcosta-m         ###   ########.fr       */
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

Config_Parser::~Config_Parser()
{
}
