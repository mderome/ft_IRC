/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esafar <esafar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 13:53:37 by esafar            #+#    #+#             */
/*   Updated: 2022/12/01 14:14:33 by esafar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <string>
# include <iostream>
# include <cstdlib>

# define	BUF_SIZE 10000

class Server
{
    public:
        Server();
        Server(std::string port, std::string password);
        ~Server();

        std::string getPort(void)const;
        std::string getPassword(void)const;

        void    createListener(void);

    private:
        std::string _port;
        std::string _password;
};

#endif