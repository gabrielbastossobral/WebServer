/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:31:20 by gcosta-m          #+#    #+#             */
/*   Updated: 2025/10/21 10:38:12 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <sstream>
#include <ctime>
#include <dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

int replace(std::string original, std::string to_replace, std::string replacement);
std::string dir_listing();
std::vector<std::string> split(const std::string &str, char delimiter);
std::string get_ip(int client_fd);
std::string get_current_date_GMT(void);
size_t string_to_hexa(std::string input);
char *ft_strnstr(const char *haystack, const char *needle, size_t len);

#endif