# Define variables
CC = gcc
CFLAGS = -I src/net 
CFLAGS += -Wall -Wextra -Werror 
CFLAGS += -std=c11 
CFLAGS += -O2 
CFLAGS += -g
# CFLAGS += -fsanitize=address -fsanitize=undefined -pthread

SRC_DIR = src
NET_DIR = $(SRC_DIR)/net
BIN_DIR = bin

# Source files
CLIENT_SRC = $(SRC_DIR)/client.c $(wildcard $(NET_DIR)/*.c)
SERVER_SRC = $(SRC_DIR)/server.c $(wildcard $(NET_DIR)/*.c)

# Output files
CLIENT_OUT = $(BIN_DIR)/client
SERVER_OUT = $(BIN_DIR)/server

# Create the bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Rules
all: $(CLIENT_OUT) $(SERVER_OUT)

$(CLIENT_OUT): $(CLIENT_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT_OUT)

$(SERVER_OUT): $(SERVER_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER_OUT)

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
