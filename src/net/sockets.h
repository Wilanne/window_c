// Author : Leo AUBRY

#ifndef SOCKETS_H
#define SOCKETS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * Creates a socket.
 *
 * @return The file descriptor of the created socket, or -1 if an error occurred.
 */
int create_socket();

/**
 * Binds a socket to a specific port.
 *
 * @param sockfd The file descriptor of the socket.
 * @param port The port number to bind the socket to.
 * @return 0 if successful, or -1 if an error occurred.
 */
int bind_socket(int sockfd, int port);

/**
 * Listens for incoming connections on a socket.
 *
 * @param sockfd The file descriptor of the socket.
 * @return 0 if successful, or -1 if an error occurred.
 */
int listen_socket(int sockfd);

/**
 * Accepts an incoming connection on a socket.
 *
 * @param sockfd The file descriptor of the socket.
 * @return The file descriptor of the accepted connection, or -1 if an error occurred.
 */
int accept_connection(int sockfd);

/**
 * Connects a socket to a remote address and port.
 *
 * @param sockfd The file descriptor of the socket.
 * @param address The remote address to connect to.
 * @param port The remote port to connect to.
 * @return 0 if successful, or -1 if an error occurred.
 */
int connect_socket(int sockfd, const char *address, int port);

/**
 * Sends data over a socket.
 *
 * @param sockfd The file descriptor of the socket.
 * @param data The data to send.
 * @param length The length of the data to send.
 * @return The number of bytes sent, or -1 if an error occurred.
 */
ssize_t send_data(int sockfd, const uint8_t *data, size_t length);

/**
 * Receives data from a socket.
 *
 * @param sockfd The file descriptor of the socket.
 * @param buffer The buffer to store the received data.
 * @param length The length of the buffer.
 * @return The number of bytes received, or -1 if an error occurred.
 */
ssize_t receive_data(int sockfd, uint8_t *buffer, size_t length);

#endif // SOCKETS_H
