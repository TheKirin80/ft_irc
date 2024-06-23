SRCS =	main.cpp\
		Client.cpp\
		Channel.cpp\
		Server.cpp\
		/COMMAND/CAP.cpp\
		/COMMAND/INVITE.cpp\
		/COMMAND/JOIN.cpp\
		/COMMAND/KICK.cpp\
		/COMMAND/MODE.cpp\
		/COMMAND/NICK.cpp\
		/COMMAND/PART.cpp\
		/COMMAND/PASS.cpp\
		/COMMAND/PRIVMSG.cpp\
		/COMMAND/QUIT.cpp\
		/COMMAND/TOPIC.cpp\
		/COMMAND/USER.cpp\

INC		= -I includes -I /usr/include

OBJS	= ${SRCS:.cpp=.o}

NAME 	= ircserv

CXX 		= c++

RM 		= rm -f

CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
# CXXFLAGS	+= -g3 -fsanitize=address,undefined,leak -Weffc++

all:		${NAME}

.cpp.o :
			$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

${NAME}:	${OBJS}
			$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:		
			${RM} ${OBJS} 

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re bonus