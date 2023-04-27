NAME        := ircserv

CXX         := c++
CXXFLAGS    := -Wall -Wextra -Werror -std=c++98 -fsanitize=address

CPPFLAGS    :=
DEPFLAGS     = -MT $@ -MMD -MP -MF $(DDIR)/$*.d

LDFLAGS     :=
LDLIBS      :=

VPATH       := ./ src/
SRCS        := Main.cpp Channel.cpp Commands.cpp Server.cpp TranslateBNF.cpp User.cpp Utils.cpp

ODIR        := obj
OBJS        := $(SRCS:%.cpp=$(ODIR)/%.o)

DDIR        := $(ODIR)/.deps
DEPS        := $(SRCS:%.cpp=$(DDIR)/%.d)


$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(ODIR)/%.o: %.cpp $(DDIR)/%.d | $(ODIR) $(DDIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< -o $@

$(ODIR):
	mkdir -p $@

$(DDIR):
	mkdir -p $@

all: $(NAME)

clean:
	$(RM) -r $(DDIR) $(ODIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

$(DEPS):
-include $(DEPS)

ip:
	ipconfig getifaddr en1