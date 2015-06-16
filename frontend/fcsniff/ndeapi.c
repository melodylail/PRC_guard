/**
 *****************************************************************************
 *
 *  (C) Copyright 2015-2015 ZERO HACKER.
 *  All Rights Reserved.
 *
 *  This program is an unpublished copyrighted work which is proprietary
 *  to Trend Micro, Inc. and contains confidential information that is not
 *  to be reproduced or disclosed to any other person or entity without
 *  prior written consent from Trend Micro, Inc. in each and every instance.
 *
 *  WARNING:  Unauthorized reproduction of this program as well as
 *  unauthorized preparation of derivative works based upon the
 *  program or distribution of copies by sale, rental, lease or
 *  lending are violations of federal copyright laws and state trade
 *  secret laws, punishable by civil and criminal penalties.
 *
 **/

#include "nde_types.h"
#include "protocol.h"
#include "nde_log.h"
#include "extract.h"
#include "common.h"
#include "ndeapi.h"
#include "scan_firechat.h"


int NDESetDebugLevel(NDE_DEBUG_LEVEL_T debugLevel)
{
    g_log_level = debugLevel;

    NLOG_ERR("NDE Debug Level : %d\n", g_log_level);

    return 0;
}

int NDESetLogCallback(NDE_DEBUG_LOG_CALLBACK pDebugLogCallbackFunc)
{
    if (0 != set_log_callback(pDebugLogCallbackFunc))
    {
        return PARA_ERR;
    }

    return 0;
}

int _NDE_Parse_IEEE80211_Layer(PacketData *p_packet_data, uint16_t *pframe_type)
{
    int rc = 0;
    uint16_t fc;

    /* skip packets smaller than a 802.11 header */

    /*if( p_packet_data->datalen < IEEE802_11_MIN_SIZE )
    {
        NLOG_INFO("_NDE_Parse_80211_Layer-> packet is too small. (<%d)\n", IEEE802_11_MIN_SIZE);
        rc = SKIP_ERR;

        return rc;
    }*/

    fc = EXTRACT_LE_16BITS(p_packet_data->data);
    *pframe_type = FC_TYPE(fc);

    return rc;
}

#ifndef ROUNDUP2
#define	ROUNDUP2(x, y)	(((x)+((y)-1))&(~((y)-1))) /* if y is powers of two */
#endif

int _NDE_Parse_IEEE80211_DATA_FRAME(PacketData *ppacket_data)
{
	uint16_t fc;
	uint32_t hdrlen;
	const uint8_t *src, *dst;

	fc = EXTRACT_LE_16BITS(ppacket_data->data);
	hdrlen = IEEE80211_extract_header_length(fc);

	if (hdrlen == 0) {
		/* Unknown frame type or control frame subtype; quit. */
		return PROTOCOL_ERR;
	}

	if (ppacket_data->datalen < hdrlen) {
		NLOG_DEBUG("_NDE_Parse_IEEE80211_DATA_FRAME-> datalen(%d) < hdrlen(%d)\n"
                , ppacket_data->datalen, hdrlen);

		return PROTOCOL_ERR;
	}

    if (DATA_FRAME_IS_NULL(FC_SUBTYPE(fc)))
    {
        NLOG_DEBUG("_NDE_Parse_IEEE80211_DATA_FRAME-> DATA_FRAME_IS_NULL, skip.\n");
        return SKIP_ERR;
    }

    /* There may be a problem w/ AP not having this bit set */
    if (FC_PROTECTED(fc)) {
        NLOG_DEBUG("_NDE_Parse_IEEE80211_DATA_FRAME-> FC_PROTECTED set, skip.\n");

        return SKIP_ERR;
    }

    IEEE80211_get_data_src_dst_mac(fc, ppacket_data->data
                                   , &(ppacket_data->src_mac)
                                   , &(ppacket_data->dst_mac)
                                   , &(ppacket_data->is_station));

    /*
	 * Go past the 802.11 header.
	 */
    /* frame has padding between 802.11 header and payload (to 32-bit boundary) */
    /*NLOG_DEBUG("_NDE_Parse_IEEE80211_DATA_FRAME-> 802.11 hdrlen: %d.\n", hdrlen);
    hdrlen = ROUNDUP2(hdrlen, 4);
    NLOG_DEBUG("_NDE_Parse_IEEE80211_DATA_FRAME-> 802.11 hdrlen after roundup2: %d.\n", hdrlen);*/
	ppacket_data->datalen -= hdrlen;
	ppacket_data->data += hdrlen;

	return SUCCESS;
}

