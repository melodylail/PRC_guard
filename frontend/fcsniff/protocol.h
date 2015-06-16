#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include "ieee80211.h"

/* Definitioins for 802.11 Layer */
#define	T_MGMT 0x0  /* management */
#define	T_CTRL 0x1  /* control */
#define	T_DATA 0x2 /* data */
#define	T_RESV 0x3  /* reserved */

/* from tcpdump begin */
#define CTRL_CONTROL_WRAPPER	0x7
#define	CTRL_BAR	0x8
#define	CTRL_BA		0x9
#define	CTRL_PS_POLL	0xA
#define	CTRL_RTS	0xB
#define	CTRL_CTS	0xC
#define	CTRL_ACK	0xD
#define	CTRL_CF_END	0xE
#define	CTRL_END_ACK	0xF

#define	DATA_DATA			0x0
#define	DATA_DATA_CF_ACK		0x1
#define	DATA_DATA_CF_POLL		0x2
#define	DATA_DATA_CF_ACK_POLL		0x3
#define	DATA_NODATA			0x4
#define	DATA_NODATA_CF_ACK		0x5
#define	DATA_NODATA_CF_POLL		0x6
#define	DATA_NODATA_CF_ACK_POLL		0x7

#define DATA_QOS_DATA			0x8
#define DATA_QOS_DATA_CF_ACK		0x9
#define DATA_QOS_DATA_CF_POLL		0xA
#define DATA_QOS_DATA_CF_ACK_POLL	0xB
#define DATA_QOS_NODATA			0xC
#define DATA_QOS_CF_POLL_NODATA		0xE
#define DATA_QOS_CF_ACK_POLL_NODATA	0xF

/*
 * The subtype field of a data frame is, in effect, composed of 4 flag
 * bits - CF-Ack, CF-Poll, Null (means the frame doesn't actually have
 * any data), and QoS.
 */
#define DATA_FRAME_IS_CF_ACK(x)		((x) & 0x01)
#define DATA_FRAME_IS_CF_POLL(x)	((x) & 0x02)
#define DATA_FRAME_IS_NULL(x)		((x) & 0x04)
#define DATA_FRAME_IS_QOS(x)		((x) & 0x08)

/*
 * Bits in the frame control field.
 */
#define	FC_VERSION(fc)		((fc) & 0x3)
#define	FC_TYPE(fc)		(((fc) >> 2) & 0x3)
#define	FC_SUBTYPE(fc)		(((fc) >> 4) & 0xF)
#define	FC_TO_DS(fc)		((fc) & 0x0100)
#define	FC_FROM_DS(fc)		((fc) & 0x0200)
#define	FC_MORE_FLAG(fc)	((fc) & 0x0400)
#define	FC_RETRY(fc)		((fc) & 0x0800)
#define	FC_POWER_MGMT(fc)	((fc) & 0x1000)
#define	FC_MORE_DATA(fc)	((fc) & 0x2000)
#define	FC_PROTECTED(fc)	((fc) & 0x4000)
#define	FC_ORDER(fc)		((fc) & 0x8000)

struct mgmt_header_t {
	uint16_t	fc;
	uint16_t 	duration;
	uint8_t		da[IEEE802_11_DA_LEN];
	uint8_t		sa[IEEE802_11_SA_LEN];
	uint8_t		bssid[IEEE802_11_BSSID_LEN];
	uint16_t	seq_ctrl;
};

#define	MGMT_HDRLEN	(IEEE802_11_FC_LEN+IEEE802_11_DUR_LEN+\
			 IEEE802_11_DA_LEN+IEEE802_11_SA_LEN+\
			 IEEE802_11_BSSID_LEN+IEEE802_11_SEQ_LEN)

#define	CAPABILITY_ESS(cap)	((cap) & 0x0001)
#define	CAPABILITY_IBSS(cap)	((cap) & 0x0002)
#define	CAPABILITY_CFP(cap)	((cap) & 0x0004)
#define	CAPABILITY_CFP_REQ(cap)	((cap) & 0x0008)
#define	CAPABILITY_PRIVACY(cap)	((cap) & 0x0010)

struct ssid_t {
	uint8_t		element_id;
	uint8_t		length;
	u_char		ssid[33];  /* 32 + 1 for null */
};

