#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define BUFFER_LEN 100
#define PORT_ADDR 23456

/**
 * The error function prints out a given message and then closes
 * the program
 *
 * @param msg A given message to print
 */
void error(const char *msg) 
{
    printf("Error: %s\n", msg);
    exit(1);
}

/**
 * Create a TCP socket
 *
 * @return the file descriptor of the socket
 */
int create_socket() 
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // check for socket creation failure
    if (fd == -1) 
        error("Unable to create socket"); 

    printf("Socket was created!\n"); 
    return fd;
}

/**
 * Create a socket address with the port number 23456
 * 
 * @return a sockaddr_in stucture that represents the address
 */
struct sockaddr_in create_address() 
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_ADDR); // port number
    addr.sin_addr.s_addr = INADDR_ANY; // any address	

    printf("Address created\n");
    return addr;
}

/**
 * Binds the address to the TCP socket
 *
 * @param addr the previously created socket address
 * @param fd the file descripter
 */
void bind_address(int fd, struct sockaddr_in addr) 
{
    int ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    // check for binding failure
    if (ret < 0) 
        error("Failed to bind address to socket");

    printf("Binding was successful\n");
}

/**
 * Listens for incoming connections with the socket
 *
 * @param fd the file descriptor of the socket
 */
void listen_for_incoming_connection(int fd) 
{
    if (listen(fd, SOMAXCONN) < 0)
        error("Failed when listening for connections");

    printf("Listening successful\n");
}

/**
 * Accepts the clients connection to the socket
 *
 * @param fd The file descriptor for the socket 
 * @return The file descriptor of the client's accepted connection
 */
int accept_client (int fd) 
{
    struct sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);

    // accepct the incoming cliet connection
    int client_fd = accept(fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);

    // check for acception failure
    if(client_fd < 0) 
	    error("Failed to accept client");
    
    printf("Successfully accepted client\n");
    return client_fd;
}

/**
 * Recieve a message that was sent from the client
 *
 * @param client_fd The client's file descpritor
 * @param buffer The pointer to the buffer to contain the recieved message
 * @return The number of bytes recieved, -1 if failure occurred
 */
ssize_t receive_client_message(int client_fd, char *buffer) 
{
    // recieve the clients message and save it to the buffer
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_LEN, 0);

    // check for recieving failure
    if(bytes_received < 0) {
	    error("Failed to recieve message");
    }

    return bytes_received;
}

/**
 * Sends a given message to the clients display
 *
 * @param client_fd The client's file descriptor
 * @param msg The specified messaged to send
 */
void send_to_client(int client_fd, const char *msg) 
{
    // send the message
    ssize_t bytes_sent = send(client_fd, msg, strlen(msg), 0);

    // check for failure
    if(bytes_sent < 0) {
        error("Failed to send message to client");
    }
}

/**
 * Reverses a given message
 *
 * @param message contains the given message
 */
void reverse_msg(char *message) {
    int length = strlen(message);
    int i, j;
    char temp;

    // Reversing the string
    for (i = 0, j = length - 1; i < j; i++, j--) {
        temp = message[i];
        message[i] = message[j];
        message[j] = temp;
    }
}


/**
 * Create server based on port 23456, take client message and send the message
 * back in reverse, then close the socket
 */
int main() 
{

    int fd = create_socket(); // create TCP socket
    struct sockaddr_in addr = create_address(); // create address
    bind_address(fd, addr); // bind address to socket
    listen_for_incoming_connection(fd); // listen for incomming connections

    while (1) { 
        int client_fd = accept_client(fd); // get client file descriptor/accept client

        // recieve client message
        char buffer[BUFFER_LEN];
        receive_client_message(client_fd, buffer);

        // reverse and sent the message back
        reverse_msg(buffer);
        send_to_client(client_fd, buffer);

        close(client_fd); // close server
    }
}