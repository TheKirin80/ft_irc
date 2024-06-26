#include "../libIRC.hpp"


void	Server::QUIT(int fd, std::string param)
{
	std::vector<Client>::iterator itclient = this->_list_client_serv.begin();
	std::vector<Channel>::iterator itchan = this->_list_channel_serv.begin();
	std::vector<Channel>::iterator itchane = this->_list_channel_serv.end();
    std::string part_param;
	for (; itchan != itchane; itchan++){
		if ((*itchan).in_list_client(this->getClientWithFd(fd).getNickname()) == OK) // On verifie si le client est dans chacun des channels ce qui correspond a PART le channel
        {
            this->PART(fd, (*itchan).getName());
		}
	}
	if (param.empty())
		this->sendRepMessage(fd, QUIT_INFO(this->getClientWithFd(fd).getNickname(), this->getClientWithFd(fd).getUsername(), this->_name, ""));
	else
		this->sendRepMessage(fd, QUIT_INFO(this->getClientWithFd(fd).getNickname(), this->getClientWithFd(fd).getUsername(), this->_name, " " + param));
	for (; itclient != this->_list_client_serv.end(); itclient++)
    {
		if ((*itclient).getFd() == fd){
			this->_list_client_serv.erase(itclient);
			break ;
		}
	}
	rm_fd(fd);
}