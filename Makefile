# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ipafnuto <ipafnuto@student.42nice.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/13 13:52:58 by ipafnuto          #+#    #+#              #
#    Updated: 2025/12/25 12:30:43 by ipafnuto         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DEFAULT		:= \033[0m
BOLD		:= \033[1m

BLUE		:= \033[34m
CYAN		:= \033[36m
GREEN		:= \033[32m
MAGEN		:= \033[35m
RED			:= \033[31m
YELL		:= \033[1;33m
CLEAR		:= \033c

DONE		:= ✅


NAME		:= vbc

SRC_DIR		:= src
SRC			:= main.c

SRC			:= $(SRC:%=$(SRC_DIR)/%)

INC			:= inc

#LIB		:= readline
#LIB_TARGET	:= .a

BUILD_DIR	:= build
OBJ			:= $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

CC			:= gcc
CFLAGS		:= -Wall -Wextra -Werror -g3
CPPFLAGS	:= $(INC:%=-I%)

LDFLAGS		:= $(addprefix -L,$(dir $(LIB_TARGET)))
LDLIBS		:= $(addprefix -l,$(LIB))

ARGS		?=

MAKEFLAGS	+= --silent --no-print-directory


all		: $(NAME)

$(LIB_TARGET):
	printf "%-55b" "Compiling $(@F)... "
	$(MAKE) -C $(@D) > /dev/null
	touch $@
	echo "$(GREEN)$(DONE)$(DEFAULT)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	printf "%-55b" \
		"Compiling $(BOLD)$(patsubst $(SRC_DIR)/%,%,$<)... $(DEFAULT)"
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<
	echo "$(GREEN)$(DONE)$(DEFAULT)"

$(NAME): $(LIB_TARGET) $(OBJ)
	printf "%-55b" \
		"Compiling $(BOLD)$@... $(DEFAULT)"
	$(CC) $(LDFLAGS) $(OBJ) $(LDLIBS) -o $(NAME)
	echo "$(GREEN)$(DONE)$(DEFAULT)"

bonus	: all

clean	:
	printf "%-60b" "$(RED)Deleting $(BOLD)objects... $(DEFAULT)"
	rm -rf $(BUILD_DIR)
	echo "$(GREEN)$(DONE)$(DEFAULT)"

fclean	: clean
	printf "%-60b" "$(RED)Deleting $(BOLD)executables... $(DEFAULT)"
	rm -f $(NAME)
	echo "$(GREEN)$(DONE)$(DEFAULT)"

re		: fclean all

run		: all
	./$(NAME) $(ARGS)

runv	: all
	valgrind \
		--leak-check=full --show-leak-kinds=all \
		./$(NAME) $(ARGS)

.PHONY: all bonus clean fclean re run runv
