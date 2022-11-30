#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>

#define	BUF_SIZE 10000


int	main(int ac, char **av)
{
	struct addrinfo 		hint;
	struct addrinfo 		*result, *rp;
	int						sfd, s;
	struct sockaddr_storage peer_addr;
	socklen_t				peer_addr_len;
	ssize_t					nread;
	char 					buff[BUF_SIZE];

	if (ac != 3)
	{
		std::cerr << "Usage: %s port password.\n";
		return -1;
	}
	if (static_cast<int>av[1] < 0)
	{
		std::cerr << "The port can't be inferior at 0\n";
		return -2;
	}
	
	return 0;
}