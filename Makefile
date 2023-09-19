NAME = webserv

SRCS_DIR = ./srcs

BUILD_DIR = ./objs

SRCS = $(shell find $(SRC_DIRS) -name '*.cpp')

OBJS = $(SRCS:.cpp=.o)

# OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# DEPS

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -Iincludes 

RM = rm

all : $(NAME)

$(NAME)	:	$(OBJS)
	@echo "Creating $(NAME)"
	@$(CXX) $(CXXFLAGS) -o $@ $^

clean :
	@$(RM) -rf $(OBJS)

fclean : clean
	@$(RM) -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re test