/*
    |  24 or 30  |   1    |   1   |    1      |     3      |  2   |  nof fixed  |   4  |
    ------------------------------------------------------------------------------------
    | 802.11 MAC |   SNAP |  SNAP |  Control  |     OUI    | TYPE |  IP Packet  | FCS  |
    |   headers  |   DSAP |  SSAP |  0x03(UI) |  0x000000  |      |             |      |
    |            |   0xaa |  0xaa |           |            |      |             |      |
    ------------------------------------------------------------------------------------
*/

int _NDE_Parse_LLC_Layer(PacketData *ppacket_data, uint16_t *pethproto)
{
	uint8_t dsap_field, dsap, ssap_field, ssap;
	uint16_t control;
	uint32_t hdrlen;
	int is_u;
	uint32_t orgcode;
    register uint16_t et;
    const uint8_t *p;

	if (ppacket_data->datalen < LLC_MIN_LEN) {
	    NLOG_DEBUG("_NDE_Parse_LLC_Layer-> llc len too small: %d\n", ppacket_data->datalen);

		return PROTOCOL_ERR;
	}

	p = ppacket_data->data;

	dsap_field = *p;
	ssap_field = *(p + 1);

	/*
	 * OK, what type of LLC frame is this?  The length
	 * of the control field depends on that - I frames
	 * have a two-byte control field, and U frames have
	 * a one-byte control field.
	 */
	control = *(p + 2);

	if ((control & LLC_U_FMT) == LLC_U_FMT) {
		/*
		 * U frame.
		 */
		is_u = 1;
		hdrlen = 3;	/* DSAP, SSAP, 1-byte control field */
	}
	else
	{
		/*
		 * The control field in I and S frames is
		 * 2 bytes...
		 */
        NLOG_DEBUG("_NDE_Parse_LLC_Layer-> not 802.11 llc, control: %d, skip\n", control);

        return SKIP_ERR;
	}

	dsap = dsap_field & ~LLC_IG;
	ssap = ssap_field & ~LLC_GSAP;

	/*
	 * Skip LLC header.
	 */
	ppacket_data->data += hdrlen;
	ppacket_data->datalen -= hdrlen;
	p = ppacket_data->data;

	if (ssap == LLCSAP_SNAP && dsap == LLCSAP_SNAP
	    && control == LLC_UI)
    {
        if (ppacket_data->datalen < LLC_SNAP_LEN)
        {
            NLOG_DEBUG("_NDE_Parse_LLC_Layer-> snap is tunked.");

            return SKIP_ERR;
        }

        orgcode = EXTRACT_24BITS(p);
        *pethproto = EXTRACT_16BITS(p + 3);

        ppacket_data->data += LLC_SNAP_LEN;
	    ppacket_data->datalen -= LLC_SNAP_LEN;

        if(orgcode != OUI_ENCAP_ETHER)
        {
            NLOG_DEBUG("_NDE_Parse_LLC_Layer->SNAP OUI is not OUI_ENCAP_ETHER(0x000000)\n");

            return SKIP_ERR;
        }

        return SUCCESS;

	}
	else
	{
	    NLOG_DEBUG("_NDE_Parse_LLC_Layer-> not LLCSAP_SNAP && LLC_UI, skip\n");

	    return SKIP_ERR;
	}
}

int parse_udp_packet(PacketData *ppacket_data)
{
    register const struct udphdr *up;
    register const struct ip *ip;
    register u_char flags;
    register u_int hlen;
    register char ch;
    uint16_t sport, dport;

    up = (struct udphdr *)ppacket_data->data;
    ch = '\0';

    ppacket_data->src_port = EXTRACT_16BITS(&up->uh_sport);
    ppacket_data->dst_port = EXTRACT_16BITS(&up->uh_dport);

    hlen = EXTRACT_16BITS(&up->uh_ulen);

    if (hlen < sizeof(struct udphdr))
    {
        NLOG_DEBUG("parse_udp_packet-> udp [truncated length %u - too short, < %lu]\n",
                 hlen, (unsigned long)sizeof(struct udphdr));

        return PROTOCOL_ERR;
    }

    hlen -= sizeof(struct udphdr);
    ppacket_data->datalen -= sizeof(struct udphdr);

    if (hlen < ppacket_data->datalen)
    {
        ppacket_data->datalen = hlen;
    }

    ppacket_data->data += sizeof(struct udphdr);

    if (ppacket_data->datalen <= 0)
    {
        NLOG_DEBUG("parse_udp_packet-> udp payload length <=0,  %u\n", ppacket_data->datalen);

        return PROTOCOL_ERR;
    }

    ppacket_data->protocol = TYPE_IP_UDP;

    return SUCCESS;
}


