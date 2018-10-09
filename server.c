#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "common.h"

#define FILENAME "output.txt"

void receive(int sock);
void make_file(Packet pck, int count);
bool is_first = true;

void main()
{
    int sock;
    struct sockaddr_in serv_addr;

    // create socket
    if((sock = socket(PF_INET, SOCK_DGRAM, 0)) == -1){
        printf("socket failed\n");
        exit(1);
    }

    printf("socket is successfully created!\n");

    // serv_addr 초기화
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPv4 IP
    serv_addr.sin_port = htons(PORT); // Port number
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 32bit IPv4 address

    // bind()
    if(bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))){
        printf("bind failed\n");
        exit(1);
    }

    receive(sock);

    close(sock);
}

void receive(int sock)
{
    int cli_addr_size;
    int cnt = 0;
    struct sockaddr_in cli_addr;

    Packet pck;

    while(1){
        cli_addr_size = sizeof(cli_addr);
        pck.pck_len = recvfrom(sock, pck.buffer, BUFSIZE, 0,
            (struct sockaddr*)&cli_addr, &cli_addr_size);

        if(*(pck.buffer) == eof)
            break;

        make_file(pck, cnt);
        cnt++;
    }

    printf("The file has been received!\n");
}

void make_file(Packet pck, int count)
{
    FILE* fp = NULL;
    long int offset;

    if(is_first == true){
        fp = fopen(FILENAME, "w+");
        is_first = false;
    }
    else{
        fp = fopen(FILENAME, "a");
        offset = BUFSIZE * count;
        fseek(fp, offset, SEEK_SET);
        fwrite(pck.buffer, pck.pck_len, 1, fp);
        fclose(fp);
    }
}