struct rates_t {
	uint8_t		element_id;
	uint8_t		length;
	uint8_t		rate[16];
};

struct challenge_t {
	uint8_t		element_id;
	uint8_t		length;
	uint8_t		text[254]; /* 1-253 + 1 for null */
};

struct fh_t {
	uint8_t		element_id;
	uint8_t		length;
	uint16_t	dwell_time;
	uint8_t		hop_set;
	uint8_t 	hop_pattern;
	uint8_t		hop_index;
};

struct ds_t {
	uint8_t		element_id;
	uint8_t		length;
	uint8_t		channel;
};

struct cf_t {
	uint8_t		element_id;
	uint8_t		length;
	uint8_t		count;
	uint8_t		period;
	uint16_t	max_duration;
	uint16_t	dur_remaing;
};

struct tim_t {
	uint8_t		element_id;
	uint8_t		length;
	uint8_t		count;
	uint8_t		period;
	uint8_t		bitmap_control;
	uint8_t		bitmap[251];
};

#define	E_SSID 		0
#define	E_RATES 	1
#define	E_FH	 	2
#define	E_DS 		3
#define	E_CF	 	4
#define	E_TIM	 	5
#define	E_IBSS 		6
/* reserved 		7 */
/* reserved 		8 */
/* reserved 		9 */
/* reserved 		10 */
/* reserved 		11 */
/* reserved 		12 */
/* reserved 		13 */
/* reserved 		14 */
/* reserved 		15 */
/* reserved 		16 */

#define	E_CHALLENGE 	16
/* reserved 		17 */
/* reserved 		18 */
/* reserved 		19 */
/* reserved 		16 */
/* reserved 		16 */


struct mgmt_body_t {
	uint8_t   	timestamp[IEEE802_11_TSTAMP_LEN];
	uint16_t  	beacon_interval;
	uint16_t 	listen_interval;
	uint16_t 	status_code;
	uint16_t 	aid;
	u_char		ap[IEEE802_11_AP_LEN];
	uint16_t	reason_code;
	uint16_t	auth_alg;
	uint16_t	auth_trans_seq_num;
	int		challenge_present;
	struct challenge_t  challenge;
	uint16_t	capability_info;
	int		ssid_present;
	struct ssid_t	ssid;
	int		rates_present;
	struct rates_t 	rates;
	int		ds_present;
	struct ds_t	ds;
	int		cf_present;
	struct cf_t	cf;
	int		fh_present;
	struct fh_t	fh;
	int		tim_present;
	struct tim_t	tim;
};

struct ctrl_control_wrapper_hdr_t {
	uint16_t	fc;
	uint16_t	duration;
	uint8_t		addr1[IEEE802_11_ADDR1_LEN];
	uint16_t	carried_fc[IEEE802_11_CARRIED_FC_LEN];
	uint16_t	ht_control[IEEE802_11_HT_CONTROL_LEN];
};

#define	CTRL_CONTROL_WRAPPER_HDRLEN	(IEEE802_11_FC_LEN+IEEE802_11_DUR_LEN+\
					 IEEE802_11_ADDR1_LEN+\
					 IEEE802_11_CARRIED_FC_LEN+\
					 IEEE802_11_HT_CONTROL_LEN)

struct ctrl_rts_hdr_t {
	uint16_t	fc;
	uint16_t	duration;
	uint8_t		ra[IEEE802_11_RA_LEN];
	uint8_t		ta[IEEE802_11_TA_LEN];
};

#define	CTRL_RTS_HDRLEN	(IEEE802_11_FC_LEN+IEEE802_11_DUR_LEN+\
			 IEEE802_11_RA_LEN+IEEE802_11_TA_LEN)

struct ctrl_cts_hdr_t {
	uint16_t	fc;
	uint16_t	duration;
	uint8_t		ra[IEEE802_11_RA_LEN];
};

#define	CTRL_CTS_HDRLEN	(IEEE802_11_FC_LEN+IEEE802_11_DUR_LEN+IEEE802_11_RA_LEN)

struct ctrl_ack_hdr_t {
	uint16_t	fc;
	uint16_t	duration;
	uint8_t		ra[IEEE802_11_RA_LEN];
};

