/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DataStructs.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gabastos <gabastos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 10:27:33 by gabastos          #+#    #+#             */
/*   Updated: 2025/11/04 10:29:59 by gabastos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATASTRUCTS_HPP
# define DATASTRUCTS_HPP

# define MAX_REQUEST_SIZE 2048
# define MAX_URI_SIZE 64
# define BSIZE 1024

enum MethodType
{
	GET,
	POST,
	DELETE,
	INVALID,
};

#endif