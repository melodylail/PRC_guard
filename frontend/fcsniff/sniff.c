#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <getopt.h>
#include <fcntl.h>

#include "nde_types.h"
#include "pcap.h"
#include "osdep/osdep.h"
#include "osdep/common.h"
#include "common.h"
#include "extract.h"
#include "protocol.h"
#include "nde_log.h"
#include "ndeapi.h"
#include "nderror.h"
#include "sniff.h"
#include "dump.h"
#include "curl/curl.h"

char g_wifi_dump_filename[MAXLEN_FILENAME + 1];
int g_wifi_dump_count = 0;

char * g_upload_file_url = "http://127.0.0.1/upload.bin";


/* setup the output files */
int dump_initialize( char *prefix)
{
    int ofn_len;
    char * ofn = NULL;

    /* If you only want to see what happening, send all data to /dev/null */
    if ( prefix == NULL || strlen( prefix ) == 0)
    {
        NLOG_ERR("dump_initialize->Invalid prefix\n");
        return -1;
    }

    /* Create a buffer of the length of the prefix + extension ("xxxx") + terminating 0. */
    ofn_len = strlen(prefix) + strlen(DUMP_CAP_EXT) + 8;
    ofn = (char *)calloc(1, ofn_len);
    g_config.prefix = (char *) malloc(strlen(prefix) + 1);
    memcpy(g_config.prefix, prefix, strlen(prefix) + 1);

    /* create the output packet capture file */
    if( g_config.output_format_pcap )
    {
        struct pcap_file_header pfh;

        memset(ofn, 0, ofn_len);
        snprintf( ofn,  ofn_len, "%s.%s", prefix, DUMP_CAP_EXT );

        if( ( g_config.f_cap = fopen( ofn, "wb+" ) ) == NULL )
        {
            NLOG_ERR("Could not create \"%s\".\n", ofn );
            free( ofn );
            return -1;
        }

        g_config.f_cap_name = (char *) malloc( strlen( ofn ) + 1 );
        memcpy( g_config.f_cap_name, ofn, strlen( ofn ) + 1 );
        free( ofn );

        pfh.magic           = TCPDUMP_MAGIC;
        pfh.version_major   = PCAP_VERSION_MAJOR;
        pfh.version_minor   = PCAP_VERSION_MINOR;
        pfh.thiszone        = 0;
        pfh.sigfigs         = 0;
        pfh.snaplen         = 65535;
        pfh.linktype        = LINKTYPE_IEEE802_11;

        if( fwrite( &pfh, 1, sizeof( pfh ), g_config.f_cap ) !=
                (size_t) sizeof( pfh ) )
        {
            NLOG_ERR("fwrite(pcap file header) failed\n" );
            return -1;
        }
    }

    return 0;
}


int dump_to_file(unsigned char *h80211, int caplen)
{
    struct pcap_pkthdr pkh;
    struct timeval tv;
    size_t n = 0;

    if( g_config.f_cap != NULL && caplen >= 10)
    {
        pkh.caplen = pkh.len = caplen;

        gettimeofday( &tv, NULL );

        pkh.tv_sec  =   tv.tv_sec;
        pkh.tv_usec = ( tv.tv_usec & ~0x1ff ) + 64;

        n = sizeof( pkh );

        if( fwrite( &pkh, 1, n, g_config.f_cap ) != n )
        {
            printf( "dump_to_file-> Failed to fwrite(packet header) failed" );
            return -1;
        }

        fflush( stdout );

        n = (size_t)pkh.caplen;

        if( fwrite( h80211, 1, n, g_config.f_cap ) != n )
        {
            printf( "dump_to_file-> Failed to fwrite(packet data) failed" );
            return -1;
        }

        fflush( stdout );
    }

    return 0;
}

