# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oishchen <oishchen@student.42heilbronn.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/16 19:52:14 by oishchen          #+#    #+#              #
#    Updated: 2025/04/20 15:16:02 by oishchen         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Declaration: pass to the headers
MIX_HEADERS = $(MIX_LIB_NAME)/printf/includes/ $(MIX_LIB_NAME)/libft/includes/
OWN_HEADER = include/

#Declaration: flags, compiler, rm
CC = cc
CFLAGS = -Wextra -Werror -Wall \
		-I$(OWN_HEADER) \
		$(addprefix -I, $(MIX_HEADERS))
RM = rm -rf

#Declaration: remote library, libraries's name, FOLDERS names
MIX_LIB_REPO = https://github.com/Zirael-leariZ/42_lib_mix
MIX_LIB_NAME = mixlibft
SERVER = server
CLIENT = client
SRC_DIR = src
OBJ_DIR = obj

#Declaration: BINARIES, .c files
MIX_LIB_A = $(MIX_LIB_NAME)/mixlibft.a
CLIENT_FILE = client.c
SERVER_FILE = server.c
CLIENT_FULLPASS = $(SRC_DIR)/$(CLIENT_FILE)
SERVER_FULLPASS = $(SRC_DIR)/$(SERVER_FILE)

#Rule: Gathers all .c into obj/%.o
CLIENT_FULL_OBJ=$(OBJ_DIR)/$(CLIENT_FILE:.c=.o)
SERVER_FULL_OBJ=$(OBJ_DIR)/$(SERVER_FILE:.c=.o)

#Command: all
all: $(CLIENT) $(SERVER) $(MIX_LIB_A)

$(MIX_LIB_NAME):
	@if [ ! -d $(MIX_LIB_NAME) ]; then \
		git clone $(MIX_LIB_REPO) $(MIX_LIB_NAME); \
		echo "CLONNING REPOSITROY"; \
	fi \

#Dpendency + rule: Tells us that any changes in the mixlibft should not evoke relink in .a
$(MIX_LIB_A): | $(MIX_LIB_NAME)

#Rule: How mixlibft should be created
$(MIX_LIB_A): 
	@if [ ! -f $(MIX_LIB_A) ]; then \
		echo "BULIDING A LIBRARY"; \
		make -C $(MIX_LIB_NAME); \
	else \
		echo "THE REPOSITORY EXISTS"; \
	fi

#Command: create obj dir in the project folder
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

#Rules: how to create the binaries: client, server.
$(CLIENT): $(CLIENT_FULL_OBJ) $(MIX_LIB_A) | $(MIX_LIB_NAME) $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(MIX_LIB_A) $(CLIENT_FULL_OBJ) -o $(CLIENT)
	@echo "CLIENT executable is ready"
	
$(SERVER): $(SERVER_FULL_OBJ) $(MIX_LIB_A) | $(MIX_LIB_NAME) $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(MIX_LIB_A) $(SERVER_FULL_OBJ) -o $(SERVER)
	@echo "SERVER executable is ready"

#Rule: how to compile .c -> .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(MIX_LIB_NAME)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "CONVERSION COMPLETED SRC -> OBJ"

#Default commands:
clean:
	@$(RM) $(OBJ_DIR)
	@make clean -C $(MIX_LIB_NAME)
	@echo "CLEAN ALL THE OJECTS"

fclean: clean
	@$(RM) $(CLIENT) $(SERVER)
	@make fclean -C $(MIX_LIB_NAME)
	@echo "Everyting was cleaned"

re: fclean all

.PHONY: all clean fclean re 