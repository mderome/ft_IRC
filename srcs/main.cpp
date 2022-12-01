#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include "utils/parsing.hpp"
#include "utils/test.hpp"

#define	BUF_SIZE 10000


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
		if (std::atoi(av[1]) < 0)
		{
			std::cerr << "The port can't be inferior at 0\n";
			return -2;
		}
		else if (std::atoi(av[1]) > 65535)
		{
			std::cerr << "The port can't be superior at 65535\n";
			return -3;
		}
		else
		{
			test         t(av[2]);

			std::cout << "Server name : " << t.get_name() << std::endl;
			std::cout << "Valid server name : " << t.valid_server_name(t.get_name()) << std::endl;
			std::cout << "Valid user name : " << t.valid_user_name(t.get_name()) << std::endl;
			std::cout << "Valid channel name : " << t.valid_channel_name(t.get_name()) << std::endl;
			std::cout << "Is local : " << t.is_local(t.get_name()) << std::endl;
			std::cout << "Is regular : " << t.is_regular(t.get_name()) << std::endl;
		}
	}
	else
	{
		std::cerr << "Usage: %s port password.\n";
		return -1;
	}
	
	return (0);
}