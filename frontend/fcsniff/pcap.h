#ifndef _PCAP_H_
#define _PCAP_H_
#include "nde_types.h"

#define FORMAT_CAP      1

#define TCPDUMP_MAGIC           0xA1B2C3D4
#define TCPDUMP_CIGAM           0xD4C3B2A1

#define PCAP_VERSION_MAJOR      2
#define PCAP_VERSION_MINOR      4

#define LINKTYPE_ETHERNET       1
#define LINKTYPE_IEEE802_11     105
#define LINKTYPE_PRISM_HEADER   119
#define LINKTYPE_RADIOTAP_HDR   127
#define LINKTYPE_PPI_HDR		192

// Maximum length of an Information Element
#define MAX_IE_ELEMENT_SIZE 256

struct pcap_file_header
{
    uint32_t magic;
    uint16_t version_major;
    uint16_t version_minor;
    int32_t thiszone;
    uint32_t sigfigs;
    uint32_t snaplen;
    uint32_t linktype;
}__attribute__((packed));

struct pcap_pkthdr
{
    int32_t tv_sec;
    int32_t tv_usec;
    uint32_t caplen;
    uint32_t len;
}__attribute__((packed));


#endif /* _PCAP_H_ */