int upload_file_to_server(const char *filepath)
{
    CURL *curl;
    CURLcode res;
    struct curl_httppost *post = NULL;
    struct curl_httppost *last = NULL;
    time_t tv;
    struct tm *tmv;
    char filename[32];
    int ret = -1;

    memset(filename, 0x00, sizeof(filename));
    time(&tv);
    tmv = localtime(&tv);
    sprintf(filename, "./tmp/%d%02d%02d%d%d%d_101.rpt", tmv->tm_year + 1900, tmv->tm_mon + 1
                    , tmv->tm_mday, tmv->tm_hour, tmv->tm_min, tmv->tm_sec);

    NLOG_DEBUG("upload_file_to_server-> generate filename %s\n", filename);

    do
    {
        if(rename(filepath, filename) != 0)
        {
            NLOG_ERR("upload_file_to_server-> Failed to rename %s to %s, errno[%d]\n"
                     , filepath, filename, errno);
            ret = -1;
            break;
        }

        curl = curl_easy_init();

        if(curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, g_upload_file_url);

            curl_formadd(&post,
                         &last,
                         CURLFORM_COPYNAME, "file",
                         CURLFORM_FILE, filename,
                         CURLFORM_END);
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);

            res = curl_easy_perform(curl);

            if(res != CURLE_OK)
            {
                NLOG_ERR("upload_file_to_server-> Failed to upload file %s, err[%d]\n", filename, res);
                ret = -1;
            }
            else
            {
                NLOG_DEBUG("upload_file_to_server-> Success to upload file %s\n", filename);
                ret = 0;
                remove(filename);
            }

            curl_formfree(post);
            curl_easy_cleanup(curl);
        }

    }while(0);


    return ret;    
}

int handle_packet( unsigned char *h80211, int caplen)
{
    int rc = SUCCESS;
    int i = 0;

    PacketData packet_data;
    NDEIdentifyResult result;

    packet_data.data = h80211;
    packet_data.datalen = caplen;

    rc = NDEScanRawPacket(&packet_data, &result);

    switch (rc)
    {
    case NDE_NONE:
        break;
    case NDE_IDENTIFY_FOUND:

        dump_one_record(g_wifi_dump_filename , "%s", (char *)result.value);
        g_wifi_dump_count += 1;

        if(g_wifi_dump_count > 1)
        {
            /* upload to backend server */
            if(upload_file_to_server(g_wifi_dump_filename) == 0)
            {
                remove(g_wifi_dump_filename);
            }

            g_wifi_dump_count = 0;
        }

        //dump_to_file(h80211, caplen);

        break;
    default:
        if (rc<0)
        {
            /* scan error*/
        }
        break;
    }

    return rc;
}

int getchancount(int valid)
{
    int i=0, chan_count=0;

    while(g_config.channels[i])
    {
        i++;
        if(g_config.channels[i] != -1)
            chan_count++;
    }

    if(valid) return chan_count;
    return i;
}

int getfreqcount(int valid)
{
    int i=0, freq_count=0;

    while(g_config.own_frequencies[i])
    {
        i++;
        if(g_config.own_frequencies[i] != -1)
            freq_count++;
    }

    if(valid) return freq_count;
    return i;
}

void channel_hopper(struct wif *wi, int chan_count )
{
    int ch, ch_idx = 0, chi=0;
    int dropped=0;

    while( getppid() != 1 )
    {
        ch_idx = chi % chan_count;
        ++chi;

        if( g_config.channels[ch_idx] == -1 )
        {
            dropped++;

            if(dropped >= chan_count)
            {
                ch = wi_get_channel(wi);
                g_config.channel = ch;

                usleep(1000);
            }
            continue;
        }

        dropped = 0;

        ch = g_config.channels[ch_idx];

        if(wi_set_channel(wi, ch) == 0 )
        {
            usleep(1000);
        }
        else
        {
            g_config.channels[ch_idx] = -1;      /* remove invalid channel */
            continue;
        }

        usleep( (g_config.hopfreq*1000) );
    }

    exit( 0 );
}

