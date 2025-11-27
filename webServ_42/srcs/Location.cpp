/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 10:19:20 by gabastos          #+#    #+#             */
/*   Updated: 2025/11/27 15:00:49 by gabastos         ###   ########.fr       */
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
    
    std::cout << "> index: ";
    for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); ++it)
    {
        std::cout << *it;
        if (it + 1 != index.end())
            std::cout << ", ";
    }
    std::cout << "\n";
    
    std::cout << "> allow_methods: ";
    for (std::vector<MethodType>::iterator it = allow_methods.begin(); it != allow_methods.end(); ++it)
    {
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

    std::map<std::string, std::string>::const_iterator it = this->cgi_info.find(extension);
    
    if (it != this->cgi_info.end())
    {
        return it->second;
    }
    
    it = this->cgi_info.find("." + extension);
    
    if (it != this->cgi_info.end())
    {
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