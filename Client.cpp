#include "libIRC.hpp"

Client::Client(int fd) : _fd(fd), _pass_check(false), _nick_check(false), _user_check(false)
{
    return;
}

Client::~Client(void)
{
    this->_list_channel.clear();
    return;
}
//_username
std::string Client::getUsername(void) const
{
    return (this->_username);
}

void Client::setUsername(std::string str)
{
    this->_username = str;
}

//_nickname
std::string Client::getNickname(void) const
{
    return (this->_nickname);
}

void Client::setNickname(std::string str)
{
    this->_nickname = str;
}
//_hostname
std::string Client::getHostname(void) const
{
    return (this->_hostname);
}

void Client::setHostname(std::string str)
{
    this->_hostname = str;
}
//_realname
std::string Client::getRealname(void) const
{
    return (this->_realname);
}

void Client::setRealname(std::string str)
{
    this->_realname = str;
}
//_fd
int Client::getFd(void) const
{
    return (this->_fd);
}

//_list_channel
Channel	&Client::getChannel(std::string chanel)
{
	std::vector<Channel>::iterator it;
	
	for (it = this->_list_channel.begin(); it != this->_list_channel.end(); it++){
		if ((*it).getName() == chanel)
			break ;
	}
	return (*it);
}
//_pass_check
bool Client::getPassCheckState(void) const
{
    return (this->_pass_check);
}

void Client::setPassCheckTrue(void)
{
    this->_pass_check = true;
}
void Client::setPassCheckFalse(void)
{
    this->_pass_check = false;
}
//_nick_check
bool Client::getNickCheckState(void) const
{
    return (this->_nick_check);
}

void Client::setNickCheckTrue(void)
{
    this->_nick_check = true;
}
void Client::setNickCheckFalse(void)
{
    this->_nick_check = false;
}

//_user_check
bool Client::getUserCheckState(void) const
{
    return (this->_user_check);
}

void Client::setUserCheckTrue(void)
{
    this->_user_check = true;
}
void Client::setUserCheckFalse(void)
{
    this->_user_check = false;
}


//Utilitaire _list_channel
void Client::add_channel(Channel &newChan)
{
    this->_list_channel.push_back(newChan);
}
void Client::rm_channel(std::string chanel)
{
    std::vector<Channel>::iterator it =  this->_list_channel.begin();
	std::vector<Channel>::iterator ite = this->_list_channel.end();

    for (; it != ite; it++){
		if ((*it).getName() == chanel)
        {
			//necessite de supprimer le client dans le channel 
            this->_list_channel.erase(it);
			return ;
		}
	}
}
void Client::rm_all_channel()
{
    std::vector<Channel>::iterator it =  this->_list_channel.begin();
	std::vector<Channel>::iterator ite = this->_list_channel.end();

    for (; it != ite; it++)
		(*it).rm_client(*this);
	this->_list_channel.clear();
}

std::string	Client::list_channel(){
	std::vector<Channel>::iterator  it =  this->_list_channel.begin();
	std::vector<Channel>::iterator  ite = this->_list_channel.end();
	std::string					    ret;

	for (; it != ite; it++)
		ret += (*it).getName() + ",";
	if (this->_list_channel.size() != 0)
		ret.erase(ret.end() - 1);
	return ret;
}

int	Client::in_channel(std::string chanel)
{
	for (std::vector<Channel>::iterator it = this->_list_channel.begin(); it != this->_list_channel.end(); it++)
		if ((*it).getName() == chanel)
			return OK;
	return ERROR;
}

int Client::countChannel(void)
{
    int count = 0;

    if (this->_list_channel.empty())
        return (count);
    std::vector<Channel>::iterator it = this->_list_channel.begin();
    for (; it != this->_list_channel.end(); it++)
    {
        count++;
    }
    return (count);

}