void frequency_hopper(struct wif *wi, int chan_count )
{
    ssize_t unused;
    int ch, ch_idx = 0, chi=0;
    int dropped=0;

    while( getppid() != 1 )
    {
        {
            ch_idx = chi % chan_count;
            ++chi;

            if( g_config.own_frequencies[ch_idx] == -1 )
            {
                dropped++;

                if(dropped >= chan_count)
                {
                    ch = wi_get_freq(wi);
                    g_config.frequency = ch;
                    usleep(1000);
                }

                continue;
            }

            dropped = 0;

            ch = g_config.own_frequencies[ch_idx];

            if(wi_set_freq(wi, ch ) == 0 )
            {
                g_config.frequency = ch;
                usleep(1000);
            }
            else
            {
                g_config.own_frequencies[ch_idx] = -1;      /* remove invalid channel */

                continue;
            }
        }

        usleep( (g_config.hopfreq*1000) );
    }

    exit( 0 );
}

int invalid_channel(int chan)
{
    int i=0;

    do
    {
        if (chan == abg_chans[i] && chan != 0 )
            return 0;
    }
    while (abg_chans[++i]);

    return 1;
}

int invalid_frequency(int freq)
{
    int i=0;

    do
    {
        if (freq == frequencies[i] && freq != 0 )
            return 0;
    }
    while (frequencies[++i]);

    return 1;
}

/* parse a string, for example "1,2,3-7,11" */

int getchannels(const char *optarg)
{
    unsigned int i=0,chan_cur=0,chan_first=0,chan_last=0,chan_max=128,chan_remain=0;
    char *optchan = NULL, *optc;
    char *token = NULL;
    int *tmp_channels;

    //got a NULL pointer?
    if(optarg == NULL)
        return -1;

    chan_remain=chan_max;

    //create a writable string
    optc = optchan = (char*) malloc(strlen(optarg)+1);
    strncpy(optchan, optarg, strlen(optarg));
    optchan[strlen(optarg)]='\0';

    tmp_channels = (int*) malloc(sizeof(int)*(chan_max+1));

    //split string in tokens, separated by ','
    while( (token = strsep(&optchan,",")) != NULL)
    {
        //range defined?
        if(strchr(token, '-') != NULL)
        {
            //only 1 '-' ?
            if(strchr(token, '-') == strrchr(token, '-'))
            {
                //are there any illegal characters?
                for(i=0; i<strlen(token); i++)
                {
                    if( (token[i] < '0') && (token[i] > '9') && (token[i] != '-'))
                    {
                        free(tmp_channels);
                        free(optc);
                        return -1;
                    }
                }

                if( sscanf(token, "%d-%d", &chan_first, &chan_last) != EOF )
                {
                    if(chan_first > chan_last)
                    {
                        free(tmp_channels);
                        free(optc);
                        return -1;
                    }
                    for(i=chan_first; i<=chan_last; i++)
                    {
                        if( (! invalid_channel(i)) && (chan_remain > 0) )
                        {
                            tmp_channels[chan_max-chan_remain]=i;
                            chan_remain--;
                        }
                    }
                }
                else
                {
                    free(tmp_channels);
                    free(optc);
                    return -1;
                }

            }
            else
            {
                free(tmp_channels);
                free(optc);
                return -1;
            }
        }
        else
        {
            //are there any illegal characters?
            for(i=0; i<strlen(token); i++)
            {
                if( (token[i] < '0') && (token[i] > '9') )
                {
                    free(tmp_channels);
                    free(optc);
                    return -1;
                }
            }

            if( sscanf(token, "%d", &chan_cur) != EOF)
            {
                if( (! invalid_channel(chan_cur)) && (chan_remain > 0) )
                {
                    tmp_channels[chan_max-chan_remain]=chan_cur;
                    chan_remain--;
                }

            }
            else
            {
                free(tmp_channels);
                free(optc);
                return -1;
            }
        }
    }

    g_config.own_channels = (int*) malloc(sizeof(int)*(chan_max - chan_remain + 1));

    for(i=0; i<(chan_max - chan_remain); i++)
    {
        g_config.own_channels[i]=tmp_channels[i];
    }

    g_config.own_channels[i]=0;

    free(tmp_channels);
    free(optc);
    if(i==1) return g_config.own_channels[0];
    if(i==0) return -1;
    return 0;
}

/* parse a string, for example "1,2,3-7,11" */

