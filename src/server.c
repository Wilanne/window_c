/**
 * Programme principal : Systemo de pilotage du robot.
 *
 * \author Léo AUBRY
 */

#include <stdio.h>
#include <unistd.h>

#include "net/sockets.h"
#include "net/protocol.h"

int main() {
	int sockfd = create_socket();
	if (sockfd < 0) {
		fprintf(stderr, "Failed to create socket\n");
		return -1;
	}

	if (bind_socket(sockfd, 12345) < 0) {
		fprintf(stderr, "Failed to bind socket\n");
		return -1;
	}

	if (listen_socket(sockfd) < 0) {
		fprintf(stderr, "Failed to listen on socket\n");
		return -1;
	}

	int client_sockfd = accept_connection(sockfd);
	if (client_sockfd < 0) {
		fprintf(stderr, "Failed to accept connection\n");
		return -1;
	}

	msg_askMoving_t msg;
	msg.header.length = sizeof(msg);
	msg.header.type = MESSAGE_TYPE_ASK_MOVING;
	msg.direction = 42;
	msg.speed = 1337;

	uint8_t buffer[sizeof(msg)];
	serialize_msg_askMoving(&msg, buffer);

	for (size_t i = 0; i < sizeof(buffer); i++) {
		printf("%d ", buffer[i]);
	}
	printf("\n");

	ssize_t bytes_sent = send_data(client_sockfd, buffer, sizeof(buffer));
	if (bytes_sent < 0) {
		perror("send");
		return -1;
	}

	close(client_sockfd);
	close(sockfd);

	return 0;
}