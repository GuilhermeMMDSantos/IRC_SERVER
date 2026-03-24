CC      = c++
CFLAGS  = -Wall -Wextra -Werror -std=c++98 -g
DEBUG ?= 0
ifeq ($(DEBUG),1)
	CFLAGS += -g -DDEBUG
endif
NAME	= ircserv

SRC_DIR = srcs
OBJ_DIR = objs

SRCS = \
	$(SRC_DIR)/main.cpp \
	$(SRC_DIR)/server/Server.cpp \
	$(SRC_DIR)/socket/Socket.cpp \
	$(SRC_DIR)/client/Client.cpp \
	$(SRC_DIR)/client/ClientConnection.cpp \
	$(SRC_DIR)/client/ClientIdentity.cpp \
	$(SRC_DIR)/client/ClientManager.cpp \
	$(SRC_DIR)/client/ClientState.cpp \
	$(SRC_DIR)/Message.cpp \
	$(SRC_DIR)/Executor.cpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

INCLUDES = -I$(SRC_DIR) \
	-I$(SRC_DIR)/server \
	-I$(SRC_DIR)/socket \
	-I$(SRC_DIR)/client

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "IRC SERVER compiled."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Objects cleaned."

fclean: clean
	@rm -f $(NAME)
	@echo "All cleaned."

re: fclean all

.PHONY: all clean fclean re