int parse_tcp_packet(PacketData *ppacket_data)
{
    register const struct tcphdr *tp;
    register const struct ip *ip;
    register u_char flags;
    register u_int hlen;
    register char ch;
    uint16_t sport, dport;

    tp = (struct tcphdr *)ppacket_data->data;
    ch = '\0';

    ppacket_data->src_port = EXTRACT_16BITS(&tp->th_sport);
    ppacket_data->dst_port = EXTRACT_16BITS(&tp->th_dport);

    hlen = TH_OFF(tp) * sizeof(uint32_t);

    if (hlen < sizeof(*tp))
    {
        NLOG_DEBUG("parse_tcp_packet-> tcp [bad hdr length %u - too short, < %lu]\n",
                 hlen, (unsigned long)sizeof(*tp));

        return PROTOCOL_ERR;
    }

    if (hlen > ppacket_data->datalen)
    {
        NLOG_DEBUG("parse_tcp_packet-> [bad hdr length %u - too long, > %u]\n",
                hlen, ppacket_data->datalen);

        return PROTOCOL_ERR;
    }

    flags = tp->th_flags;
    ppacket_data->datalen -= hlen;
    ppacket_data->data += hlen;

    if (ppacket_data->datalen <= 0)
    {
        NLOG_DEBUG("parse_tcp_packet-> tcp payload length <=0,  %u\n", ppacket_data->datalen);

        return PROTOCOL_ERR;
    }

    ppacket_data->protocol = TYPE_IP_TCP;

    return SUCCESS;
}


int parse_ipv4_packet(PacketData *ppacket_data)
{
	uint32_t hlen;
	const struct ip *iph;
	uint16_t ip_total_len = 0;
	uint16_t frag_off = 0;
	int rc = SKIP_ERR;

	iph = (const struct ip *)(ppacket_data->data);

	if (IP_V(iph) != IPVERSION4)
	{
	    /* print version if != 4 */
	    if (IP_V(iph) == IPVERSION6)
	    {
	        NLOG_DEBUG("parse_ipv4_packet-> IPV6, wrong link-layer encapsulation.\n");
	    }
	    else
	    {
	        NLOG_DEBUG("parse_ipv4_packet-> IP%u \n", IP_V(iph));
	    }

        return SKIP_ERR;
	}

	if (ppacket_data->datalen < sizeof(struct ip))
	{
        NLOG_DEBUG("parse_ipv4_packet-> IP size(%d) error, truncated-ip.\n", ppacket_data->datalen);

        return SKIP_ERR;
    }


	hlen = IP_HL(iph) * sizeof(uint32_t);

	if (hlen < sizeof(struct ip)) {
		NLOG_DEBUG("parse_ipv4_packet-> bad ip header len:%u \n", hlen);

		return SKIP_ERR;
	}

	ip_total_len = EXTRACT_16BITS(&iph->ip_len);

	if (ppacket_data->datalen < ip_total_len)
	{
	    NLOG_DEBUG("parse_ipv4_packet-> truncated-ip - %u bytes missing! \n", ip_total_len - ppacket_data->datalen);
	}

	if (ip_total_len < hlen)
	{
	    NLOG_DEBUG("parse_ipv4_packet-> bad ip total len:%u \n", ip_total_len);

        return SKIP_ERR;
	}

	frag_off = EXTRACT_16BITS(&iph->ip_off);

	/*
	 * If this is fragment zero, hand it to the next higher
	 * level protocol.
	 */

    if (frag_off & (IP_MF | IP_OFFMASK))
    {
        NLOG_DEBUG("parse_ipv4_packet->Type: IP Fragment, skip.");

        return SKIP_ERR;
    }

    ppacket_data->protocol = TYPE_IP;
    ppacket_data->src_ip = (uint8_t *)(&iph->ip_src);
    ppacket_data->dst_ip = (uint8_t *)(&iph->ip_dst);
    ppacket_data->data += hlen;
    ppacket_data->datalen = ip_total_len - hlen;

    if(iph->ip_p == MAGIC_TCP)
    {
        rc = parse_tcp_packet(ppacket_data);
    }
    else if(iph->ip_p == MAGIC_UDP)
    {
        rc = parse_udp_packet(ppacket_data);
    }
    else
    {
        return SKIP_ERR;
    }

	return rc;
}


