#include "../libIRC.hpp"

void	Server::PASS(int fd, std::string param)
{
	Client client = this->getClientWithFd(fd);
	if (param.empty())
	{
		this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "PASS"));
		return ;
	}
	if (client.getUserCheckState() == true)
	{
		return ;
	}
	if (client.getPassCheckState() == true)
	{
		this->sendErrMessage(fd, ERR_ALREADYREGISTRED(this->_name, client.getNickname()));
		return ;
	}
	if (param != this->_passwrd_serv)
	{
		this->sendErrMessage(fd, ERR_PASSWDMISMATCH(this->_name, client.getNickname()));
		return ;
	}
	this->getClientWithFd(fd).setPassCheckTrue();
}