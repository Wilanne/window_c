// Author : Léo AUBRY

#include "protocol.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sockets.h"
#include "communication.h"

typedef struct {
    int sockfd;
    void (*callback)(const uint8_t *data, size_t length);
} receive_thread_args_t;

void *receive_thread(void *args) {
    receive_thread_args_t *r_args = (receive_thread_args_t *)args;
    int sockfd = r_args->sockfd;
    void (*callback)(const uint8_t *data, size_t length) = r_args->callback;
    uint8_t buffer[300];
    ssize_t bytes_received;

    while ((bytes_received = receive_data(sockfd, buffer, sizeof(buffer))) > 0) {
        callback(buffer, bytes_received);
    }

    if (bytes_received < 0) {
        perror("recv");
    }

    free(r_args);
    return NULL;
}

pthread_t start_receive_thread(int sockfd, void (*callback)(const uint8_t *data, size_t length)) {
    pthread_t thread_id;
    receive_thread_args_t *args = malloc(sizeof(receive_thread_args_t));
    if (!args) {
        perror("malloc");
        return -1;
    }
    args->sockfd = sockfd;
    args->callback = callback;

    if (pthread_create(&thread_id, NULL, receive_thread, args) != 0) {
        perror("pthread_create");
        free(args);
        return -1;
    }
    return thread_id;
}

int establish_pending_communication(int port) {
    int sockfd = create_socket();
    if (sockfd < 0) {
        fprintf(stderr, "Failed to create socket\n");
        return -1;
    }

    if (bind_socket(sockfd, port) < 0) {
        fprintf(stderr, "Failed to bind socket on port %d\n", port);
        close(sockfd);
        return -1;
    }

    if (listen_socket(sockfd) < 0) {
        fprintf(stderr, "Failed to listen on port %d\n", port);
        close(sockfd);
        return -1;
    }

    printf("Server is listening on port %d\n", port);

    return sockfd;
}

int establish_client_connection(const char *address, int port) {
    int sockfd = create_socket();
    if (sockfd < 0) {
        fprintf(stderr, "Failed to create socket\n");
        return -1;
    }

    if (connect_socket(sockfd, address, port) < 0) {
        fprintf(stderr, "Failed to connect to server on port %d\n", port);
        close(sockfd);
        return -1;
    }
    return sockfd;
}

ssize_t send_message(int sockfd, const message_t *msg) {
    uint8_t buffer[300];
    serialize_message(msg, buffer);
    size_t length = msg->header.length + sizeof(msg->header.length) + sizeof(msg->header.type);
    return send_data(sockfd, buffer, length);
}

void stop_receive_thread(pthread_t thread_id) {
    pthread_cancel(thread_id);
    pthread_join(thread_id, NULL);
}
