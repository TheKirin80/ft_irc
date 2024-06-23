DOC_COMMAND		= COMMAND
FILES_COMMAND = CAP.cpp\
				INVITE.cpp\
				JOIN.cpp\
				KICK.cpp\
				MODE.cpp\
				NICK.cpp\
				PART.cpp\
				PASS.cpp\
				PRIVMSG.cpp\
				QUIT.cpp\
				TOPIC.cpp\
				USER.cpp
SRCS =			main.cpp\
				Client.cpp\
				Channel.cpp\
				Server.cpp\
				loop.cpp

INC		= -I includes -I /usr/include

SRCS_COMMAND	= $(addprefix $(DOC_COMMAND)/, $(FILES_COMMAND))
OBJ_COMMAND = ${SRCS_COMMAND:.cpp=.o}
OBJ	= ${SRCS:.cpp=.o}
OBJS = $(OBJ_COMMAND) $(OBJ)
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