NAME		= 	ircserv
CC 			= 	c++
CPPFLAGS	= 	-Wall -Wextra -Werror -std=c++98
SRC 		=	Channel.cpp	\
				Main.cpp	\
				Server.cpp	\
				Connection.cpp \
				commands.cpp
HEADER		=	./Headers/Channel.hpp	\
				./Headers/Server.hpp	\
				./Headers/Utils.hpp		\
				./Headers/Connection.hpp\
				./Headers/commands.h
			
OBJ = $(SRC:%.cpp=%.o)
all: $(NAME)
$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
clean:
	@rm -f $(OBJ)
fclean: clean
	@rm -f $(NAME)
re: fclean all