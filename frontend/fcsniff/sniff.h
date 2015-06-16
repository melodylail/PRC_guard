/*
 * sniff on wlan, capture data using pattern
 * (C) 2015
 * author: mike zhuang
 *
 */

#ifndef _ZEROSNIFF_H_
#define _ZEROSNIFF_H_

#include "eapol.h"
#include "ieee80211.h"

/* some constants */

#define REFRESH_RATE 100000  /* default delay in us between updates */
#define DEFAULT_HOPFREQ 250  /* default delay in ms between channel hopping */
#define DEFAULT_CWIDTH  20 /* 20 MHz channels by default */

#define NB_PWR  5       /* size of signal power ring buffer */
#define NB_PRB 10       /* size of probed ESSID ring buffer */

#define MAX_CARDS 1	/* maximum number of cards to capture from */

#define	STD_OPN		0x0001
#define	STD_WEP		0x0002
#define	STD_WPA		0x0004
#define	STD_WPA2	0x0008

#define STD_FIELD	(STD_OPN | STD_WEP | STD_WPA | STD_WPA2)

#define	ENC_WEP		0x0010
#define	ENC_TKIP	0x0020
#define	ENC_WRAP	0x0040
#define	ENC_CCMP	0x0080
#define ENC_WEP40	0x1000
#define	ENC_WEP104	0x0100

#define ENC_FIELD	(ENC_WEP | ENC_TKIP | ENC_WRAP | ENC_CCMP | ENC_WEP40 | ENC_WEP104)

#define	AUTH_OPN	0x0200
#define	AUTH_PSK	0x0400
#define	AUTH_MGT	0x0800

#define AUTH_FIELD	(AUTH_OPN | AUTH_PSK | AUTH_MGT)

#define STD_QOS         0x2000

#define	QLT_TIME	5
#define	QLT_COUNT	25


#define RATES           \
    "\x01\x04\x02\x04\x0B\x16\x32\x08\x0C\x12\x18\x24\x30\x48\x60\x6C"

#define PROBE_REQ       \
    "\x40\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xCC\xCC\xCC\xCC\xCC\xCC"  \
    "\xFF\xFF\xFF\xFF\xFF\xFF\x00\x00"

//milliseconds to store last packets
#define BUFFER_TIME 3000


#define DUMP_CAP_EXT "cap"

#define NB_EXTENSIONS 1

const unsigned char LLCNULL[4] = {0, 0, 0, 0};
char *f_ext[NB_EXTENSIONS] = { DUMP_CAP_EXT };

extern const unsigned long int crc_tbl[256];
extern const unsigned char crc_chop_tbl[256][4];

static unsigned char ZERO[32] =
"\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00";


#define MIN_RAM_SIZE_LOAD_OUI_RAM 32768

int read_pkts=0;

int abg_chans [] =
{
    1, 7, 13, 2, 8, 3, 14, 9, 4, 10, 5, 11, 6, 12,
    36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108,
    112, 116, 120, 124, 128, 132, 136, 140, 149,
    153, 157, 161, 184, 188, 192, 196, 200, 204,
    208, 212, 216,0
};

int bg_chans  [] =
{
    1, 7, 13, 2, 8, 3, 14, 9, 4, 10, 5, 11, 6, 12, 0
};

int a_chans   [] =
{
    36, 40, 44, 48, 52, 56, 60, 64, 100, 104, 108,
    112, 116, 120, 124, 128, 132, 136, 140, 149,
    153, 157, 161, 184, 188, 192, 196, 200, 204,
    208, 212, 216,0
};

int *frequencies;

/* linked list of received packets for the last few seconds */
struct pkt_buf
{
    struct pkt_buf  *next;      /* next packet in list */
    unsigned char   *packet;    /* packet */
    unsigned short  length;     /* packet length */
    struct timeval  ctime;      /* capture time */
};

/* oui struct for list management */
struct oui {
	char id[9]; /* TODO: Don't use ASCII chars to compare, use unsigned char[3] (later) with the value (hex ascii will have to be converted) */
	char manuf[128]; /* TODO: Switch to a char * later to improve memory usage */
	struct oui *next;
};

/* WPS_info struct */
struct WPS_info {
    unsigned char version;    /* WPS Version */
    unsigned char state;      /* Current WPS state */
    unsigned char ap_setup_locked; /* AP setup locked */
    unsigned int meth;        /* WPS Config Methods */
};

/* linked list of detected access points */
struct AP_info
{
    struct AP_info *prev;     /* prev. AP in list         */
    struct AP_info *next;     /* next  AP in list         */

    time_t tinit, tlast;      /* first and last time seen */

    int channel;              /* AP radio channel         */
    int max_speed;            /* AP maximum speed in Mb/s */
    int avg_power;            /* averaged signal power    */
    int best_power;           /* best signal power    */
    int power_index;          /* index in power ring buf. */
    int power_lvl[NB_PWR];    /* signal power ring buffer */
    int preamble;             /* 0 = long, 1 = short      */
    int security;             /* ENC_*, AUTH_*, STD_*     */
    int beacon_logged;        /* We need 1 beacon per AP  */
    int dict_started;         /* 1 if dict attack started */
    int ssid_length;          /* length of ssid           */
    float gps_loc_min[5];     /* min gps coordinates      */
    float gps_loc_max[5];     /* max gps coordinates      */
    float gps_loc_best[5];    /* best gps coordinates     */


