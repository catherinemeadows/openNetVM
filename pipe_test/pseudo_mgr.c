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
    int fd;
    char * writer = "/tmp/9c3f2e30c868fcd83f1fc779abe125c3a06402a362fae63d9ac06ebca37bec6f";

    /* create the FIFO (named pipe) */
    mkfifo(writer, 0666);

    /* send pkt to the FIFO */
    fd = open(writer, O_WRONLY);
    while (1) {
        write(fd, packet, sizeof(pkt));
    }

    /* cleanup */ 
    close(fd);
    unlink(writer);

    return 0;
}