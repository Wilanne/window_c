// Author : Léo AUBRY 
// Author : Baptiste JOUBERT 
// Author : THEO MALINGE

/* /!\ Big problem with structure padding /!\
* Ex : askMoving_t
* header : 8 bytes
* direction : 1 byte
* speed : 1 byte
* Total : 10 bytes but sizeof(askMoving_t) = 12 bytes
* Solution : __attribute__((packed)) in structure definition
* Ex : typedef struct __attribute__((packed)) { ... } askMoving_t;
* Or -fpack-struct in gcc command line
* Or every structure should be a multiple of 4 bytes
* Causing severe problems with the Java side, not C side
*/

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stdio.h>

/**
 * @brief Enumeration of message types.
 */
typedef enum msg_type_t {
    MESSAGE_TYPE_ASK_MOVING,
    MESSAGE_TYPE_SEND_INFO_ROBOT,
    MESSAGE_TYPE_ALL
} msg_type_t;

/**
 * @brief Structure representing the header of a message.
 */
typedef struct msg_header_t {
    uint32_t length; 
    msg_type_t type;
} msg_header_t;

/**
 * @brief Structure representing a message for asking moving state.
 */
typedef struct msg_askMoving_t { 
    msg_header_t header; // 8 bytes
    uint16_t direction; // 2 bytes
    uint16_t speed; // 2 bytes
} msg_askMoving_t; // 12 bytes

/**
 * @brief Structure representing a message for asking moving state.
 */
typedef struct __attribute__((packed)) msg_askMoving_packed_t { 
    msg_header_t header; // 8 bytes
    uint8_t direction; // 1 bytes
    uint8_t speed; // 1 bytes
} msg_askMoving_packed_t; // 10 bytes

/**
 * @brief Structure representing a message for sending robot information.
 */
typedef struct msg_sendInfoRobot_t {
    msg_header_t header; // 8 bytes
    uint8_t pM1; // 1 byte
    uint8_t pM2; // 1 byte
    uint8_t speed; // 1 byte
} msg_sendInfoRobot_t; // 11 bytes

void serialize_msg_sendInfoRobot(const msg_sendInfoRobot_t *msg, uint8_t *buffer);
void deserialize_msg_sendInfoRobot(const uint8_t *buffer, msg_sendInfoRobot_t *msg);

void serialize_msg_askMoving(const msg_askMoving_t *msg, uint8_t *buffer);
void deserialize_msg_askMoving(const uint8_t *buffer, msg_askMoving_t *msg);

////////////////////////////// File Handling /////////////////////////////

/**
 * @brief Sends a file over a socket connection.
 * 
 * @param fp The file pointer of the file to be sent.
 * @param sockfd The socket file descriptor.
 */
void send_file(FILE *fp, int sockfd);

/**
 * @brief Receives a file over a socket connection.
 * 
 * @param sockfd The socket file descriptor.
 */
void receive_file(int sockfd);

#endif // PROTOCOL_H