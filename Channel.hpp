#ifndef Channel_HPP
#define Channel_HPP

#include "libIRC.hpp"

class Client;

class Channel
{
    public :
        Channel(std::string name);
        ~Channel(void);
        //getset
        std::string getName(void) const;
        void        setName(std::string str);
        std::string getPasswrd(void) const;
        void        setPasswrd(std::string str);
        std::string getTopic(void) const;
        void        setTopic(std::string str);
        int         getLimitClient(void) const;
        void        setLimitClient(int i);
        bool        getRestrictedTopicState(void) const;
        void        setRestrictedTopicTrue(void);
        void        setRestrictedTopicFalse(void);
        bool        getOnlyInviteState(void) const;
        void        setOnlyInviteTrue(void);
        void        setOnlyInviteFalse(void);
        //utilitaire
        int         in_list_client(std::string client);
        int         in_list_op_client(std::string client);
        int         in_list_invite_client(std::string client);
        void	    add_client(Client &client);
        void	    add_op_client(Client &client);
        void	    add_invite_client(Client &client);
        void	    rm_perm(Client &client);
        void	    rm_invite(Client &client);
        void        rm_client(Client &client);
        int	        isChannelEmpty(void);
        void        rm_passwrd(void);
        int         count_client(void);
        std::string	getNicknameOfListClient(void);
        void        sendRepMessageChannel(int fd, std::string to_send);
        void        replyToAll(std::string reply);
        void        replyToAllButNotMe(std::string reply, int fd);
        void        replyToAllTopic(std::string origin, std::string serv_name, std::string topic);

    private :
        std::string         _name;
        std::string         _passwrd;
        std::string         _topic;
        int                 _limit_client;
        bool                _restricted_topic;
        bool                _only_invite;
        std::vector<Client> _list_client;
        std::vector<Client> _list_op_client;
        std::vector<Client> _list_invite_client;

};

#endif