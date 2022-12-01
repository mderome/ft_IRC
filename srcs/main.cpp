/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esafar <esafar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 13:41:22 by esafar            #+#    #+#             */
/*   Updated: 2022/12/01 13:43:46 by esafar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <cstdlib>

#define	BUF_SIZE 10000

bool	checkPort(std::string port)
{
	for (int i = 0; i < port.size(); i++)
	{
		if (port[i] < '0' || port[i] > '9')
			return (false);
	}
	return (true);
}

int	main(int ac, char **av)
{
	// struct addrinfo 		hint;
	// struct addrinfo 		*result, *rp;
	// int						sfd, s;
	// struct sockaddr_storage peer_addr;
	// socklen_t				peer_addr_len;
	// ssize_t					nread;
	// char 					buff[BUF_SIZE];

	if (ac == 3)
	{
		if (!checkPort(av[1]))
		{
			std::cerr << "Error: Invalid port number" << std::endl;
			return (EXIT_FAILURE);
		}
		else
		{
			//start running server here
		}
	}
	else
	{
		std::cerr << "Error: usage: ./ircserv <port> <password>" << std::endl;
		return (EXIT_FAILURE);
	}
	return (0);
}