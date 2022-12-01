/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esafar <esafar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 14:01:27 by esafar            #+#    #+#             */
/*   Updated: 2022/12/01 14:02:18 by esafar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() {}

Server::Server(std::string port, std::string password)
{
    _port = port;
    _password = password;
}

Server::~Server() {}

std::string Server::getPort(void)const
{
    return (this->_port);
}

std::string Server::getPassword(void)const
{
    return (this->_password);
}