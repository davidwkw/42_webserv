NAME = webserv

SRCS_DIR = srcs

SRCS = $(shell find . -name "*.cpp")

OBJS = $(SRCS:.cpp=.o)

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -Iincludes


all : $(NAME)

$(NAME)	:	$(OBJS)
	@echo "Creating $(NAME)"
	@$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	@rm -rf $(OBJS)

fclean : clean
	@rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re test