int getfrequencies(const char *optarg)
{
    unsigned int i=0,freq_cur=0,freq_first=0,freq_last=0,freq_max=10000,freq_remain=0;
    char *optfreq = NULL, *optc;
    char *token = NULL;
    int *tmp_frequencies;

    //got a NULL pointer?
    if(optarg == NULL)
        return -1;

    freq_remain=freq_max;

    //create a writable string
    optc = optfreq = (char*) malloc(strlen(optarg)+1);
    strncpy(optfreq, optarg, strlen(optarg));
    optfreq[strlen(optarg)]='\0';

    tmp_frequencies = (int*) malloc(sizeof(int)*(freq_max+1));

    //split string in tokens, separated by ','
    while( (token = strsep(&optfreq,",")) != NULL)
    {
        //range defined?
        if(strchr(token, '-') != NULL)
        {
            //only 1 '-' ?
            if(strchr(token, '-') == strrchr(token, '-'))
            {
                //are there any illegal characters?
                for(i=0; i<strlen(token); i++)
                {
                    if( (token[i] < '0' || token[i] > '9') && (token[i] != '-'))
                    {
                        free(tmp_frequencies);
                        free(optc);
                        return -1;
                    }
                }

                if( sscanf(token, "%d-%d", &freq_first, &freq_last) != EOF )
                {
                    if(freq_first > freq_last)
                    {
                        free(tmp_frequencies);
                        free(optc);
                        return -1;
                    }
                    for(i=freq_first; i<=freq_last; i++)
                    {
                        if( (! invalid_frequency(i)) && (freq_remain > 0) )
                        {
                            tmp_frequencies[freq_max-freq_remain]=i;
                            freq_remain--;
                        }
                    }
                }
                else
                {
                    free(tmp_frequencies);
                    free(optc);
                    return -1;
                }

            }
            else
            {
                free(tmp_frequencies);
                free(optc);
                return -1;
            }
        }
        else
        {
            //are there any illegal characters?
            for(i=0; i<strlen(token); i++)
            {
                if( (token[i] < '0') && (token[i] > '9') )
                {
                    free(tmp_frequencies);
                    free(optc);
                    return -1;
                }
            }

            if( sscanf(token, "%d", &freq_cur) != EOF)
            {
                if( (! invalid_frequency(freq_cur)) && (freq_remain > 0) )
                {
                    tmp_frequencies[freq_max-freq_remain]=freq_cur;
                    freq_remain--;
                }

                /* special case "-C 0" means: scan all available frequencies */
                if(freq_cur == 0)
                {
                    freq_first = 1;
                    freq_last = 9999;
                    for(i=freq_first; i<=freq_last; i++)
                    {
                        if( (! invalid_frequency(i)) && (freq_remain > 0) )
                        {
                            tmp_frequencies[freq_max-freq_remain]=i;
                            freq_remain--;
                        }
                    }
                }

            }
            else
            {
                free(tmp_frequencies);
                free(optc);
                return -1;
            }
        }
    }

    g_config.own_frequencies = (int*) malloc(sizeof(int)*(freq_max - freq_remain + 1));

    for(i=0; i<(freq_max - freq_remain); i++)
    {
        g_config.own_frequencies[i]=tmp_frequencies[i];
    }

    g_config.own_frequencies[i]=0;

    free(tmp_frequencies);
    free(optc);
    if(i==1) return g_config.own_frequencies[0];   //exactly 1 frequency given
    if(i==0) return -1;                     //error occured
    return 0;                               //frequency hopping
}


int init_card(const char* iface, struct wif **wi)
{
    struct wif *wis;

    wis = wi_open((char *)iface);

    if (!wis)
    {
        NLOG_ERR("Failed to init card.\n");
        return -1;
    }

    *wi = wis;

    return 0;
}


int check_monitor(struct wif *wi, int *fd_raw)
{
    int monitor;
    char ifname[64];

    monitor = wi_get_monitor(wi);

    if(monitor != 0)
    {
        NLOG_INFO("%s reset to monitor mode\n", wi_get_ifname(wi));
        //reopen in monitor mode

        strncpy(ifname, wi_get_ifname(wi), sizeof(ifname)-1);
        ifname[sizeof(ifname)-1] = 0;

        wi_close(wi);
        wi = wi_open(ifname);
        if (!wi)
        {
            NLOG_ERR("Can't reopen %s\n", ifname);
            exit(1);
        }

        *fd_raw = wi_fd(wi);
    }

    return 0;
}