    unsigned long nb_bcn;     /* total number of beacons  */
    unsigned long nb_pkt;     /* total number of packets  */
    unsigned long nb_data;    /* number of  data packets  */
    unsigned long nb_data_old;/* number of data packets/sec*/
    int nb_dataps;  /* number of data packets/sec*/
    struct timeval tv;        /* time for data per second */

    unsigned char bssid[6];   /* the access point's MAC   */
    char *manuf;              /* the access point's manufacturer */
    unsigned char essid[MAX_IE_ELEMENT_SIZE];
                              /* ascii network identifier */
    unsigned long long timestamp;
    						  /* Timestamp to calculate uptime   */

    unsigned char lanip[4];   /* last detected ip address */
                              /* if non-encrypted network */

    unsigned char **uiv_root; /* unique iv root structure */
                              /* if wep-encrypted network */

    int    rx_quality;        /* percent of captured beacons */
    int    fcapt;             /* amount of captured frames   */
    int    fmiss;             /* amount of missed frames     */
    unsigned int    last_seq; /* last sequence number        */
    struct timeval ftimef;    /* time of first frame         */
    struct timeval ftimel;    /* time of last frame          */
    struct timeval ftimer;    /* time of restart             */

    char *key;		      /* if wep-key found by dict */
    int essid_stored;         /* essid stored in ivs file? */

    char decloak_detect;      /* run decloak detection? */
    struct pkt_buf *packets;  /* list of captured packets (last few seconds) */
    char is_decloak;          /* detected decloak */

	// This feature eats 48Mb per AP
	int EAP_detected;
    unsigned char *data_root; /* first 2 bytes of data if */
    						  /* WEP network; used for    */
    						  /* detecting WEP cloak	  */
    						  /* + one byte to indicate   */
    						  /* (in)existence of the IV  */

    int marked;
    int marked_color;
    struct WPS_info wps;
};

/* linked list of detected clients */

struct ST_info
{
    struct ST_info *prev;    /* the prev client in list   */
    struct ST_info *next;    /* the next client in list   */
    struct AP_info *base;    /* AP this client belongs to */
    time_t tinit, tlast;     /* first and last time seen  */
    unsigned long nb_pkt;    /* total number of packets   */
    unsigned char stmac[6];  /* the client's MAC address  */
    char *manuf;             /* the client's manufacturer */
    int probe_index;         /* probed ESSIDs ring index  */
    char probes[NB_PRB][MAX_IE_ELEMENT_SIZE];
                             /* probed ESSIDs ring buffer */
    int ssid_length[NB_PRB]; /* ssid lengths ring buffer  */
    int power;               /* last signal power         */
    int rate_to;             /* last bitrate to station   */
    int rate_from;           /* last bitrate from station */
    struct timeval ftimer;   /* time of restart           */
    int missed;              /* number of missed packets  */
    unsigned int lastseq;    /* last seen sequence number */
    struct WPA_hdsk wpa;     /* WPA handshake data        */
    int qos_to_ds;           /* does it use 802.11e to ds */
    int qos_fr_ds;           /* does it receive 802.11e   */
    int channel;             /* Channel station is seen   */
                             /*  Not used yet		  */
};

/* linked list of detected macs through ack, cts or rts frames */

struct NA_info
{
    struct NA_info *prev;    /* the prev client in list   */
    struct NA_info *next;    /* the next client in list   */
    time_t tinit, tlast;     /* first and last time seen  */
    unsigned char namac[6];  /* the stations MAC address  */
    int power;               /* last signal power         */
    int channel;             /* captured on channel       */
    int ack;                 /* number of ACK frames      */
    int ack_old;             /* old number of ACK frames  */
    int ackps;               /* number of ACK frames/s    */
    int cts;                 /* number of CTS frames      */
    int rts_r;               /* number of RTS frames (rx) */
    int rts_t;               /* number of RTS frames (tx) */
    int other;               /* number of other frames    */
    struct timeval tv;       /* time for ack per second   */
};
/* bunch of global stuff */

struct globals
{
    char *dump_prefix;
    char *f_cap_name;

    FILE *f_cap;            /* output cap file      */

    int channel;           /* current channel #    */
    int frequency;           /* current frequency #    */
    int *channels;
//     int *frequencies;
    int singlechan;         /* channel hopping set 1*/
    int singlefreq;         /* frequency hopping: 1 */
    int update_s;	    /* update delay in sec  */

    volatile int do_exit;            /* interrupt flag       */

    time_t sk_start;
    char *prefix;
    int sk_len;
    int sk_len2;

    int * own_channels;	    /* custom channel list  */
    int * own_frequencies;	    /* custom frequency list  */

    int record_data;		/* do we record data?   */
    char * iwpriv;
    char * iwconfig;
    char * wlanctlng;
    char * wl;

    char message[512];
    char decloak;
    int hopfreq;


    char*   s_iface;        /* source interface to read from */
    char   *s_file;
    FILE *f_cap_in;
    struct pcap_file_header pfh_in;

    char *freqstring;
    int freqoption;
    int chanoption;
    int output_format_pcap;

    int file_write_interval;

}g_config;

#endif /* _ZEROSNIFF_H_ */
