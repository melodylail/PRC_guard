#include "nde_types.h"
#include "common.h"
#include "nde_log.h"
#include "ieee80211.h"
#include "protocol.h"

uint32_t IEEE80211_extract_header_length(int16_t fc)
{
	uint32_t len = 0;

	switch (FC_TYPE(fc)) {
	case T_MGMT:
		return MGMT_HDRLEN;
	case T_CTRL:
		switch (FC_SUBTYPE(fc)) {
		case CTRL_CONTROL_WRAPPER:
			return CTRL_CONTROL_WRAPPER_HDRLEN;
		case CTRL_BAR:
			return CTRL_BAR_HDRLEN;
		case CTRL_BA:
			return CTRL_BA_HDRLEN;
		case CTRL_PS_POLL:
			return CTRL_PS_POLL_HDRLEN;
		case CTRL_RTS:
			return CTRL_RTS_HDRLEN;
		case CTRL_CTS:
			return CTRL_CTS_HDRLEN;
		case CTRL_ACK:
			return CTRL_ACK_HDRLEN;
		case CTRL_CF_END:
			return CTRL_END_HDRLEN;
		case CTRL_END_ACK:
			return CTRL_END_ACK_HDRLEN;
		default:
			NLOG_ERR("extract_IEEE80211_header_length-> unknown 802.11 ctrl frame subtype (%d)\n"
                    , FC_SUBTYPE(fc));
			return PROTOCOL_ERR;
		}
	case T_DATA:
		len = (FC_TO_DS(fc) && FC_FROM_DS(fc)) ? 30 : 24;

		if (DATA_FRAME_IS_QOS(FC_SUBTYPE(fc)))
		{
		    len += 2;
		}

		return len;
	default:
		NLOG_ERR("extract_IEEE80211_header_length-> unknown 802.11 frame type (%d)\n"
                , FC_TYPE(fc));
		return PROTOCOL_ERR;
	}
}


/*
 *  Data Frame - Address field contents
 *
 *  To Ds  | From DS | Addr 1 | Addr 2 | Addr 3 | Addr 4
 *    0    |  0      |  DA    | SA     | BSSID  | n/a       STA->STA
 *    0    |  1      |  DA    | BSSID  | SA     | n/a       AP ->STA
 *    1    |  0      |  BSSID | SA     | DA     | n/a       STA->AP
 *    1    |  1      |  RA    | TA     | DA     | SA        AP ->AP
 */

/*
 * Function to get source and destination MAC addresses for a data frame.
 */
void IEEE80211_get_data_src_dst_mac(uint16_t fc, const uint8_t *p, uint8_t **srcp, uint8_t **dstp, uint8_t *is_station)
{
#define ADDR1  (p + 4)
#define ADDR2  (p + 10)
#define ADDR3  (p + 16)
#define ADDR4  (p + 24)

	if (!FC_TO_DS(fc) && !FC_FROM_DS(fc)) {
		*srcp = ADDR2;
		*dstp = ADDR1;
		*is_station = 1;
	} else if (!FC_TO_DS(fc) && FC_FROM_DS(fc)) {
		*srcp = ADDR3;
		*dstp = ADDR1;
		*is_station = 0;
	} else if (FC_TO_DS(fc) && !FC_FROM_DS(fc)) {
		*srcp = ADDR2;
		*dstp = ADDR3;
		*is_station = 1;
	} else if (FC_TO_DS(fc) && FC_FROM_DS(fc)) {
		*srcp = ADDR4;
		*dstp = ADDR3;
		*is_station = 0;
	}

#undef ADDR1
#undef ADDR2
#undef ADDR3
#undef ADDR4
}

