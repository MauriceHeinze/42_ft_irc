NAME		= 	ircserv
CC 			= 	c++
CPPFLAGS	= 	-Wall -Wextra -Werror -std=c++98
SRC 		=	Channel.cpp			\
				Commands.cpp		\
				Main.cpp			\
				Server.cpp			\
				User.cpp			\
				Utils.cpp			\
				BNF_Translate.cpp

HEADER		=	./Headers/Channel.hpp		\
				./Headers/Commands.hpp		\
				./Headers/Server.hpp		\
				./Headers/User.hpp			\
				./Headers/Utils.hpp			\
				./Headers/BNF_Translate.hpp

OBJ = $(SRC:%.cpp=%.o)
all: $(NAME)
$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
clean:
	@rm -f $(OBJ)
fclean: clean
	@rm -f $(NAME)
re: fclean all

ip:
	ipconfig getifaddr en1