#ifndef _NDE_API_H_
#define _NDE_API_H_

#include "nderror.h"
#include "avl.h"
#include "ring_buf.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @file ndeapi.h
 *  @brief Network Detection Engine API (NDEAPI)
 */

#define NDE_SCAN_TYPE_NONE          0x0000
#define NDE_SCAN_TYPE_QQ            0x0001
#define NDE_SCAN_TYPE_WECHAT        0x0002
#define NDE_SCAN_TYPE_TAOBAO        0x0004
#define NDE_SCAN_TYPE_SINAWEIBO     0x0008
#define NDE_SCAN_TYPE_FIRECHAT      0x0010


typedef struct _NDEContext
{
    int scan_type;
    AVL  *kAppDb;
    ring_buf_t task_rb;
}NDEContext;


typedef struct _PacketData
{
    int protocol;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t *src_mac;
    uint8_t *dst_mac;
    uint8_t *src_ip;
    uint8_t *dst_ip;
    uint8_t is_station;

    const uint8_t *data; /* Data head pointer   */
    uint32_t  datalen;
}PacketData;

#define NDE_ID_VALUE_MAX_LEN 2048
typedef struct _NDEIdentifyResult
{
    int type;
    int size;
    uint8_t value[NDE_ID_VALUE_MAX_LEN + 1];
}NDEIdentifyResult;


#define NDE_NONE                        0
#define NDE_IDENTIFY_FOUND              2

/* identification type */
#define NDE_ID_TYPE_FIRECHAT_MSG        1



/* Types of Scan Block */
#define TYPE_RAW        (0x00574152) /*RAW*/
#define TYPE_ETHER      (0x45485445) /*ETHE*/
#define TYPE_IP         (0x00005049) /*IP*/
#define TYPE_IP_TCP     (0x00504354) /*TCP*/
#define TYPE_IP_UDP     (0x00504455) /*UDP*/
#define TYPE_IP_ICMP    (0x504D4349) /*ICMP*/
#define TYPE_IP_ICMP6   (0x364D4349) /*ICM6*/
#define TYPE_ARP        (0x00505241) /*ARP*/



/* NetWork Scan Raw Packet */
/* if < 0    ; error occures.                               */
/* if == 0   : everything is OK.                            */
/* if > 0    : find some identification, need to record it. */

/** Scan Raw Packet
 *  @param[in,out] pPacketData
 *  @param[in,out] pPacketData
 *  @retval 0 PASS
 *  @retval >0 (NDE_IDENTIFY_FOUND, ...)
 *  @retval <0 failed (PARA_ERR,SKIP_ERR,...)
 *  @samplecode
        int rc = 0;
        rc = NDEScanRawPacket(pPacketData, pNDEResult);
        switch (rc) {
        case NDE_NONE:
            break;
        case NDE_IDENTIFY_FOUND:
            break;
        default:
            if (rc<0) {
                // scan error
            }
            break;
        }
 *  @endcode
 */
int NDEScanRawPacket(PacketData * pPacketData, NDEIdentifyResult *pNDEResult);


/* Debug Level */
typedef int     NDE_DEBUG_LEVEL_T;

#define NDE_DEBUG_LEVEL_DISABLE  (0x00)

#define NDE_DEBUG_LEVEL_ERR      (0x01)
#define NDE_DEBUG_LEVEL_WARN     (0x02)
#define NDE_DEBUG_LEVEL_INFO     (0x03)
#define NDE_DEBUG_LEVEL_DEBUG    (0x04)

/** Set Debug level
 *  @param[in] level
 *  @retval 0 successful
 *  @retval <0 failed (PARA_ERR)
 *  @samplecode
        NDE_DEBUG_LEVEL_T debugLevel = NDE_DEBUG_LEVEL_ERR;

        if (NSSetDebugLevel(pNSC, debugLevel) == 0) {
            // Set Successfully
        } else {
            // impossible error
        }
 *  @endcode
 */
int NDESetDebugLevel(NDE_DEBUG_LEVEL_T debugLevel);

/**
 * Type definition of the debugging callback function.
 *
 *  @param debugLevel   [in] The level of the logging message.
 *                           This value may include multiple levels, like NS_DEBUG_LEVEL_ERR, NS_DEBUG_LEVEL_WARN, etc.
 *
 *  @param cszMessage   [in] Log message
 *  @param cszFile      [in] Source file
 *  @param uiLine       [in] Line number of the source file
 *
 *  @samplecode
    void _debug_log_callback(NDE_DEBUG_LEVEL_T   debugLevel,
                             const char*        cszMessage)
    {
        if (debugLevel & NDE_DEBUG_LEVEL_DEBUG)
        {
            // Debug-Level message
        }
        else if (debugLevel & NDE_DEBUG_LEVEL_INFO)
        {
            // Info-Level message
        }
        else if (debugLevel & NDE_DEBUG_LEVEL_WARN)
        {
            // Warning-Level message
        }
        else if (debugLevel & NDE_DEBUG_LEVEL_ERR)
        {
            // Error-Level message
        }
    }

 *  @endcode
 */
typedef void (*NDE_DEBUG_LOG_CALLBACK)(NDE_DEBUG_LEVEL_T  debugLevel,
                                       const char*       cszMessage);

/**
 *  Init Log callback functions.
 *
 *  Usage:
 *    1. Suggest calling this function before all APIs to gather all logs
 *
 *  @param[in] pDebugLogCallbackFunc    The callback function for logging
 *
 *  @retval 0 successful
 *  @retval <0 failed (PARA_ERR)
 */
int NDESetLogCallback(NDE_DEBUG_LOG_CALLBACK pDebugLogCallbackFunc);


#ifdef __cplusplus
}
#endif

#endif /* _NDE_API_H_ */
