#include "../libIRC.hpp"

void	Server::PRIVMSG(int fd, std::string param)
{
	std::vector<std::string>	list_param;
    Client client = this->getClientWithFd(fd);

	if (client.getUserCheckState() == false)
    {
		this->sendErrMessage(fd, "USER not registered yet so command rejected\r\n");
		return ;
	}
	if (param.empty())
    {
		this->sendErrMessage(fd, ERR_NORECIPIENT(this->_name, "PRIVMSG"));
		return ;
	}
	list_param = this->once_split(param, " ");
	if (list_param.size() == 1)
    {
		this->sendErrMessage(fd,"412 :No text to send\r\n");
		return ;
	}
	if (this->inListClientServ(list_param.at(0)) == OK)
    {
		this->sendRepMessage(this->getClientWithName(list_param.at(0)).getFd(), PRIVSMG_CLI_INFO(client.getNickname(), list_param.at(0), this->_name, list_param.at(1)));
	}
	else if (this->inListChannelServ(list_param.at(0)) == OK)
    {
        this->getChannelWithName(list_param.at(0)).replyToAll(PRIVSMG_CHAN_INFO(client.getNickname(), client.getUsername(), this->_name, list_param.at(0), list_param.at(1)));
	}
    else
    {
        this->sendErrMessage(fd, ERR_NOSUCHNICK(this->_name, this->getClientWithName(list_param.at(0)).getNickname()));
        return;
    }
}