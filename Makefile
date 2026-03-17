# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gudos-sa <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/17 15:22:24 by gudos-sa          #+#    #+#              #
#    Updated: 2026/01/17 15:22:30 by gudos-sa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
NAME = IRC
SRCS = main.cpp Server.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)
HEADERS = Server.hpp utils.hpp

all: $(NAME)
%.o:%.cpp $(HEADERS)
	@$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)
	@clear
	@echo "IRC SERVER compiled."

clean:
	@rm -f $(OBJS)
	@clear
	@echo "Objects cleaned."

fclean: clean
	@rm -f $(NAME)
	@clear
	@echo "All cleaned"


re: fclean all

PHONY: re fclean clean all