#define	CTRL_ACK_HDRLEN	(IEEE802_11_FC_LEN+IEEE802_11_DUR_LEN+IEEE802_11_RA_LEN)

struct ctrl_ps_poll_hdr_t {
	uint16_t	fc;
	uint16_t	aid;
	uint8_t		bssid[IEEE802_11_BSSID_LEN];
	uint8_t		ta[IEEE802_11_TA_LEN];
};

#define	CTRL_PS_POLL_HDRLEN	(IEEE802_11_FC_LEN+IEEE802_11_AID_LEN+\
				 IEEE802_11_BSSID_LEN+IEEE802_11_TA_LEN)

struct ctrl_end_hdr_t {
	uint16_t	fc;
	uint16_t	duration;
	uint8_t		ra[IEEE802_11_RA_LEN];
	uint8_t		bssid[IEEE802_11_BSSID_LEN];
};

#define	CTRL_END_HDRLEN	(IEEE802_11_FC_LEN+IEEE802_11_DUR_LEN+\
			 IEEE802_11_RA_LEN+IEEE802_11_BSSID_LEN)

struct ctrl_end_ack_hdr_t {
	uint16_t	fc;
	uint16_t	duration;
	uint8_t		ra[IEEE802_11_RA_LEN];
	uint8_t		bssid[IEEE802_11_BSSID_LEN];
};

#define	CTRL_END_ACK_HDRLEN	(IEEE802_11_FC_LEN+IEEE802_11_DUR_LEN+\
				 IEEE802_11_RA_LEN+IEEE802_11_BSSID_LEN)

struct ctrl_ba_hdr_t {
	uint16_t	fc;
	uint16_t	duration;
	uint8_t		ra[IEEE802_11_RA_LEN];
};

#define	CTRL_BA_HDRLEN	(IEEE802_11_FC_LEN+IEEE802_11_DUR_LEN+IEEE802_11_RA_LEN)

struct ctrl_bar_hdr_t {
	uint16_t	fc;
	uint16_t	dur;
	uint8_t		ra[IEEE802_11_RA_LEN];
	uint8_t		ta[IEEE802_11_TA_LEN];
	uint16_t	ctl;
	uint16_t	seq;
};

#define	CTRL_BAR_HDRLEN		(IEEE802_11_FC_LEN+IEEE802_11_DUR_LEN+\
				 IEEE802_11_RA_LEN+IEEE802_11_TA_LEN+\
				 IEEE802_11_CTL_LEN+IEEE802_11_SEQ_LEN)

struct meshcntl_t {
	uint8_t		flags;
	uint8_t		ttl;
	uint8_t		seq[4];
	uint8_t		addr4[6];
	uint8_t		addr5[6];
	uint8_t		addr6[6];
};

#define	IV_IV(iv)	((iv) & 0xFFFFFF)
#define	IV_PAD(iv)	(((iv) >> 24) & 0x3F)
#define	IV_KEYID(iv)	(((iv) >> 30) & 0x03)

/* from tcpdump end */


/* Definitions for LLC header BEGIN */
#define LLC_MIN_LEN 3
#define LLC_SNAP_LEN 5

#define	LLC_U_FMT	3
#define	LLC_GSAP	1
#define	LLC_IG	        1 /* Individual / Group */
#define LLC_S_FMT	1

#define	LLC_U_POLL	0x10
#define	LLC_IS_POLL	0x0100
#define	LLC_XID_FI	0x81

#define	LLC_U_CMD(u)	((u) & 0xef)
#define	LLC_UI		0x03
#define	LLC_UA		0x63
#define	LLC_DISC	0x43
#define	LLC_DM		0x0f
#define	LLC_SABME	0x6f
#define	LLC_TEST	0xe3
#define	LLC_XID		0xaf
#define	LLC_FRMR	0x87

#define	LLC_S_CMD(is)	(((is) >> 2) & 0x03)
#define	LLC_RR		0x0001
#define	LLC_RNR		0x0005
#define	LLC_REJ		0x0009

#define LLC_IS_NR(is)	(((is) >> 9) & 0x7f)
#define LLC_I_NS(is)	(((is) >> 1) & 0x7f)

