NAME = webserv

SRC_DIR = srcs

BUILD_DIR = objs

SRCS = $(shell find $(SRC_DIR) -name '*.cpp' -not -path "$(SRC_DIR)/.git/*")

OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIR) -type d)

INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CXX = g++

CPPFLAGS := -Iincludes $(INC_FLAGS) -MMD -MP

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic

RM = rm

all : $(NAME)

$(NAME)	:	$(OBJS)
	@echo "Creating $(NAME)"
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean :
	@$(RM) -rf $(BUILD_DIR)

fclean : clean
	@$(RM) -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re

-include $(DEPS)
