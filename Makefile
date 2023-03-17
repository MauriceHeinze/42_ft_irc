NAME		= 	ircserv
CC 			= 	c++
CPPFLAGS	= 	-Wall -Wextra -Werror -std=c++98
SRC 		=	Main.cpp	\
				Helper.cpp
HEADER		=	Helper.hpp

OBJ = $(SRC:%.cpp=%.o)
all: $(NAME)
$(NAME): $(OBJ) $(HEADER)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
clean:
	@rm -f $(OBJ)
fclean: clean
	@rm -f $(NAME)
re: fclean all