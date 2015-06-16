#include "nde_types.h"
#include "common.h"
#include "nde_log.h"
#include "extract.h"
#include "bmhsrch.h"
#include "scan_firechat.h"


#define IP_V4_LEN  4

uint8_t  FIRECHAT_IM_DST_IP[IP_V4_LEN] = {0xef, 0xc0, 0x00, 0x00}; /* 239.192.0.0 */
uint16_t FIRECHAT_IM_SRC_PORT = 7576;
uint16_t FIRECHAT_IM_DST_PORT = 7576;


#define FIRECHAT_PAYLOAD_MIN_LEN 20


int scan_firechat_packet(PacketData * ppacket_data, NDEIdentifyResult *pnde_result)
{
    int rc = SUCCESS;
    int i = 0;
    uint32_t payload_len = 0;
    uint8_t *position;
    char encode[256*2+256 + 1] = {0};
    char buffer[257] = {0};
    int length = 0;
    uint8_t offset = 0;
    uint8_t msg_len = 0;

    uint8_t *data = ppacket_data->data;
    uint32_t datalen = ppacket_data->datalen;

    NLOG_DEBUG("scan_firechat_packet-> Begin scan_firechat_packet...\n");

    do
    {
        if(datalen <= FIRECHAT_PAYLOAD_MIN_LEN)
        {
            NLOG_DEBUG("scan_firechat_packet-> payload len too small(%u).\n", datalen);
            rc = SUCCESS;
            break;
        }

        /* fileter port 7576 and 7576 */
        if(ppacket_data->dst_port == FIRECHAT_IM_DST_PORT 
            &&  ppacket_data->src_port == FIRECHAT_IM_SRC_PORT
            && memcmp(ppacket_data->dst_ip, FIRECHAT_IM_DST_IP, IP_V4_LEN) == 0)
        {
            NLOG_I_DUMP_HEX(data, datalen);
            memset(pnde_result, 0x00, sizeof(NDEIdentifyResult));

            /* decode msg */
            char *pstr = encode;

            for(i = 0; i < datalen; i++)
            {
                sprintf(pstr, "%%%2x", *((uint8_t *)(data + i)));
                pstr += 3;
            }

            *pstr = '\0';
            char mac_str[20] = {0};
            mac_to_str(ppacket_data->src_mac, mac_str);
            sprintf((char *)pnde_result->value, "MAC[%s] -- ", mac_str);
            pnde_result->size = strlen((char *)pnde_result->value);
            NLOG_WARN("MAC[%s] -- ", mac_str);
            urldecode(encode, (int)(pstr - encode), buffer, length);
            NLOG_WARN("[%s]\n", buffer);
            sprintf((char *)pnde_result->value + pnde_result->size, "[%s]\n", buffer);
            pnde_result->size = strlen((char *)pnde_result->value);
            
            rc = NDE_IDENTIFY_FOUND;
        }
     
    }while(0);


    return rc;
}
