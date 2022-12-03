/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achane-l <achane-l@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 13:41:22 by esafar            #+#    #+#             */
/*   Updated: 2022/12/03 15:34:44 by achane-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <string.h>

#define	BUF_SIZE 10000

#include "../inc/server.hpp"

bool	checkPort(char  *port)
{
	for (size_t i = 0; i < strlen(port); i++)
	{
		if (port[i] < '0' || port[i] > '9')
			return (false);
	}
	return (true);
}

int	main(int ac, char **av)
{
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
			try {
				Server server(av[1], av[2]);
			}
			catch (std::exception &e) {
				std::cerr << e.what() << std::endl;
				return (EXIT_FAILURE);
			}
		}
	}
	else
	{
		std::cerr << "Error: usage: ./ircserv <port> <password>" << std::endl;
		return (EXIT_FAILURE);
	}
	return (0);
}