#ifndef LLCSAP_NULL
#define	LLCSAP_NULL		0x00
#endif
#ifndef LLCSAP_GLOBAL
#define	LLCSAP_GLOBAL		0xff
#endif
#ifndef LLCSAP_8021B_I
#define	LLCSAP_8021B_I		0x02
#endif
#ifndef LLCSAP_8021B_G
#define	LLCSAP_8021B_G		0x03
#endif
#ifndef LLCSAP_SNA
#define	LLCSAP_SNA		0x04
#endif
#ifndef LLCSAP_IP
#define	LLCSAP_IP		0x06
#endif
#ifndef LLCSAP_PROWAYNM
#define	LLCSAP_PROWAYNM		0x0e
#endif
#ifndef LLCSAP_8021D
#define	LLCSAP_8021D		0x42
#endif
#ifndef LLCSAP_RS511
#define	LLCSAP_RS511		0x4e
#endif
#ifndef LLCSAP_ISO8208
#define	LLCSAP_ISO8208		0x7e
#endif
#ifndef LLCSAP_PROWAY
#define	LLCSAP_PROWAY		0x8e
#endif
#ifndef LLCSAP_SNAP
#define	LLCSAP_SNAP		0xaa
#endif
#ifndef LLCSAP_IPX
#define LLCSAP_IPX		0xe0
#endif
#ifndef LLCSAP_NETBEUI
#define LLCSAP_NETBEUI		0xf0
#endif
#ifndef LLCSAP_ISONS
#define	LLCSAP_ISONS		0xfe
#endif

/* Definitions for LLC header END */

/* ethertype BEGIN */
#ifndef ETHERTYPE_LEN
#define ETHERTYPE_LEN           2
#endif

#ifndef ETHERTYPE_GRE_ISO
#define ETHERTYPE_GRE_ISO       0x00FE  /* not really an ethertype only used in GRE */
#endif
#ifndef ETHERTYPE_PUP
#define	ETHERTYPE_PUP		0x0200	/* PUP protocol */
#endif
#ifndef ETHERTYPE_IP
#define	ETHERTYPE_IP		0x0800	/* IP protocol */
#endif
#ifndef ETHERTYPE_ARP
#define ETHERTYPE_ARP		0x0806	/* Addr. resolution protocol */
#endif
#ifndef ETHERTYPE_REVARP
#define ETHERTYPE_REVARP	0x8035	/* reverse Addr. resolution protocol */
#endif
#ifndef ETHERTYPE_NS
#define ETHERTYPE_NS		0x0600
#endif
#ifndef	ETHERTYPE_SPRITE
#define	ETHERTYPE_SPRITE	0x0500
#endif
#ifndef ETHERTYPE_TRAIL
#define ETHERTYPE_TRAIL		0x1000
#endif
#ifndef	ETHERTYPE_MOPDL
#define	ETHERTYPE_MOPDL		0x6001
#endif
#ifndef	ETHERTYPE_MOPRC
#define	ETHERTYPE_MOPRC		0x6002
#endif
#ifndef	ETHERTYPE_DN
#define	ETHERTYPE_DN		0x6003
#endif
#ifndef	ETHERTYPE_LAT
#define	ETHERTYPE_LAT		0x6004
#endif
#ifndef ETHERTYPE_SCA
#define ETHERTYPE_SCA		0x6007
#endif
#ifndef ETHERTYPE_TEB
#define ETHERTYPE_TEB		0x6558
#endif
#ifndef	ETHERTYPE_LANBRIDGE
#define	ETHERTYPE_LANBRIDGE	0x8038
#endif
#ifndef	ETHERTYPE_DECDNS
#define	ETHERTYPE_DECDNS	0x803c
#endif
#ifndef	ETHERTYPE_DECDTS
#define	ETHERTYPE_DECDTS	0x803e
#endif
#ifndef	ETHERTYPE_VEXP
#define	ETHERTYPE_VEXP		0x805b
#endif
#ifndef	ETHERTYPE_VPROD
#define	ETHERTYPE_VPROD		0x805c
#endif
#ifndef ETHERTYPE_ATALK
#define ETHERTYPE_ATALK		0x809b
#endif
#ifndef ETHERTYPE_AARP
#define ETHERTYPE_AARP		0x80f3
#endif
#ifndef	ETHERTYPE_TIPC
#define	ETHERTYPE_TIPC		0x88ca
#endif
#ifndef	ETHERTYPE_8021Q
#define	ETHERTYPE_8021Q		0x8100
#endif

