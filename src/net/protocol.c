// Author : Leo AUBRY

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "protocol.h"

/////////////////////////// Serialize ////////////////////////////////

//////////////////////////  ASK MOVING   //////////////////////////

void serialize_msg_askMoving(const msg_askMoving_t *msg, uint8_t *buffer) {
    uint32_t net_length = htonl(msg->header.length);
    uint32_t net_type = htonl(msg->header.type);
    uint16_t net_direction = htons(msg->direction);
    uint16_t net_speed = htons(msg->speed);

    memcpy(buffer, &net_length, sizeof(net_length));
    memcpy(buffer + sizeof(net_length), &net_type, sizeof(net_type));
    memcpy(buffer + sizeof(net_length) + sizeof(net_type), &net_direction, sizeof(net_direction));
    memcpy(buffer + sizeof(net_length) + sizeof(net_type) + sizeof(net_direction), &net_speed, sizeof(net_speed));
}

//////////////////////////  ASK MOVING_PACKED   //////////////////////////

void serialize_msg_askMoving_Packed(const msg_askMoving_packed_t *msg, uint8_t *buffer) {
    uint32_t length = sizeof(msg_askMoving_packed_t);
    uint32_t net_length = htons(length);
    uint32_t net_type = htons(msg->header.type);

    memcpy(buffer, &net_length, sizeof(net_length));
    memcpy(buffer + sizeof(net_length), &net_type, sizeof(net_type));
    memcpy(buffer + sizeof(net_length) + sizeof(net_type), &msg->direction, sizeof(msg->direction));
    memcpy(buffer + sizeof(net_length) + sizeof(net_type) + sizeof(msg->direction), &msg->speed, sizeof(msg->speed));
}
//////////////////////////  SEND INFO ROBOT   //////////////////////////

void serialize_msg_sendInfoRobot(const msg_sendInfoRobot_t *msg, uint8_t *buffer) {
    uint32_t length = sizeof(msg_sendInfoRobot_t);
    uint32_t net_length = htons(length);
    uint32_t net_type = htons(msg->header.type);

    memcpy(buffer, &net_length, sizeof(net_length));
    memcpy(buffer + sizeof(net_length), &net_type, sizeof(net_type));
    memcpy(buffer + sizeof(net_length) + sizeof(net_type), &msg->pM1, sizeof(msg->pM1));
    memcpy(buffer + sizeof(net_length) + sizeof(net_type) + sizeof(msg->pM1), &msg->pM2, sizeof(msg->pM2));
    memcpy(buffer + sizeof(net_length) + sizeof(net_type) + sizeof(msg->pM1) + sizeof(msg->pM2), &msg->speed, sizeof(msg->speed));
}

/////////////////////////////// Deserialize ////////////////////////////////

//////////////////////////  ASK MOVING   //////////////////////////

void deserialize_msg_askMoving(const uint8_t *buffer, msg_askMoving_t *msg) {
    uint32_t net_length, net_type;
    uint16_t net_direction, net_speed;

    memcpy(&net_length, buffer, sizeof(net_length));
    memcpy(&net_type, buffer + sizeof(net_length), sizeof(net_type));
    memcpy(&net_direction, buffer + sizeof(net_length) + sizeof(net_type), sizeof(net_direction));
    memcpy(&net_speed, buffer + sizeof(net_length) + sizeof(net_type) + sizeof(net_direction), sizeof(net_speed));

    msg->header.length = ntohl(net_length);
    msg->header.type = ntohl(net_type);
    msg->direction = ntohs(net_direction);
    msg->speed = ntohs(net_speed);
}

//////////////////////////  SEND INFO ROBOT   //////////////////////////

void deserialize_msg_sendInfoRobot(const uint8_t *buffer, msg_sendInfoRobot_t *msg) {
    uint32_t net_length, net_type;
    memcpy(&net_length, buffer, sizeof(net_length));
    memcpy(&net_type, buffer + sizeof(net_length), sizeof(net_type));

    msg->header.length = ntohs(net_length);
    msg->header.type = ntohs(net_type);

    memcpy(&msg->pM1, buffer + sizeof(net_length) + sizeof(net_type), sizeof(msg->pM1));
    memcpy(&msg->pM2, buffer + sizeof(net_length) + sizeof(net_type) + sizeof(msg->pM1), sizeof(msg->pM2));
    memcpy(&msg->speed, buffer + sizeof(net_length) + sizeof(net_type) + sizeof(msg->pM1) + sizeof(msg->pM2), sizeof(msg->speed));
}

/////////////////////////////// File Handling ////////////////////////////////

void send_file(FILE *fp, int sockfd) {
    char buffer[1024];
    ssize_t bytes_read;
    
    while ((bytes_read = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0) {
        if (send(sockfd, buffer, bytes_read, 0) == -1) {
            perror("Error sending file");
            return;
        }
    }
}

void receive_file(int sockfd) {
    char buffer[1024];
    ssize_t bytes_received;

    FILE *fp = fopen("received_file.txt", "wb");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
            if (bytes_received == -1) {
            perror("Error receiving file");
            exit(1);
        }
        fwrite(buffer, sizeof(char), bytes_received, fp);
    }
    fclose(fp);
}

