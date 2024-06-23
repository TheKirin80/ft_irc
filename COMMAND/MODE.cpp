#include "../libIRC.hpp"

// static int	check_kol(std::string params){
// 	int	how_many = 0;
	
// 	for (size_t i = 1; i < params.size(); i++)
// 		if (params[i] == 'o' || params[i] == 'k' || params[i] == 'l')
// 			how_many++;
// 	if (how_many >= 2)
// 		return EXIT_FAILURE;
// 	return (EXIT_SUCCESS);
// }

// static std::string get_param_mods(std::string params){
// 	std::string mods;
// 	size_t i = 0;
// 	for (; i < params.size(); i++){
// 		if (params[i] == ' ')
// 			break ;
// 	}
// 	i++;
// 	if (params[i]){
// 		for (size_t j = i; j < params.size(); j++)
// 			mods += params[j];
// 	}
// 	return mods;
// }

static int  checkFlag(std::vector<std::string> &list_param)
{
    if (list_param.at(1).size() != 2)
        return (ERROR);
    if (list_param.at(1).at(0) != '+' && list_param.at(1).at(0) != '-')
        return (ERROR);
    if (list_param.at(1).find_first_not_of("itkol", 1))
        return (ERROR);
    // if (list_param.at(1).at(1) != 'i' && list_param.at(1).at(1) != 't' && list_param.at(1).at(1) != 'k' && list_param.at(1).at(1) != 'o' && list_param.at(1).at(1) != 'l')
    //     return (ERROR)
    return (OK);
}



void	Server::MODE(int fd, std::string param) 
{
	std::vector<std::string> list_param;
    Client client = this->getClientWithFd(fd);

	if (param.empty())
    {
		return ;
	}
	list_param = this->multi_split(param, " ");
	if (list_param.size() < 2)
    {
        //ERR_NEEDMOREPARAMS
		return ;
	}
    if (checkFlag(list_param) == ERROR)
    {
        //ERROR UnknowCommand
        return;
    }
	if (this->inListChannelServ(list_param.at(0)) == ERROR) 
    {
        //ERR_NOSUCHCHANNEL
		return ;
	}
    if (this->getChannelWithName(list_param.at(0)).in_list_client(client.getNickname()) == ERROR)
    {
        //ERR_USERNOTINCHANNEL
		return ;
	}
	if (this->getChannelWithName(list_param.at(0)).in_list_op_client(client.getNickname()) == ERROR)
    {
        //ERR_CHANOPRIVSNEEDED
		return ;
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
                //ERR_KEYSET
                return ;
            }
        }
        if ((*it).at(1) == 'o')
        {
            if (list_param.size() < 3)
                return ;
            if (this->inListClientServ(list_param.at(2)) == ERROR)
            {
                //ERR_USERNOTINCHANNEL
                return ;
            }
            if (this->getChannelWithName(list_param.at(0)).in_list_client(list_param.at(2)) == ERROR) // On verifie que le futur operateur fait partie du channel
            {
                //ERR_USERNOTINCHANNEL
                return ;
            }
            if (this->getChannelWithName(list_param.at(0)).in_list_op_client(list_param.at(2)) == OK) // Pas deja present
            {
                //RPL_UNIQOPIS
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
            if (isdigit(list_param.at(2).at(0)) == 0 || isdigit(list_param.at(2).at(1)) == 0)
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
}

// ERR_NEEDMOREPARAMS pour le +k
// ERR_KEYSET pour le +k, il y a deja un mdp
// ERR_CHANOPRIVSNEEDED
// ERR_USERNOTINCHANNEL
// ERR_UNKNOWNMODE

// RPL_CHANNELMODEIS
// RPL_UNIQOPIS	pour le +o, utilisateur deja operateur

// ! faire les modes +-ikot
//	i pour inviteonly
//	t pour restreindre ou non acces a la commande topic
//	k pour ajouter un mdp au channel
//	o pour creer un nouvel operateur chan
//	l pour limiter le nombre de user d un chan