/* see:
        http://en.wikipedia.org/wiki/IEEE_802.1Q
    and http://en.wikipedia.org/wiki/QinQ
*/
#ifndef	ETHERTYPE_8021Q9100
#define	ETHERTYPE_8021Q9100	0x9100
#endif
#ifndef	ETHERTYPE_8021Q9200
#define	ETHERTYPE_8021Q9200	0x9200
#endif
#ifndef	ETHERTYPE_8021QinQ
#define	ETHERTYPE_8021QinQ      0x88a8
#endif
#ifndef ETHERTYPE_IPX
#define ETHERTYPE_IPX		0x8137
#endif
#ifndef ETHERTYPE_IPV6
#define ETHERTYPE_IPV6		0x86dd
#endif
#ifndef ETHERTYPE_PPP
#define	ETHERTYPE_PPP		0x880b
#endif
#ifndef ETHERTYPE_MPCP
#define	ETHERTYPE_MPCP		0x8808
#endif
#ifndef ETHERTYPE_SLOW
#define	ETHERTYPE_SLOW		0x8809
#endif
#ifndef	ETHERTYPE_MPLS
#define	ETHERTYPE_MPLS		0x8847
#endif
#ifndef	ETHERTYPE_MPLS_MULTI
#define	ETHERTYPE_MPLS_MULTI	0x8848
#endif
#ifndef ETHERTYPE_PPPOED
#define ETHERTYPE_PPPOED	0x8863
#endif
#ifndef ETHERTYPE_PPPOES
#define ETHERTYPE_PPPOES	0x8864
#endif
#ifndef ETHERTYPE_PPPOED2
#define ETHERTYPE_PPPOED2	0x3c12
#endif
#ifndef ETHERTYPE_PPPOES2
#define ETHERTYPE_PPPOES2	0x3c13
#endif
#ifndef ETHERTYPE_MS_NLB_HB
#define ETHERTYPE_MS_NLB_HB	0x886f /* MS Network Load Balancing Heartbeat */
#endif
#ifndef ETHERTYPE_JUMBO
#define ETHERTYPE_JUMBO         0x8870
#endif
#ifndef ETHERTYPE_LLDP
#define ETHERTYPE_LLDP          0x88cc
#endif
#ifndef ETHERTYPE_EAPOL
#define ETHERTYPE_EAPOL  	0x888e
#endif
#ifndef ETHERTYPE_RRCP
#define ETHERTYPE_RRCP  	0x8899
#endif
#ifndef ETHERTYPE_AOE
#define ETHERTYPE_AOE  		0x88a2
#endif
#ifndef	ETHERTYPE_LOOPBACK
#define	ETHERTYPE_LOOPBACK	0x9000
#endif
#ifndef	ETHERTYPE_VMAN
#define	ETHERTYPE_VMAN	        0x9100 /* Extreme VMAN Protocol */
#endif
#ifndef	ETHERTYPE_CFM_OLD
#define	ETHERTYPE_CFM_OLD       0xabcd /* 802.1ag depreciated */
#endif
#ifndef	ETHERTYPE_CFM
#define	ETHERTYPE_CFM           0x8902 /* 802.1ag */
#endif
#ifndef	ETHERTYPE_IEEE1905_1
#define	ETHERTYPE_IEEE1905_1    0x893a /* IEEE 1905.1 */
#endif
#ifndef	ETHERTYPE_ISO
#define	ETHERTYPE_ISO           0xfefe  /* nonstandard - used in Cisco HDLC encapsulation */
#endif
#ifndef	ETHERTYPE_CALM_FAST
#define	ETHERTYPE_CALM_FAST     0x1111  /* ISO CALM FAST */
#endif
#ifndef	ETHERTYPE_GEONET_OLD
#define	ETHERTYPE_GEONET_OLD    0x0707  /* ETSI GeoNetworking (before Jan 2013) */
#endif
#ifndef	ETHERTYPE_GEONET
#define	ETHERTYPE_GEONET        0x8947  /* ETSI GeoNetworking (Official IEEE registration from Jan 2013) */
#endif

/* ethertype END */

