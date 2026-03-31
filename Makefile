CC      = c++
CFLAGS  = -Wall -Wextra -Werror -std=c++98 -g
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
	$(SRC_DIR)/channel/Mode.cpp \
	$(SRC_DIR)/channel/Channel.cpp \
	$(SRC_DIR)/channel/ChannelManager.cpp \
	$(SRC_DIR)/Message.cpp \
	$(SRC_DIR)/Executor.cpp \
	$(SRC_DIR)/commands/cap.cpp \
	$(SRC_DIR)/commands/join.cpp \
	$(SRC_DIR)/commands/mode.cpp \
	$(SRC_DIR)/commands/nick.cpp \
	$(SRC_DIR)/commands/pass.cpp \
	$(SRC_DIR)/commands/privmsg.cpp \
	$(SRC_DIR)/commands/ping.cpp \
	$(SRC_DIR)/commands/kick.cpp \
	$(SRC_DIR)/commands/topic.cpp \
	$(SRC_DIR)/commands/invite.cpp \
	$(SRC_DIR)/commands/user.cpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

HDRS = \
	$(SRC_DIR)/Executor.hpp \
	$(SRC_DIR)/Message.hpp \
	$(SRC_DIR)/channel/Channel.hpp \
	$(SRC_DIR)/channel/ChannelManager.hpp \
	$(SRC_DIR)/channel/Mode.hpp \
	$(SRC_DIR)/client/Client.hpp \
	$(SRC_DIR)/client/ClientConnection.hpp \
	$(SRC_DIR)/client/ClientIdentity.hpp \
	$(SRC_DIR)/client/ClientManager.hpp \
	$(SRC_DIR)/client/ClientState.hpp \
	$(SRC_DIR)/server/Server.hpp \
	$(SRC_DIR)/socket/Socket.hpp \
	$(SRC_DIR)/util/Util.hpp

INCLUDES = -I$(SRC_DIR) \
	-I$(SRC_DIR)/util \
	-I$(SRC_DIR)/channel \
	-I$(SRC_DIR)/server \
	-I$(SRC_DIR)/socket \
	-I$(SRC_DIR)/client \
	-I$(SRC_DIR)/channel

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "IRC SERVER compiled."

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HDRS)
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

