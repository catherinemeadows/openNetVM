#include <rte_mbuf_core.h>

#include "cont_nf.h"

const int pkt_size = sizeof(struct rte_mbuf*);

/* file descriptors for TX and RX pipes */
int tx_fd;
int rx_fd;

int
open_pipes() {
        if (rx_fd = open(CONT_RX_PIPE_NAME, O_RDONLY | O_NONBLOCK) == -1) {
                return -1;
        }

        if (tx_fd = open(CONT_TX_PIPE_NAME, O_WRONLY | O_NONBLOCK) == -1) {
                return -1;
        }

        return 0;
}

void
pipe_cleanup() {
        remove(CONT_RX_PIPE_NAME);
        remove(CONT_TX_PIPE_NAME);

        close(rx_fd);
        close(tx_fd);
}

struct rte_mbuf*
read_packet() {
        size_t pkt_size = sizeof(struct rte_mbuf);
        struct rte_mbuf* packet = malloc(pkt_size);
        if (read(rx_fd, packet, pkt_size) == -1) {
                return NULL;
        }

        return packet;
}

int
write_packet(struct rte_mbuf* packet) {
        if (write(tx_fd, packet, pkt_size) == -1) {
                return -1;
        }

        return 0;
}

/*
 * Receive incoming packets
 */
void
receive_packets() {
        struct rte_mbuf* packet = read_packet();
        if (packet == NULL) {
                perror("Couldn't read packet data\n");
        }
        printf("Received packet from port %d\n", packet->port);

        // dummy to let host know we modified packet data
        packet->port = 10;

        if (write_packet(packet) == -1) {
                perror("Couldn't write data to TX pipe");
        }
}

int
main(void) {
        int ret = -1;

        /* open pipes */
        printf("Starting to open pipes\n");
        while (open_pipes() == -1) {
        }

        printf("Initialization finished\n");

        /* receive packets */
        receive_packets();

        pipe_cleanup();
        return 0;
}
