#include "../libIRC.hpp"

void	Server::NICK(int fd, std::string param){

	Client client = this->getClientWithFd(fd);	//Verif passage de la commande PASS
	if (client.getPassCheckState() == false){
		this->sendErrMessage(fd, "PASS not done, need to set it before doing NICK\r\n");
		return ;
	}
	//Verification si le param est vide si le client possede deja un nickname
	if (param.empty()){
		if (client.getNickname().empty()){
			this->sendErrMessage(fd, ERR_NONICKNAMEGIVEN(this->_name, client.getNickname()));
		}
		else
			this->sendRepMessage(fd, client.getNickname());
		return ;
	}
	if (client.getUserCheckState() == false)
		if (this->checkNickname(param) == ERROR)
		{
			this->sendErrMessage(fd, ERR_ERRONEUSNICKNAME(this->_name, client.getNickname(), param));
			return ;
		}

	if (this->checkDoublon(fd, param) == ERROR)
	{
		this->sendErrMessage(fd, ERR_NICKNAMEINUSE(this->_name, client.getNickname(), param));
		return ;
	}

	if (client.getNickname() == param)
		return ;
	
	std::string ret = ":"+client.getNickname()+"!~"+client.getUsername()+"@"+this->_name + " NICK "+ param +"\r\n";
	this->sendRepMessage(fd, ret);
	this->getClientWithFd(fd).setNickname(param);
	this->getClientWithFd(fd).setNickCheckTrue();
}