int check_channel(struct wif *wi)
{
    int chan;

    chan = wi_get_channel(wi);

    if(g_config.channel != chan)
    {
        NLOG_INFO("fixed channel %s: %d \n", wi_get_ifname(wi), chan);
        wi_set_channel(wi, g_config.channel);
    }

    return 0;
}

int check_frequency(struct wif *wi)
{
    int freq;

    freq = wi_get_freq(wi);
    if(freq < 0) return -1;

    if(g_config.frequency != freq)
    {
        NLOG_INFO("fixed frequency %s: %d \n", wi_get_ifname(wi), freq);
        wi_set_freq(wi, g_config.frequency);
    }

    return 0;
}

int detect_frequencies(struct wif *wi)
{
    int start_freq = 2192;
    int end_freq = 2732;
    int max_freq_num = 2048; //should be enough to keep all available channels
    int freq=0, i=0;

    NLOG_INFO("Checking available frequencies, this could take few seconds.\n");

    frequencies = (int*) malloc((max_freq_num+1) * sizeof(int)); //field for frequencies supported
    memset(frequencies, 0, (max_freq_num+1) * sizeof(int));
    for(freq=start_freq; freq<=end_freq; freq+=5)
    {
        if(wi_set_freq(wi, freq) == 0)
        {
            frequencies[i] = freq;
            i++;
        }
        if(freq == 2482)
        {
            //special case for chan 14, as its 12MHz away from 13, not 5MHz
            freq = 2484;
            if(wi_set_freq(wi, freq) == 0)
            {
                frequencies[i] = freq;
                i++;
            }
            freq = 2482;
        }
    }

    //again for 5GHz channels
    start_freq=4800;
    end_freq=6000;
    for(freq=start_freq; freq<=end_freq; freq+=5)
    {
        if(wi_set_freq(wi, freq) == 0)
        {
            frequencies[i] = freq;
            i++;
        }
    }

    NLOG_INFO("detect_frequencies Done.\n");
    return 0;
}

char usage[] =

    "\n"
    "  1.0 - (C) 2015-2015 mike zhuang\n"
    "  By default, fcsniff hop on 2.4GHz channels.\n"
    "\n"
    "  usage: fcsniff <options> <interface>\n"
    "\n"
    "  Options:\n"
    "      -r                <file> : Read packets from that file\n"
    "      -w              <prefix> : Dump file prefix, format-cap\n"
    "      -h                       : Displays this usage screen\n"
    "      -c <channels>            : Capture on specific channels\n"
    "      -d <debug level>         : Debug level[0-disabled, 1-error, 2-warn, 3-info, 4-debug]\n"
    "\n";


