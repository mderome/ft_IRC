#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <cstdlib>

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
	}
	else
	{
		std::cerr << "Usage: %s port password.\n";
		return -1;
	}
	
	return (0);
}