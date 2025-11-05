/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config_Parser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:31:25 by gcosta-m          #+#    #+#             */
/*   Updated: 2025/11/03 10:24:03 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP

# include <Utils.hpp>
# include <fstream>

# define SUCCESS 0
# define FAILURE -1
# define EMPTY -2

class Config_Parser
{
  private:
	std::string content;

	Server parse_server(size_t *index);
	Location parse_location(size_t *index);

	int set_server_value(Server *server, const std::string key, const std::string value);
	int set_location_value(Location *location, const std::string key,const std::string value);
	int check_line_syntax(const std::string line);
	int print_parse_error();

  public:
	Config_Parser(const char *filename);
	~Config_Parser();

	std::vector<Server> *parse();
};

#endif