/* Organizationally Unique Identifier(OUI) BEGIN */
#define OUI_ENCAP_ETHER 0x000000        /* encapsulated Ethernet */
#define OUI_CISCO       0x00000c        /* Cisco protocols */
#define OUI_NORTEL      0x000081        /* Nortel SONMP */
#define OUI_CISCO_90    0x0000f8        /* Cisco bridging */
#define OUI_RFC2684     0x0080c2        /* RFC 2427/2684 bridged Ethernet */
#define OUI_ATM_FORUM   0x00A03E        /* ATM Forum */
#define OUI_CABLE_BPDU  0x00E02F        /* DOCSIS spanning tree BPDU */
#define OUI_APPLETALK   0x080007        /* Appletalk */
#define OUI_JUNIPER     0x009069        /* Juniper */
#define OUI_HP          0x080009        /* Hewlett-Packard */
#define OUI_IEEE_8021_PRIVATE 0x0080c2      /* IEEE 802.1 Organisation Specific - Annex F */
#define OUI_IEEE_8023_PRIVATE 0x00120f      /* IEEE 802.3 Organisation Specific - Annex G */
#define OUI_TIA         0x0012bb        /* TIA - Telecommunications Industry Association - ANSI/TIA-1057- 2006 */
#define OUI_DCBX        0x001B21        /* DCBX */
#define OUI_NICIRA      0x002320        /* Nicira Networks */
#define OUI_BSN         0x5c16c7        /* Big Switch Networks */
#define OUI_VELLO       0xb0d2f5        /* Vello Systems */
#define OUI_HP2         0x002481        /* HP too */
#define OUI_HPLABS      0x0004ea        /* HP-Labs */
#define OUI_INFOBLOX    0x748771        /* Infoblox Inc */
#define OUI_ONLAB       0xa42305        /* Open Networking Lab */
#define OUI_FREESCALE   0x00049f        /* Freescale */
#define OUI_NETRONOME   0x0015ad        /* Netronome */
/* Organizationally Unique Identifier(OUI) END */

/* ip BEGIN */
/*
 * Definitions for internet protocol version 4.
 * Per RFC 791, September 1981.
 */
#define	IPVERSION4	4
#define	IPVERSION6	6

#define MAGIC_TCP   (0x06)
#define MAGIC_UDP   (0x11)


/*
 * Structure of an internet header, naked of options.
 *
 * We declare ip_len and ip_off to be short, rather than u_short
 * pragmatically since otherwise unsigned comparisons can result
 * against negative integers quite easily, and fail in subtle ways.
 */
struct ip {
	uint8_t		ip_vhl;		/* header length, version */
#define IP_V(ip)	(((ip)->ip_vhl & 0xf0) >> 4)
#define IP_HL(ip)	((ip)->ip_vhl & 0x0f)
	uint8_t		ip_tos;		/* type of service */
	uint16_t	ip_len;		/* total length */
	uint16_t	ip_id;		/* identification */
	uint16_t	ip_off;		/* fragment offset field */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	uint8_t		ip_ttl;		/* time to live */
	uint8_t		ip_p;		/* protocol */
	uint16_t	ip_sum;		/* checksum */
	uint32_t    ip_src;     /* source address */
	uint32_t    ip_dst;	    /* dest address */
} __attribute__((packed));

struct ip_print_demux_state {
	const struct ip *ip;
	const u_char *cp;
	u_int   len, off;
	u_char  nh;
	int     advance;
}__attribute__((packed));

#define	IP_MAXPACKET	65535		/* maximum packet size */

/*
 * Definitions for IP type of service (ip_tos)
 */
#define	IPTOS_LOWDELAY		0x10
#define	IPTOS_THROUGHPUT	0x08
#define	IPTOS_RELIABILITY	0x04

/*
 * Definitions for IP precedence (also in ip_tos) (hopefully unused)
 */
#define	IPTOS_PREC_NETCONTROL		0xe0
#define	IPTOS_PREC_INTERNETCONTROL	0xc0
#define	IPTOS_PREC_CRITIC_ECP		0xa0
#define	IPTOS_PREC_FLASHOVERRIDE	0x80
#define	IPTOS_PREC_FLASH		0x60
#define	IPTOS_PREC_IMMEDIATE		0x40
#define	IPTOS_PREC_PRIORITY		0x20
#define	IPTOS_PREC_ROUTINE		0x00