/* Identify Packet Type */
/*
 * Input: PacketData *pPacketData.
 * Output: PacketData *pPacketData.
 * Return: Successful : 0.
 *         failed: <0  (PARA_ERR,SKIP_ERR)
 */
int NDEIdentifyPacket(PacketData *pPacketData)
{
    int rc = SKIP_ERR;
    uint16_t frame_type = 0;
    uint16_t ethproto = 0;

    do
    {
        /* parse 802.11 layer */
        rc = _NDE_Parse_IEEE80211_Layer(pPacketData, &frame_type);

        if(SUCCESS != rc)
        {
            break;
        }

        /* only parse data frame */
        if(T_DATA != frame_type)
        {
            NLOG_DEBUG("NDEIdentifyPacket-> Not data frame, skip it.\n");
            break;
        }

        /* parse 802.11 data frame */
        rc = _NDE_Parse_IEEE80211_DATA_FRAME(pPacketData);

        if(SUCCESS != rc)
        {
            break;
        }

        /* parse 802.11 LLC layer */
        rc = _NDE_Parse_LLC_Layer(pPacketData, &ethproto);

        if(SUCCESS != rc)
        {
            break;
        }

        if(ETHERTYPE_IP != ethproto)
        {
            NLOG_DEBUG("NDEIdentifyPacket-> Not ip packet, skip it.\n");
            rc = SKIP_ERR;
            break;
        }

        /* parse ipv4 packet */
        rc = parse_ipv4_packet(pPacketData);
        break;

    }while(0);

    return rc;
}

int _NDEScanTcpPacket(PacketData *ppacket_data, NDEIdentifyResult *pnde_result)
{
    int rc = SUCCESS;

    /********* check QQ protocol ************/
    /*rc = scan_qq_packet(ppacket_data, pnde_result);

    if(rc > 0)
    {
        return rc;
    }*/

    /********* check WECHAT protocol ************/
    /*rc = scan_wechat_packet(ppacket_data, pnde_result);*/

    return rc;
}

int _NDEScanUdpPacket(PacketData *ppacket_data, NDEIdentifyResult *pnde_result)
{
    int rc = SUCCESS;

    /********* check firechat protocol ************/
    rc = scan_firechat_packet(ppacket_data, pnde_result);

    return rc;
}

int _NDEScanPacket(PacketData *ppacket_data, NDEIdentifyResult *pnde_result)
{
    int rc = SUCCESS;
    int cur_rc = SUCCESS;

    do
    {
        if(ppacket_data->protocol == TYPE_IP_TCP)
        {
            rc = _NDEScanTcpPacket(ppacket_data, pnde_result);
        }
        else if(ppacket_data->protocol == TYPE_IP_UDP)
        {
            rc = _NDEScanUdpPacket(ppacket_data, pnde_result);
        }
        else
        {
            rc = SKIP_ERR;
        }

    }while(0);

    return rc;
}

int NDEScanRawPacket(PacketData *pPacketData, NDEIdentifyResult *pNDEResult)
{
    int rc = 0;

    NLOG_DEBUG("NDEScanRawPacket-> begin to scan frame, data len: %d\n", pPacketData->datalen);

    if(NULL == pPacketData || NULL == pNDEResult)
    {
        NLOG_DEBUG("NDEScanRawPacket-> Invalid params.\n")
        return PARA_ERR;
    }

    do
    {
        /*after NDEIdentifyPacket(), packet type and related information fields such as packet
          type/header fields, data,end,...etc in _PacketData are set*/
        NLOG_DEBUG("NDEScanRawPacket-> begin to  NDEIdentifyPacket()\n");

        rc = NDEIdentifyPacket(pPacketData);
        if (rc != SUCCESS)
        {
            NLOG_DEBUG("NDEScanRawPacket->  NDEIdentifyPacket() != 0 (skip it)\n");
            break;
        }

        rc = _NDEScanPacket(pPacketData, pNDEResult);


    }while(0);

    NLOG_DEBUG("NDEScanRawPacket-> exit\n");

    return rc;
}
