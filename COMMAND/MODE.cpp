#include "../libIRC.hpp"

static int  checkFlag(std::vector<std::string> &list_param)
{
    if (list_param.at(1).size() != 2)
        return (ERROR);
    if (list_param.at(1).at(0) != '+' && list_param.at(1).at(0) != '-')
        return (ERROR);
    if (list_param.at(1).at(1) != 'i' && list_param.at(1).at(1) != 't' && list_param.at(1).at(1) != 'k' && list_param.at(1).at(1) != 'o' && list_param.at(1).at(1) != 'l')
        return (ERROR);
    std::cout << RED << "4 "<< list_param.at(1) << std::endl;
    return (OK);
}
static int  isStringDigit(std::string str){
    for (size_t i = 0; i < str.size(); i++){
        if (!isdigit(str[i]))
            return (ERROR);
    }
    return (OK);
}

void	Server::MODE(int fd, std::string param) 
{
	std::vector<std::string> list_param;
    Client client = this->getClientWithFd(fd);

    if (client.getUserCheckState() == false)
    {
		this->sendErrMessage(fd, "USER not registered yet so command rejected\r\n");
		return ;
	}
	if (param.empty())
    {
        this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "MODE"));
		return ;
	}
	list_param = this->multi_split(param, " ");
    if (this->inListClientServ(list_param.at(0)) == OK){
        return;
    }
	if (this->inListChannelServ(list_param.at(0)) == ERROR) 
    {
        this->sendErrMessage(fd, ERR_NOSUCHCHANNEL(this->_name, client.getNickname(), list_param.at(0)));
		return ;
	}
	if (this->getChannelWithName(list_param.at(0)).in_list_op_client(client.getNickname()) == ERROR)
    {
        this->sendErrMessage(fd, ERR_CHANOPRIVSNEEDED(this->_name, client.getNickname(), list_param.at(0)));
		return ;
	}
	if (list_param.size() == 1)
    {
        this->sendRepMessage(fd, RPL_CHANNELMODIS(this->_name, client.getNickname(), list_param.at(0), this->channelModeIs(list_param.at(0))));
		return ;
	}
    if (checkFlag(list_param) == ERROR)
    {
        this->sendErrMessage(fd, ERR_UNKNOWNMODE(this->_name, client.getNickname(), list_param.at(1), list_param.at(0)));
        return;
    }
    std::vector<std::string>::iterator it = list_param.begin() + 1;
	if ((*it).at(0) == '+')
    {
        if ((*it).at(1) == 'i')
        {
            if (this->getChannelWithName(list_param.at(0)).getOnlyInviteState()== true)
                return ;
            else
                this->getChannelWithName(list_param.at(0)).setOnlyInviteTrue();
        }
        if ((*it).at(1) == 't')
        {
            if (this->getChannelWithName(list_param.at(0)).getRestrictedTopicState() == true)
                return ;
            else
                this->getChannelWithName(list_param.at(0)).setRestrictedTopicTrue();
        }
        if ((*it).at(1) == 'k')
        {
            if (list_param.size() < 3)
                return ;
            if (this->getChannelWithName(list_param.at(0)).getPasswrd().empty())
            {
                this->getChannelWithName(list_param.at(0)).setPasswrd(list_param.at(2));
            }
            else
            {
                this->sendErrMessage(fd, ERR_KEYSET(this->_name, client.getNickname(), list_param.at(0)));
                return ;
            }
        }
        if ((*it).at(1) == 'o')
        {
            if (list_param.size() < 3)
                return ;
            if (this->inListClientServ(list_param.at(2)) == ERROR)
            {
                this->sendErrMessage(fd, ERR_USERNOTINCHANNEL(this->_name, client.getNickname(), list_param.at(2), list_param.at(0)));
                return ;
            }
            if (this->getChannelWithName(list_param.at(0)).in_list_client(list_param.at(2)) == ERROR) // On verifie que le futur operateur fait partie du channel
            {
                this->sendErrMessage(fd, ERR_USERNOTINCHANNEL(this->_name, client.getNickname(), list_param.at(2), list_param.at(0)));
                return ;
            }
            if (this->getChannelWithName(list_param.at(0)).in_list_op_client(list_param.at(2)) == OK) // Pas deja present
            {
                this->sendRepMessage(fd, RPL_UNIQOPIS(this->_name, client.getNickname(), list_param.at(0), list_param.at(2)));
                return ;
            }
            else
                this->getChannelWithName(list_param.at(0)).add_op_client(this->getClientWithName(list_param.at(2)));
        }
        if ((*it).at(1) == 'l')
        {
            if (list_param.size() < 3)
            {
                return ;
            }
            if (list_param[2].size() > 2)
            {
                return ;
            }
            if (isStringDigit(list_param[2]) == ERROR)
            {
                return;
            }
            this->getChannelWithName(list_param.at(0)).setLimitClient(atoi(list_param[2].c_str()));
        }
	}
	else
    {
        if ((*it).at(1) == 'i')
        {
            if (this->getChannelWithName(list_param.at(0)).getOnlyInviteState() == false)
                return ;
            else
                this->getChannelWithName(list_param.at(0)).setOnlyInviteFalse();
        }
        if ((*it).at(1) == 't')
        {
            if (this->getChannelWithName(list_param.at(0)).getRestrictedTopicState() == false)
                return ;
            else
                this->getChannelWithName(list_param.at(0)).setRestrictedTopicFalse();
        }
        if ((*it).at(1) == 'k')
            this->getChannelWithName(list_param.at(0)).rm_passwrd();
        if ((*it).at(1) == 'o')
        {
            if (list_param.size() < 3)
                return ;
            if (this->getChannelWithName(list_param.at(0)).in_list_client(list_param.at(2)) == ERROR)
                return ;
            if (this->getChannelWithName(list_param.at(0)).in_list_op_client(list_param.at(2)) == OK){
                this->getChannelWithName(list_param.at(0)).rm_perm(this->getClientWithName(list_param.at(2)));
            }
        }
        if ((*it).at(1) == 'l')
        {
            this->getChannelWithName(list_param.at(0)).setLimitClient(0);
        }
	}
    if (list_param.size() == 3)
        this->sendRepMessage(fd, MODE_INFO(client.getNickname(), client.getUsername(), this->_name, list_param.at(0), " " + list_param.at(2)));
    else
        this->sendRepMessage(fd, MODE_INFO(client.getNickname(), client.getUsername(), this->_name, list_param.at(0), ""));
}

