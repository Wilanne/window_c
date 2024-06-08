/**
 * Programme principal : Systemo de pilotage du robot.
 *
 * \author Léo AUBRY
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "mrpiz.h"
#include "pilot/pilot.h"
#include "sock/communication.h"
#include "scribe/scribe.h"

#define NUM_CLIENTS 2

static volatile int running = 1;
static bool emergency_state = 0;

static void handle_received_data(const uint8_t *data, size_t length);
static void *client_handler(void *arg);

static void handle_received_emergency(const uint8_t *data, size_t length);
static void *emergency_handler(void *arg);

static void *obstacle_handler(void *arg);

static void launch();

static void sigint_handler();
static void sigint_handler() {
    running = 0;
}

int main (void)
{
	printf("Hello Robot : Ctrl+C pour quitter\n");
	fflush(stdout);

	if (mrpiz_init() == -1) {
		mrpiz_error_print("Problème d'initialisation");
		return EXIT_FAILURE;
	}
	
	signal(SIGINT, sigint_handler);

	emergency_state = readEmergencyState();
	if (emergency_state == 1) {
		printf("Emergency Engaged, Unlock Robot Before Moving\n");
	}
	else if (emergency_state == 0) {
		printf("Emergency Released, Robot Can Move\n");
	}
	launch();

	stopAllMotor();
	setLed(0);
	mrpiz_close();

	printf("Systemo s'est éteint\n");
	return EXIT_SUCCESS;
}

static void handle_received_data(const uint8_t *data, size_t length) {
    message_t msg;
    deserialize_message(data, &msg);
    printf("Server received a message of type %d and length %d\n", msg.header.type, msg.header.length);
	if (emergency_state == 1) {
		printf("Emergency Engaged, Unlock Robot Before Moving\n");
		stopAllMotor();
	}
	else if (emergency_state == 0) {
		if (obstacleDetection() == 1) {
			printf("Obstacle Detected, Stop Robot\n");
			stopAllMotor();
		}
		else {
			printf("No Obstacle Detected, Robot Can Move\n");
			printf("Direction : %d, Speed : %d\n", msg.askMoving.direction, msg.askMoving.speed);
			makeAMove(msg.askMoving.direction, msg.askMoving.speed);
		}
	}
}

static void handle_received_emergency(const uint8_t *data, size_t length) {
    message_t msg;
    deserialize_message(data, &msg);

    printf("Server received a message of type %d and length %d\n", msg.header.type, msg.header.length);
	printf("Emergency state : %d\n", msg.setEmergency.state);

	if (msg.setEmergency.state == 1) {
		printf("Emergency stop\n");
		stopAllMotor();
		writeEmergencyState(1);
		setLed(1);
		emergency_state = 1;
	}
	else {
		printf("Emergency release\n");
		writeEmergencyState(0);
		setLed(0);
		emergency_state = 0;
	}
}

static void *client_handler(void *arg) {
    int client_sockfd = *(int *)arg;
    pthread_t receiver_thread = start_receive_thread(client_sockfd, handle_received_data);

    while (running)
	{
		message_t msg;
		memset(&msg, 0, sizeof(msg));

		msg.header.length = sizeof(message_t);
		msg.header.type = MESSAGE_TYPE_ALL;

		msg.sendInfoRobot.header.length = sizeof(msg_sendInfoRobot_t);
		msg.sendInfoRobot.header.type = MESSAGE_TYPE_SEND_INFO_ROBOT;

		msg.sendInfoRobot.pM1 = 10;
		msg.sendInfoRobot.pM2 = 10;
		msg.sendInfoRobot.speed = 50;

		msg.selectConf.header.length = sizeof(msg_selectConf_t);
		msg.selectConf.header.type = MESSAGE_TYPE_SELECT_CONF;
		msg.selectConf.id_algo = 1;
		msg.selectConf.time = 10;
		msg.selectConf.completion_percentage = 50;

		if (send_message(client_sockfd, &msg) < 0) {
			perror("send_message");
		}

        sleep(1);
	}

    stop_receive_thread(receiver_thread);
    close(client_sockfd);
    return NULL;
}

static void *emergency_handler(void *arg) {
    int client_sockfd = *(int *)arg;
    pthread_t receiver_thread = start_receive_thread(client_sockfd, handle_received_emergency);

    while (running) {
		msg_sendEmergencyState_t msg;
		uint8_t buffer[sizeof(msg_sendEmergencyState_t)];

		memset(&msg, 0, sizeof(msg));
		memset(&buffer, 0, sizeof(buffer));

		msg.header.length = sizeof(msg_sendEmergencyState_t);
		msg.header.type = MESSAGE_TYPE_SEND_EMERGENCY_STATE;
		msg.state = emergency_state;

		serialize_msg_sendEmergencyState(&msg, buffer);

		if (send_data(client_sockfd, buffer, sizeof(buffer)) < 0) {
			perror("send");
			exit(EXIT_FAILURE);
		}
        sleep(1); 
	}

    stop_receive_thread(receiver_thread);
    close(client_sockfd);
    return NULL;
}

static void *obstacle_handler(void *arg) {
	while (running) {
		if (obstacleDetection() == 1) {
			printf("Obstacle Detected, Stop Robot\n");
			stopAllMotor();
		} else {printf("No Obstacle Detected, Robot Can Move\n");}
		usleep(250000);
	}
	return NULL;
}

static void launch() {    
	int ports[NUM_CLIENTS] = {12346, 12347};
    int server_sockfds[NUM_CLIENTS];
    int client_sockfds[NUM_CLIENTS];
    pthread_t client_threads[NUM_CLIENTS];
	pthread_t obstacle_thread;

	setLed(3);

    for (int i = 0; i < NUM_CLIENTS; ++i) {
        server_sockfds[i] = establish_pending_communication(ports[i]);
        if (server_sockfds[i] < 0) {
            fprintf(stderr, "Failed to establish pending communication on port %d\n", ports[i]);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_CLIENTS; ++i) {
        client_sockfds[i] = accept_connection(server_sockfds[i]);
        close(server_sockfds[i]);  // Close the listening socket, not needed anymore
        if (client_sockfds[i] < 0) {
            fprintf(stderr, "Failed to accept connection on port %d\n", ports[i]);
            exit(EXIT_FAILURE);
        }
    }
	
	if (pthread_create(&client_threads[0], NULL, client_handler, &client_sockfds[0]) != 0) {
		perror("pthread_create");
            exit(EXIT_FAILURE);
	}

	if (pthread_create(&client_threads[1], NULL, emergency_handler, &client_sockfds[1]) != 0) {
		perror("pthread_create");
            exit(EXIT_FAILURE);
	}

	if (pthread_create(&obstacle_thread, NULL, obstacle_handler, NULL) != 0) {
		perror("pthread_create");
            exit(EXIT_FAILURE);
	}
	pthread_detach(obstacle_thread);

    for (int i = 0; i < NUM_CLIENTS; ++i) {
        pthread_join(client_threads[i], NULL);
    }

    for (int i = 0; i < NUM_CLIENTS; ++i) {
        close(server_sockfds[i]);
    }
}

