/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gcosta-m <gcosta-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 10:38:51 by gcosta-m          #+#    #+#             */
/*   Updated: 2025/10/21 10:46:40 by gcosta-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Utils.hpp"

int replace(std::string original, std::string to_replace, std::string replacement)
{
	size_t pos = original.find(to_replace);
	if (pos != std::string::npos)
	{
		original.erase(pos, to_replace.length());
		original.insert(pos, replacement);
		return 0;
	}
	return -1;
}

std::string dir_listing()
{
	
}