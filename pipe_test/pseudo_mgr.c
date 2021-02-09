#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct pkt {
	int port; 
	int pkt_type;  
} pkt; 

int main()
{
	/* pkt to pass over pipe */ 
	pkt* packet = malloc (sizeof(pkt));
	packet->port = 8080;
	packet->pkt_type = 0;

	/* send with mkfifo() */ 
    int fd, fd2;
    char * writer = "/tmp/myfifo0";
    char * reader = "/tmp/myfifo1";


    /* create the FIFO (named pipe) */
    mkfifo(writer, 0666);

    /* send pkt to the FIFO */
    fd = open(writer, O_WRONLY);
    // write(fd, packet, sizeof(pkt));

    while (1) {
        write(fd, packet, sizeof(pkt));

        fd2 = open(reader, O_RDONLY);

        /* get packet */ 
        read(fd2, packet, sizeof(pkt));
        printf("Received port: %d\n", packet->port);
	    printf("Received type: %d\n", packet->pkt_type);
    }


    close(fd2);
    close(fd);

    /* remove the FIFO */
    unlink(writer);

    return 0;
}