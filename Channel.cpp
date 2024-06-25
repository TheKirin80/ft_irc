#include "libIRC.hpp"

Channel::Channel(std::string name) : _name(name), _passwrd(""), _topic(""), _limit_client(0), _restricted_topic(false), _only_invite(false), _list_client(), _list_op_client()
{
    return;
}

Channel::~Channel(void)
{
    this->_list_client.clear();
    this->_list_op_client.clear();
    return;
}
// _name
std::string Channel::getName(void) const
{
    return (this->_name);
}

void Channel::setName(std::string str)
{
    this->_name = str;
}
// _passwrd
std::string Channel::getPasswrd(void) const
{
    return (this->_passwrd);
}

void Channel::setPasswrd(std::string str)
{
    this->_passwrd = str;
}
// _topic
std::string Channel::getTopic(void) const
{
    return (this->_topic);
}

void Channel::setTopic(std::string str)
{
    if (str == ""){
		this->_topic.clear();
		return ;
	}
    this->_topic = str;
}
// _limit_client
int Channel::getLimitClient(void) const
{
    return (this->_limit_client);
}

void Channel::setLimitClient(int i)
{
    this->_limit_client = i;
}
//_restricted_topic
bool Channel::getRestrictedTopicState(void) const
{
    return (this->_restricted_topic);
}

void Channel::setRestrictedTopicTrue(void)
{
    this->_restricted_topic = true;
}
void Channel::setRestrictedTopicFalse(void)
{
    this->_restricted_topic = false;
}

//_only_invite
bool Channel::getOnlyInviteState(void) const
{
    return (this->_only_invite);
}

void Channel::setOnlyInviteTrue(void)
{
    this->_only_invite = true;
}
void Channel::setOnlyInviteFalse(void)
{
    this->_only_invite = false;
}

//Utilitaires
int Channel::in_list_client(std::string client)
{
    for(std::vector<Client>::iterator it = this->_list_client.begin(); it != this->_list_client.end();it++)
        if ((*it).getNickname() == client)
            return (OK);
    return (ERROR);
}
int Channel::in_list_op_client(std::string client)
{
    for(std::vector<Client>::iterator it = this->_list_op_client.begin(); it != this->_list_op_client.end();it++)
        if ((*it).getNickname() == client)
            return (OK);
    return (ERROR);
}

void	Channel::add_client(Client &client)
{
	this->_list_client.push_back(client);
}

void	Channel::add_op_client(Client &client)
{
	this->_list_op_client.push_back(client);
}
void	Channel::rm_perm(Client &client)
{
	std::vector<Client>::iterator it = this->_list_op_client.begin();
	for (; it != this->_list_op_client.end(); it++){
		if ((*it).getNickname() == client.getNickname())
        {
			this->_list_op_client.erase(it);
            return;
        }
	}
}

void    Channel::rm_client(Client &client)
{
    std::vector<Client>::iterator it = this->_list_client.begin();

    for (; it != this->_list_client.end(); it++){
		if ((*it).getNickname() == client.getNickname())
        {
            rm_perm(client);
			this->_list_client.erase(it);
            return;
        }
	}
}
int	Channel::isChannelEmpty(void)
{
	if (this->_list_client.empty())
		return OK;
	return ERROR;
}

void Channel::rm_passwrd(void)
{
	this->_passwrd.clear();
}
int Channel::count_client(void)
{
    std::vector<Client>::iterator it = this->_list_client.begin();
    int count = 0;
    for (; it != this->_list_client.end(); it++)
    {
        count++;
    }
    return (count);
}
std::string	Channel::getNicknameOfListClient(void)
{
	std::string	nicknames;
    std::vector<Client>::iterator it = this->_list_client.begin();

	for (; it != this->_list_client.end(); it++)
    {
        std::string nick = (*it).getNickname();
		if (it == this->_list_client.begin())
        {
			nicknames = "@"+ nick + " ";
		}
		else
        {
			if (this->in_list_op_client(nick) == OK)
				nicknames += "@";
			nicknames += nick +" ";
		}
	}
	nicknames.erase(nicknames.end()-1);
	return nicknames;
}
void Channel::sendRepMessageChannel(int fd, std::string to_send)
{
	if(send(fd, to_send.c_str(), to_send.size(), 0) == -1)
		std::cerr << RED << "failed to use sendRepMessage with this fd : "<< fd << RESET << std::endl;
}
void Channel::replyToAll(std::string reply)
{
	std::vector<Client>::iterator it = _list_client.begin();
    for (;it != this->_list_client.end(); it++)
    {
        sendRepMessageChannel((*it).getFd(), reply);
    }
}