int main( int argc, char *argv[] )
{
    long cycle_time;
    int caplen=0, fd_is_set, chan_count;
    int fd_raw;
    int option = 0;
    char ifnam[64];
    int wi_read_failed=0;
    int n = 0;
    int ret = -1;
    struct pcap_pkthdr pkh;
    time_t tt_org;

    struct wif	       *wi;
    struct rx_info     ri;
    unsigned char      buffer[4096];
    unsigned char      tmpbuf[4096];
    unsigned char      *h80211;

    struct timeval     timeout;
    struct timeval     tv_now;
    struct timeval     tv_org;
    fd_set             rfds;
    int log_level = NDE_DEBUG_LEVEL_WARN;

    /* initialize a bunch of variables */
    srand( time( NULL ) );
    memset( &g_config, 0x00, sizeof(g_config) );
    h80211         =  NULL;
    g_config.chanoption   =  0;
    g_config.freqoption   =  0;
    fd_is_set	   =  0;
    chan_count	   =  0;
    g_config.channels     =  bg_chans;
    g_config.singlechan   =  0;
    g_config.singlefreq   =  0;
    g_config.dump_prefix  =  NULL;
    g_config.record_data  =  0;
    g_config.f_cap        =  NULL;
    g_config.prefix       =  NULL;
    g_config.hopfreq      =  DEFAULT_HOPFREQ;
    g_config.s_file       =  NULL;
    g_config.s_iface      =  NULL;
    g_config.f_cap_in     =  NULL;
    g_config.output_format_pcap = 1;

    memset(&g_config.pfh_in, '\x00', sizeof(struct pcap_file_header));

    gettimeofday( &timeout, NULL );

    fd_raw = -1;
    g_config.channel = 0;
    g_wifi_dump_count = 0;
    memset(g_wifi_dump_filename, 0x00, sizeof(g_wifi_dump_filename));
    sprintf(g_wifi_dump_filename, "./firechat_wifi_cap.txt");

    NDESetDebugLevel(log_level);

    /* check the arguments */
    do
    {
        option = getopt( argc, argv, "c:r:w:d:h");

        if( option < 0 ) break;

        switch( option )
        {
        case 0 :

            break;

        case ':':

            NLOG_ERR("\"%s -h\" for help.\n", argv[0]);
            return -1;

        case '?':

            NLOG_ERR("\"%s -h\" for help.\n", argv[0]);
            return -1;

        case 'w':

            if (g_config.dump_prefix != NULL)
            {
                NLOG_WARN( "Notice: dump prefix already given\n" );
                break;
            }
            /* Write prefix */
            g_config.dump_prefix   = optarg;
            g_config.record_data = 1;
            break;

        case 'r' :

            if( g_config.s_file )
            {
                NLOG_ERR( "Packet source already specified.\n" );
                NLOG_ERR("\"%s -h\" for help.\n", argv[0]);
                return -1;
            }
            g_config.s_file = optarg;
            break;
        case 'c' :

            if (g_config.channel > 0 || g_config.chanoption == 1)
            {
                if (g_config.chanoption == 1)
                {
                    NLOG_ERR( "Notice: Channel range already given\n" );
                }
                else
                {
                    NLOG_ERR( "Notice: Channel already given (%d)\n", g_config.channel);
                }
                break;
            }

            g_config.channel = getchannels(optarg);

            if ( g_config.channel < 0 )
            {
                NLOG_INFO( usage);
                return -1;
            }

            g_config.chanoption = 1;
            /*NLOG_INFO("sniff on channel: %d\n", g_config.channel);*/

            /* more than 1 channel set, need to hopper */
            if( g_config.channel == 0 )
            {
                g_config.channels = g_config.own_channels;
                break;
            }
            g_config.channels = bg_chans;
            break;

        case 'd':

            log_level = atoi(optarg);
            NDESetDebugLevel(log_level);
            break;

        case 'h':

            NLOG_ERR( usage);
            return -1;

        default :
            NLOG_ERR( usage);
            return -1;
        }
    }
    while ( 1 );

    if( argc - optind != 1 && g_config.s_file == NULL)
    {
        if(argc == 1)
        {
            NLOG_INFO( usage );
        }
        if( argc - optind == 0)
        {
            NLOG_ERR("No interface specified.\n");
        }
        if(argc > 1)
        {
            NLOG_INFO("\"%s -h\" for help.\n", argv[0]);
        }

        return -1;
    }

    if( argc - optind == 1 )
    {
        g_config.s_iface = argv[argc-1];
        NLOG_INFO("Sniffing on: %s\n", g_config.s_iface);
    }

    /* init dump file */
    /*memset(&g_wifi_dump_ctx, 0x00, sizeof(g_wifi_dump_ctx));
    sprintf(g_wifi_dump_ctx.filename, "./firechat_wifi.txt");
    
    if(dump_init(&g_wifi_dump_ctx) < 0)
    {
        NLOG_ERR("Failed to init wifi dump context\n");
    }*/

    /* read from file or capture from wlan */
    if( g_config.s_file != NULL )
    {
        if( ! ( g_config.f_cap_in = fopen( g_config.s_file, "rb" ) ) )
        {
            NLOG_ERR( "open failed\n" );
            return -1;
        }

        n = sizeof( struct pcap_file_header );

        if( fread( &g_config.pfh_in, 1, n, g_config.f_cap_in ) != (size_t) n )
        {
            NLOG_ERR( "fread(pcap file header) failed\n" );
            return -1;
        }

        if( g_config.pfh_in.magic != TCPDUMP_MAGIC &&
                g_config.pfh_in.magic != TCPDUMP_CIGAM )
        {
            NLOG_ERR("\"%s\" isn't a pcap file (expected "
                     "TCPDUMP_MAGIC).\n", g_config.s_file );
            return -1;
        }

        if( g_config.pfh_in.magic == TCPDUMP_CIGAM )
            SWAP32(g_config.pfh_in.linktype);

        if( g_config.pfh_in.linktype != LINKTYPE_IEEE802_11 &&
                g_config.pfh_in.linktype != LINKTYPE_PRISM_HEADER &&
                g_config.pfh_in.linktype != LINKTYPE_RADIOTAP_HDR &&
                g_config.pfh_in.linktype != LINKTYPE_PPI_HDR )
        {
            NLOG_ERR("Wrong linktype from pcap file header "
                     "(expected LINKTYPE_IEEE802_11) -\n"
                     "this doesn't look like a regular 802.11 "
                     "capture.\n" );
            return -1;
        }

        while( 1 )
        {
            if( g_config.do_exit )
            {
                break;
            }

            /* Read one packet */
            n = sizeof( pkh );

            if( fread( &pkh, n, 1, g_config.f_cap_in ) != 1 )
            {
                NLOG_INFO("Finished reading input file %s.\n", g_config.s_file);
                g_config.s_file = NULL;
                return 0;
            }

            if( g_config.pfh_in.magic == TCPDUMP_CIGAM )
            {
                SWAP32( pkh.caplen );
                SWAP32( pkh.len );
            }

            n = caplen = pkh.caplen;

            memset(buffer, 0, sizeof(buffer));
            h80211 = buffer;

            if( n <= 0 || n > (int) sizeof( buffer ) )
            {
                NLOG_INFO("Finished reading input file %s.\n", g_config.s_file);
                g_config.s_file = NULL;
                return 0;
            }

            if( fread( h80211, n, 1, g_config.f_cap_in ) != 1 )
            {
                NLOG_INFO("Finished reading input file %s.\n", g_config.s_file);
                g_config.s_file = NULL;
                return 0;
            }

            if( g_config.pfh_in.linktype == LINKTYPE_PRISM_HEADER )
            {
                if( h80211[7] == 0x40 )
                    n = 64;
                else
                    n = *(int *)( h80211 + 4 );

                if( n < 8 || n >= (int) caplen )
                    continue;

                memcpy( tmpbuf, h80211, caplen );
                caplen -= n;
                memcpy( h80211, tmpbuf + n, caplen );
            }

            if( g_config.pfh_in.linktype == LINKTYPE_RADIOTAP_HDR )
            {
                /* remove the radiotap header */

                n = *(unsigned short *)( h80211 + 2 );

                if( n <= 0 || n >= (int) caplen )
                    continue;

                memcpy( tmpbuf, h80211, caplen );
                caplen -= n;
                memcpy( h80211, tmpbuf + n, caplen );
            }

            if( g_config.pfh_in.linktype == LINKTYPE_PPI_HDR )
            {
                /* remove the PPI header */

                n = le16_to_cpu(*(unsigned short *)( h80211 + 2));

                if( n <= 0 || n>= (int) caplen )
                    continue;

                /* for a while Kismet logged broken PPI headers */
                if ( n == 24 && le16_to_cpu(*(unsigned short *)(h80211 + 8)) == 2 )
                    n = 32;

                if( n <= 0 || n>= (int) caplen )
                    continue;

                memcpy( tmpbuf, h80211, caplen );
                caplen -= n;
                memcpy( h80211, tmpbuf + n, caplen );
            }

            /* scan 1 packet */
            handle_packet( h80211, caplen);

            read_pkts++;

            if(read_pkts%10 == 0)
                usleep(1);
        }

        return 0;

    }/*if( g_config.s_file != NULL )*/


    /* initialize card */
    ret = init_card(g_config.s_iface, &wi);

    if(ret < 0)
    {
        NLOG_ERR("Failed to init card\n");
        return -1;
    }

    NLOG_INFO("Success to init card %s\n", g_config.s_iface);

    fd_raw = wi_fd(wi);

    chan_count = getchancount(0);

    /* find the interface index */
    /* start a child to hop between channels */
    if( g_config.channel == 0 )
    {
        if(fork() == 0)
        {
            /* Drop privileges */
            if (setuid( getuid() ) == -1) {
                perror("setuid");
            }

            channel_hopper(wi, chan_count);
            exit( 1 );
        }

    }
    else
    {
        wi_set_channel(wi, g_config.channel);
        g_config.singlechan = 1;
        NLOG_INFO("sniff on single channel: %d\n", g_config.channel);
    }

    /* Drop privileges */
    if (setuid( getuid() ) == -1) {
        perror("setuid");
    }

    /* open or create the output files */
    if (g_config.record_data)
    {
        if(dump_initialize( g_config.dump_prefix) != 0)
        {
            return -1;
        }
    }

    tt_org        = time( NULL );
    gettimeofday( &tv_now, NULL );
    gettimeofday( &tv_org, NULL );

    while( 1 )
    {
        if( g_config.do_exit )
        {
            break;
        }

        if( time( NULL ) - tt_org > 5 )
        {
            tt_org = time( NULL );

            /* flush the output files */
            if( g_config.f_cap != NULL ) fflush( g_config.f_cap );
        }

        gettimeofday( &tv_now, NULL );

        /* cycle_time = 1000000UL * ( tv_now.tv_sec  - tv_org.tv_sec  )
                     + ( tv_now.tv_usec - tv_org.tv_usec );

        if( cycle_time > 500000 )
        {
            gettimeofday( &tv_org, NULL );
            check_monitor(wi, &fd_raw);

            if(g_config.singlechan)
                check_channel(wi);

            if(g_config.singlefreq)
                check_frequency(wi);
        } */

        /* capture one packet */
        FD_ZERO( &rfds );
        FD_SET( fd_raw, &rfds );

        timeout.tv_sec  = 0;
        timeout.tv_usec = REFRESH_RATE;

        if( select( fd_raw + 1, &rfds, NULL, NULL, &timeout ) < 0 )
        {
            if( errno == EINTR )
            {
                continue;
            }

            perror( "select failed" );

            return -1;
        }

        fd_is_set = 0;

        if( FD_ISSET( fd_raw, &rfds ) )
        {
            memset(buffer, 0, sizeof(buffer));
            h80211 = buffer;

            if ((caplen = wi_read(wi, h80211, sizeof(buffer), &ri)) == -1)
            {
                wi_read_failed++;

                if(wi_read_failed > 1)
                {
                    NLOG_ERR("wi_read_failed > 1, system exit.\n");
                    g_config.do_exit = 1;
                    continue;
                }

                NLOG_ERR("interface %s down\n", wi_get_ifname(wi));

                //reopen in monitor mode
                strncpy(ifnam, wi_get_ifname(wi), sizeof(ifnam)-1);
                ifnam[sizeof(ifnam)-1] = 0;

                wi_close(wi);
                wi = wi_open(ifnam);

                if (!wi)
                {
                    NLOG_ERR("Can't reopen %s\n", ifnam);
                    exit(1);
                }

                fd_raw = wi_fd(wi);
            }
            else
            {
                wi_read_failed = 0;
                handle_packet( h80211, caplen);
            }
        }

    }//while( 1 )

    if(g_config.own_channels)
        free(g_config.own_channels);

    if(g_config.prefix)
        free(g_config.prefix);

    if(g_config.f_cap_name)
        free(g_config.f_cap_name);

    wi_close(wi);

    if (g_config.record_data)
    {
        if ( g_config.output_format_pcap ||  g_config.f_cap != NULL ) fclose( g_config.f_cap );
    }

    //dump_fini(&g_wifi_dump_ctx);

    return( 0 );
}
