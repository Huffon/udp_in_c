#define PORT 50000
#define BUFSIZE 1024

char eof = 0x1A;

// Fake Packet
typedef struct _PACKET {
    // Packet infomation
    char buffer[BUFSIZE];
    char src_ip[20];
    char dest_ip[20];

    // UDP header
    char dest_port[10];
    int pck_len;
} Packet;