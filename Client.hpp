#ifndef Client_HPP
#define Client_HPP

#include "libIRC.hpp"

class Channel;

class Client
{
    public :
        Client(int fd);
        ~Client(void);
        std::string _buff;
        //getset
        std::string getUsername(void) const;
        void        setUsername(std::string str);
        std::string getNickname(void) const;
        void        setNickname(std::string str);
        std::string getHostname(void) const;
        void        setHostname(std::string str);
        std::string getRealname(void) const;
        void        setRealname(std::string str);
        int         getFd(void) const; //Pas de set car inchangeable apres instanciation
        Channel	    &getChannel(std::string chanel);
        bool        getPassCheckState(void) const;
        void        setPassCheckTrue(void);
        void        setPassCheckFalse(void);
        bool        getNickCheckState(void) const;
        void        setNickCheckTrue(void);
        void        setNickCheckFalse(void);
        bool        getUserCheckState(void) const;
        void        setUserCheckTrue(void);
        void        setUserCheckFalse(void);

        //utilitaire
        void        add_channel(Channel &newChan);
        void        rm_channel(std::string chanel);
        void        rm_all_channel(void);
        std::string	list_channel(void);
        int	        in_channel(std::string chanel);
        int         countChannel(void);

    private :
        std::string _username;
        std::string _nickname;
        std::string _hostname;
        std::string _realname;
        int         _fd;
        std::vector<Channel> _list_channel;
        bool        _pass_check;
        bool        _nick_check;
        bool        _user_check;

};

#endif