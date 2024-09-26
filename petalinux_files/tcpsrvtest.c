#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>


int main(int argc, char *argv[])
{
    printf("TCP/IPv4 Server Test Started\n"); 

    int sock_num = 0;             // Socket descriptor number
    int conn_num = 0;             // Connection number/socket descriptor 
    struct sockaddr_in serv_addr; // IPv4 socket address (sin_addr: IP address, sin_family: socket domain, sin_port: port, sin_zero: reserved)
   
    char *ip_addr;   // IP Address 
    char *port_num;  // Port Number  

    char txBuffer[512]; // Transmit data buffer

    sock_num = socket(AF_INET, SOCK_STREAM, 0); // Creating endpoint for the communication, returns socket descriptor
    // socket() (arg1: address family (IPv4, IPv6), arg2: socket/connection type ( SOCK_STREAM: connection-oriented, SOCK_DGRAM: connectionless), arg3: socket protocol (default 0, TCP, UDP)
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(txBuffer, '0', sizeof(txBuffer));

    inet_aton("192.168.1.10", &serv_addr.sin_addr); // Store IP address "192.168.1.10" in "serv_addr"  
    ip_addr = inet_ntoa(serv_addr.sin_addr);        // Get string representation of IP address
    printf("Server IP address: %s\n", ip_addr);  

    serv_addr.sin_family = AF_INET;   // Address family IPv4
    serv_addr.sin_port = htons(5000); // Convert port number host byte order to network byte order and set serv_addr port 

    bind(sock_num, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); // Set local address for the socket 

    listen(sock_num, 5); // Listen and invite incoming connection requests (arg1: socket, arg2: max number of connection requests that can be queued)

    conn_num = accept(sock_num, (struct sockaddr*)NULL, NULL); // Wait for connection request and make connection, returns socket descriptor.
    printf("Connection Established\n");  
  
    // Send connection status info to the client
    const char *conninfo = "Server-Client Connection Ready";  // Connection status info 
    printf("Sending Connection Status Info to the Client\n");        
    snprintf(txBuffer, sizeof(txBuffer), "%s\r\n", conninfo); // Write to txBuffer
    write(conn_num, txBuffer, strlen(txBuffer));              // Write data to socket  

    int rx_size = 0;    // Receive data size (bytes)
    char rxBuffer[512]; // Receive data buffer
    memset(rxBuffer, '0',sizeof(rxBuffer));

    printf("Waiting for the client ...\n");

    // Start receiving data from the client and sending back
    while (1) 
    {
        rx_size = read(conn_num, rxBuffer, sizeof(rxBuffer)-1); // Read rx data sent by the client
        
        sleep(1); // Wait 1 seconds
    
        if(rx_size < 0) // Check for data receive error
        {
           printf("\n RX Channel Error  \n");
        } 

        if(rx_size > 0) // Check for received data
        {
           printf("Received data: %.*s\n", rx_size, rxBuffer);
           
           // Send data back to client
           printf("Echo: %.*s\n", rx_size,rxBuffer); 
           snprintf(txBuffer, sizeof(txBuffer), "%s\r\n", rxBuffer); // Write to txBuffer
           write(conn_num, txBuffer, rx_size);                       // Write data to socket

        }

        if ( (rx_size==1) && (rxBuffer[0]=='E') ) // Check for the end of connection indicator character 'E'
        {
           close(conn_num); // Close connection
           printf("Connection Ended\n"); 
           break;
        }

    }    
  
    return 0;
}
