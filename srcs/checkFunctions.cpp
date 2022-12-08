#include "../inc/server.hpp"

bool    Server::checkChannelExistOnNetwork(std::string channel){
	try{
		_channels.at(channel);
		return (true);
	}
	catch(const std::out_of_range &e){
		return (false);
	}
}

bool    Server::checkUserExistOnNetwork(std::string user){
	for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); it++){
		if (user == it->second->getNickname())
			return (true);
	}
	return (false);
}