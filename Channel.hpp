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
        //utilitaire
        int         in_list_client(std::string client);
        int         in_list_op_client(std::string client);
        void	    add_client(Client &client);
        void	    add_op_client(Client &client);
        void	    rm_perm(Client &client);
        void        rm_client(Client &client);
        int	        isChannelEmpty(void);

    private :
        std::string         _name;
        std::vector<Client> _list_client;
        std::vector<Client> _list_op_client;
        std::string         _passwrd;
        std::string         _topic;
        int                 _limit_client;
        bool                _restricted_topic;

};

#endif