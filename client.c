#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "common.h"

#define FILENAME "input.txt"

void main(int argc, char* argv[]) {
    int sock;
    int serv_addr_size;
    struct sockaddr_in serv_addr;

    FILE* fp = NULL;
    size_t f_size;

    Packet pck;

    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("socket failed\n");
        exit(1);
    }

    printf("socket is successfully created!\n");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPv4 IP
    serv_addr.sin_port = htons(PORT); // port number
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // 32bit IPv4 address

    fp = fopen(FILENAME, "r");

    fseek(fp, 0, SEEK_END);
    f_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // loop until file is completly sent
    while (f_size > 0) {
        pck.pck_len = f_size > BUFSIZE ? BUFSIZE : f_size;

        fread(pck.buffer, pck.pck_len, 1, fp);
        sendto(sock, pck.buffer, pck.pck_len, 0,
            (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        f_size -= pck.pck_len;
    }
    // send eof to server
    sendto(sock, &eof, sizeof(eof), 0,
            (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("The file has been sent.\n");

    fclose(fp);
    close(sock);
}