/*
 * Definitions for options.
 */
#define	IPOPT_COPIED(o)		((o)&0x80)
#define	IPOPT_CLASS(o)		((o)&0x60)
#define	IPOPT_NUMBER(o)		((o)&0x1f)

#define	IPOPT_CONTROL		0x00
#define	IPOPT_RESERVED1		0x20
#define	IPOPT_DEBMEAS		0x40
#define	IPOPT_RESERVED2		0x60

#define	IPOPT_EOL		0		/* end of option list */
#define	IPOPT_NOP		1		/* no operation */

#define	IPOPT_RR		7		/* record packet route */
#define	IPOPT_TS		68		/* timestamp */
#define	IPOPT_RFC1393           82              /* traceroute RFC 1393 */
#define	IPOPT_SECURITY		130		/* provide s,c,h,tcc */
#define	IPOPT_LSRR		131		/* loose source route */
#define	IPOPT_SATID		136		/* satnet id */
#define	IPOPT_SSRR		137		/* strict source route */
#define IPOPT_RA                148             /* router-alert, rfc2113 */

/*
 * Offsets to fields in options other than EOL and NOP.
 */
#define	IPOPT_OPTVAL		0		/* option ID */
#define	IPOPT_OLEN		1		/* option length */
#define IPOPT_OFFSET		2		/* offset within option */
#define	IPOPT_MINOFF		4		/* min value of above */

/*
 * Time stamp option structure.
 */
struct	ip_timestamp {
	uint8_t		ipt_code;	/* IPOPT_TS */
	uint8_t		ipt_len;	/* size of structure (variable) */
	uint8_t		ipt_ptr;	/* index of current entry */
	uint8_t		ipt_oflwflg;	/* flags, overflow counter */
#define IPTS_OFLW(ip)	(((ipt)->ipt_oflwflg & 0xf0) >> 4)
#define IPTS_FLG(ip)	((ipt)->ipt_oflwflg & 0x0f)
	union ipt_timestamp {
		uint32_t ipt_time[1];
		struct	ipt_ta {
			uint32_t ipt_addr;
			uint32_t ipt_time;
		} ipt_ta[1];
	} ipt_timestamp;
} __attribute__((packed));

/* flag bits for ipt_flg */
#define	IPOPT_TS_TSONLY		0		/* timestamps only */
#define	IPOPT_TS_TSANDADDR	1		/* timestamps and addresses */
#define	IPOPT_TS_PRESPEC	3		/* specified modules only */

/* bits for security (not byte swapped) */
#define	IPOPT_SECUR_UNCLASS	0x0000
#define	IPOPT_SECUR_CONFID	0xf135
#define	IPOPT_SECUR_EFTO	0x789a
#define	IPOPT_SECUR_MMMM	0xbc4d
#define	IPOPT_SECUR_RESTR	0xaf13
#define	IPOPT_SECUR_SECRET	0xd788
#define	IPOPT_SECUR_TOPSECRET	0x6bc5

/*
 * Internet implementation parameters.
 */
#define	MAXTTL		255		/* maximum time to live (seconds) */
#define	IPDEFTTL	64		/* default ttl, from RFC 1340 */
#define	IPFRAGTTL	60		/* time to live for frags, slowhz */
#define	IPTTLDEC	1		/* subtracted when forwarding */

#define	IP_MSS		576		/* default maximum segment size */
/* ip END */

/* tcp BEGIN */
typedef	uint32_t	tcp_seq;
/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */
struct tcphdr {
	uint16_t	th_sport;		/* source port */
	uint16_t	th_dport;		/* destination port */
	tcp_seq		th_seq;			/* sequence number */
	tcp_seq		th_ack;			/* acknowledgement number */
	uint8_t		th_offx2;		/* data offset, rsvd */
	uint8_t		th_flags;
	uint16_t	th_win;			/* window */
	uint16_t	th_sum;			/* checksum */
	uint16_t	th_urp;			/* urgent pointer */
} __attribute__((packed));

#define TH_OFF(th)	(((th)->th_offx2 & 0xf0) >> 4)

