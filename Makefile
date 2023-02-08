CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic

SRCS = main.cpp

OBJS = $(SRCS:.cpp=.o)

NAME = webserv

all : $(NAME)

$(NAME)	:	$(OBJS)
	@echo "Creating $(NAME)"
	@$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	@rm -rf $(OBJS)

fclean : clean
	@rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
