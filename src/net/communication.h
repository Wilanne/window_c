// Author : Léo AUBRY

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

#include "protocol.h"
#include "sockets.h"

/**
 * Starts a new thread to receive data from the specified socket.
 *
 * @param sockfd The socket file descriptor to receive data from.
 * @param callback A function pointer to the callback function that will be called when data is received.
 *                 The callback function should have the following signature: void callback(const uint8_t *data, size_t length).
 * @return The thread ID of the newly created thread.
 */
pthread_t start_receive_thread(int sockfd, void (*callback)(const uint8_t *data, size_t length));

/**
 * Establishes a pending communication on the specified port.
 *
 * @param port The port number to establish the communication on.
 * @return The socket file descriptor for the established communication.
 */
int establish_pending_communication(int port);

/**
 * Establishes a client connection to the specified address and port.
 *
 * @param address The address to connect to.
 * @param port The port number to connect to.
 * @return The socket file descriptor for the established connection.
 */
int establish_client_connection(const char *address, int port);

/**
 * Sends a message through the specified socket.
 *
 * @param sockfd The socket file descriptor to send the message through.
 * @param msg A pointer to the message to send.
 * @return The number of bytes sent, or -1 if an error occurred.
 */
ssize_t send_message(int sockfd, const message_t *msg);

/**
 * Stops the receive thread with the specified thread ID.
 *
 * @param thread_id The thread ID of the receive thread to stop.
 */
void stop_receive_thread(pthread_t thread_id);

#endif // COMMUNICATION_H