/* TCP flags */
#define	TH_FIN     0x01
#define	TH_SYN	   0x02
#define	TH_RST	   0x04
#define	TH_PUSH	   0x08
#define	TH_ACK	   0x10
#define	TH_URG	   0x20
#define TH_ECNECHO 0x40	/* ECN Echo */
#define TH_CWR	   0x80	/* ECN Cwnd Reduced */


#define	TCPOPT_EOL		    0
#define	TCPOPT_NOP		    1
#define	TCPOPT_MAXSEG		2
#define TCPOLEN_MAXSEG		4
#define	TCPOPT_WSCALE		3	/* window scale factor (rfc1323) */
#define	TCPOPT_SACKOK		4	/* selective ack ok (rfc2018) */
#define	TCPOPT_SACK		    5	/* selective ack (rfc2018) */
#define	TCPOPT_ECHO		    6	/* echo (rfc1072) */
#define	TCPOPT_ECHOREPLY	7	/* echo (rfc1072) */
#define TCPOPT_TIMESTAMP	8	/* timestamp (rfc1323) */
#define TCPOLEN_TIMESTAMP	10
#define TCPOLEN_TSTAMP_APPA		(TCPOLEN_TIMESTAMP+2) /* appendix A */
#define TCPOPT_CC		    11	/* T/TCP CC options (rfc1644) */
#define TCPOPT_CCNEW		12	/* T/TCP CC options (rfc1644) */
#define TCPOPT_CCECHO		13	/* T/TCP CC options (rfc1644) */
#define TCPOPT_SIGNATURE	19	/* Keyed MD5 (rfc2385) */
#define TCPOLEN_SIGNATURE	18
#define TCP_SIGLEN          16			/* length of an option 19 digest */
#define TCPOPT_AUTH         20      /* Enhanced AUTH option */
#define	TCPOPT_UTO		    28	/* tcp user timeout (rfc5482) */
#define	TCPOLEN_UTO			4
#define	TCPOPT_MPTCP		30	/* MPTCP options */
#define TCPOPT_FASTOPEN		34	/* TCP Fast Open (rfc7413) */
#define TCPOPT_EXPERIMENT2	254	/* experimental headers (rfc4727) */

#define TCPOPT_TSTAMP_HDR	\
    (TCPOPT_NOP<<24|TCPOPT_NOP<<16|TCPOPT_TIMESTAMP<<8|TCPOLEN_TIMESTAMP)

#ifndef TELNET_PORT
#define TELNET_PORT             23
#endif
#ifndef SMTP_PORT
#define SMTP_PORT		25
#endif
#ifndef BGP_PORT
#define BGP_PORT                179
#endif
#define NETBIOS_SSN_PORT        139
#ifndef OPENFLOW_PORT_OLD
#define OPENFLOW_PORT_OLD       6633
#endif
#ifndef OPENFLOW_PORT_IANA
#define OPENFLOW_PORT_IANA      6653
#endif
#ifndef PPTP_PORT
#define PPTP_PORT	        1723
#endif
#define BEEP_PORT               10288
#ifndef NFS_PORT
#define NFS_PORT	        2049
#endif
#define MSDP_PORT	        639
#define RPKI_RTR_PORT	        323
#define LDP_PORT                646
#ifndef SMB_PORT
#define SMB_PORT                445
#endif
#ifndef HTTP_PORT
#define HTTP_PORT		80
#endif
#ifndef HTTP_PORT_ALT
#define HTTP_PORT_ALT		8080
#endif
#ifndef RTSP_PORT
#define RTSP_PORT		554
#endif
#ifndef RTSP_PORT_ALT
#define RTSP_PORT_ALT		8554
#endif
#ifndef FTP_PORT
#define FTP_PORT		21
#endif
/* tcp END */

/* UDP BEGIN*/
/*
 * Udp protocol header.
 * Per RFC 768, September, 1981.
 */
struct udphdr {
	uint16_t	uh_sport;		/* source port */
	uint16_t	uh_dport;		/* destination port */
	uint16_t	uh_ulen;		/* udp length */
	uint16_t	uh_sum;			/* udp checksum */
};

/* UDP END */

uint32_t IEEE80211_extract_header_length(int16_t fc);
void IEEE80211_get_data_src_dst_mac(uint16_t fc, const uint8_t *p, uint8_t **srcp
	                                , uint8_t **dstp, uint8_t *is_station);


#endif /* _PROTOCOL_H_ */
