#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef int SOCKET;
#define MAX_CLIENTS 2 * 4

#define PORT_NUM 6969
int main(void){
    int s_res;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    SOCKET sock;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s_res = getaddrinfo(NULL,PORT_NUM, &hints, &result);

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sock = socket(rp->ai_family, rp->ai_socktype,
                    rp->ai_protocol);
        if (sock == -1)
            continue;

        if (bind(sock, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* Success */
        
        listen(sock, LISTEN_BACKLOG)
    }
    struct sockaddr client_addr[MAX_CLIENTS] = {0}
    SOCKET client_socks[MAX_CLIENTS] = {0}

    client_socks[0] = accept(sock,&client_addr[0],sizeof(client_addr[0]));


    freeaddrinfo(result);           /* No longer needed */

    char msg[512] = {0};
    int len = snprintf(msg,"Hello World %d !",67);
    send(client_socks[0], msg, len, 0);

    char buff[512] = {0};
    recv(client_socks[0],buff,512,0);

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }
}