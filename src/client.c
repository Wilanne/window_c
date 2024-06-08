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

	if (connect_socket(sockfd, "127.0.0.1", 12345) < 0) {
		fprintf(stderr, "Failed to connect to server\n");
		return -1;
	}

	uint8_t buffer[sizeof(msg_askMoving_t)];
	if (receive_data(sockfd, buffer, sizeof(buffer)) < 0) {
		fprintf(stderr, "Failed to receive data\n");
		return -1;
	}

	for (size_t i = 0; i < sizeof(buffer); i++) {
		printf("%d ", buffer[i]);
	}
	printf("\n");

	msg_askMoving_t msg;
	deserialize_msg_askMoving(buffer, &msg);

	printf("Received message of size %d and type %d\n", msg.header.length, msg.header.type);
	printf("Direction: %d\n", msg.direction);
	printf("Speed: %d\n", msg.speed);

	close(sockfd);
	
	return 0;
}
