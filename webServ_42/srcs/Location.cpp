/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 10:19:20 by gabastos          #+#    #+#             */
/*   Updated: 2025/11/18 10:30:17 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"

Location::Location(/* args */)
{
	path = "";
	root = "";
}

Location::~Location()
{
}

void Location::print_location_info()
{
	std::cout << "----------------- Location Info -----------------\n";
    std::cout << "> path: " << path << "\n";
    std::cout << "> root: " << root << "\n";
    
    // Iterar sobre o vector de index
    std::cout << "> index: ";
    for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); ++it)
    {
        std::cout << *it;
        if (it + 1 != index.end())
            std::cout << ", ";
    }
    std::cout << "\n";
    
    // Iterar sobre o vector de allow_methods
    std::cout << "> allow_methods: ";
    for (std::vector<MethodType>::iterator it = allow_methods.begin(); it != allow_methods.end(); ++it)
    {
        // Converter MethodType para string
        if (*it == GET)
            std::cout << "GET";
        else if (*it == POST)
            std::cout << "POST";
        else if (*it == DELETE)
            std::cout << "DELETE";
        else
            std::cout << "INVALID";
        
        if (it + 1 != allow_methods.end())
            std::cout << ", ";
    }
    std::cout << "\n";
    
    // Iterar sobre o map de cgi_info
    for (std::map<std::string, std::string>::iterator i = cgi_info.begin(); i != cgi_info.end(); i++)
    {
        std::cout << "> cgi_info: " << (*i).first << ", " << (*i).second << "\n";
    }
}

MethodType Location::s_to_methodtype(std::string str)
{
	if (str == "GET")
	{
		return GET;
	}
	else if (str == "POST")
	{
		return POST;
	}
	else if (str == "DELETE")
	{
		return DELETE;
	}
	return INVALID;
}

std::string Location::getCgiBinary(std::string &extension)
{
	for (std::map<std::string, std::string>::const_iterator it = this->cgi_info.begin();
	it != this->cgi_info.end(); ++it)
	{
		if (it->first == "." + extension)
			return it->second;
	}
	return "";
}

std::string Location::get_root(void)
{
	return this->root;
}

std::string Location::get_path(void)
{
	return this->path;
}