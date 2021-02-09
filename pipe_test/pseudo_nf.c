#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_BUF 1024

typedef struct pkt {
	int port; 
	int pkt_type; 
} pkt;

int main()
{
	/* receive w mkfifo() */ 
    int fd = -1;
    int fd2; 
    char * reader = "/tmp/myfifo0";
    char * writer = "/tmp/myfifo1";
   
	pkt* packet = malloc (sizeof(pkt));

    /* open, read, and display the message from the FIFO */
    while(fd == -1) {
        fd = open(reader, O_RDONLY);
    }
    
    /* open writer so we can send pkt back to mgr */
    printf("making writer fifo\n"); 
    mkfifo(writer, 0666);
    fd2 = open(writer, O_WRONLY);

    while (1) {
        /* get packet */ 
        read(fd, packet, sizeof(pkt));
        printf("Received port: %d\n", packet->port);
	    printf("Received type: %d\n", packet->pkt_type);

        /* modify data before sending back */ 
        packet->port--;
        packet->pkt_type++;

        /* send it back */ 
        write(fd2, packet, sizeof(pkt));
    }

    close(fd);
    close(fd2);
    unlink(writer);

    return 0;
}