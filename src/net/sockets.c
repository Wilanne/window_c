// Author : Leo AUBRY

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "sockets.h"

int create_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    return sockfd;
}

int bind_socket(int sockfd, int port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return -1;
    }
    return 0;
}

int listen_socket(int sockfd) {
    if (listen(sockfd, 5) < 0) {
        perror("listen");
        return -1;
    }
    return 0;
}

int accept_connection(int sockfd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sockfd < 0) {
        perror("accept");
        return -1;
    }
    return client_sockfd;
}

int connect_socket(int sockfd, const char *address, int port) {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, address, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return -1;
    }
    return 0;
}

ssize_t send_data(int sockfd, const uint8_t *data, size_t length) {
    return send(sockfd, data, length, 0);
}

ssize_t receive_data(int sockfd, uint8_t *buffer, size_t length) {
    return recv(sockfd, buffer, length, 0);
}
