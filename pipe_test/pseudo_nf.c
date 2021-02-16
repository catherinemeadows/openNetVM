#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

typedef struct pkt0 {
	void * buf_addr;
    uint16_t refcnt; 
    uint16_t nb_segs;
    uint16_t port;
    uint64_t ol_flags;
    uint32_t pkt_len;
    uint16_t data_len;
    uint16_t vlan_tci;
    uint16_t vlan_tci_outer;
    uint16_t buf_len;
    // struct rte_mempool * pool;
    // struct rte_mbuf * next; 
    // struct rte_mbuf_ext_shared_info * shinfo;
    uint16_t priv_size;
    uint16_t timesync;
    uint16_t dynfield1[9];
    uint32_t packet_type;
    uint8_t l2_type:4;
    uint8_t l3_type:4;
    uint8_t l4_type:4;
    uint8_t tun_type:4;
    uint8_t inner_esp_next_proto;
    uint8_t inner_l2_type:4;
    uint8_t inner_l3_type:4;
    uint8_t inner_l4_type:4;
    uint32_t rss;
    uint32_t lo;
    uint32_t hi;
} pkt0;

typedef struct pkt {
	int port; 
	int pkt_type;  
} pkt;


/* 
 * Get container ID and concatenate with /tmp/ dir 
 * to return fifo name
*/ 
char*
get_fifo_name(void)
{
    char* reader = NULL;
    char* tmp = "/tmp/";
    char buf[255];
  
    /* get container ID */ 
    FILE *fp = (popen("head -1 /proc/self/cgroup|cut -d/ -f3", "r"));
    if (fp == NULL) {
        perror("Failed to popen\n" );
        exit(1);
    }

    fgets(buf, sizeof(buf), fp) != NULL;
    strtok(buf, "\n");

    /* concat with /tmp/ */ 
    reader = malloc(strlen(buf) + strlen(tmp));
    strcpy(reader, tmp);
    strcat(reader, buf);

    pclose(fp);

    return reader;
}

int 
main(void)
{
    /* receive w mkfifo() */ 
    int fd = -1;
    
    /* get fifo name */ 
    char* reader = get_fifo_name();
   
	pkt* packet = malloc (sizeof(pkt));

    /* open fifo to read */
    while(fd == -1) {
        fd = open(reader, O_RDONLY);
    }
    
    while (1) {
        /* get packet */ 
        read(fd, packet, sizeof(pkt));
        printf("Received port: %d\n", packet->port);
    }

    close(fd);
}
