#include "libIRC.hpp"

void	Server::PASS(int fd, std::string param){
	
	Client client = this->getClientWithFd(fd);
	if (client.getNickCheckState() == true){
		return ;
	}
	if (client.getUserCheckState() == true){
		return ;
	}
	if (client.getPassCheckState() == true){
		return ;
	}
	if (param.empty()){
		return ;
	}
	if (param != this->_passwrd_serv){
		return ;
	}
	client.setPassCheckTrue();
}