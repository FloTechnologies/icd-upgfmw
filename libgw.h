#ifndef __ZIGBEE_GATEWAY_H__
#define __ZIGBEE_GATEWAY_H__

#define RSX_DEBUG
#define ZCDEVICE "/dev/usb/tts/0"

#define MAX_ATTRIBUTE_TABLE_SIZE 7
#define MAX_WAITING_TIME 60

#define BAUDRATE B115200
#define FALSE 0
#define TRUE 1

#define CR 0x0d
#define LF 0x0a

#define CURRENT_TIME time(NULL)

#define MAX_BUFFER_SIZE 256
#define MAX_ARRAY_SIZE 64
#define MAX_CHILD_TABLE_SIZE 30
#define MAX_RAWDATA_TABLE_SIZE MAX_CHILD_TABLE_SIZE //one node, one power-meter, one rawdata

#define CLUSTER_ID 0x0702 // the power meter cluster id

#ifdef RSX_DEBUG
#define PRINTH(p, size, str) \
  do{\
    char tmp[32];\
    memset(tmp, 0x00, 32);\
    memcpy(tmp, (p), (size));\
    printf("[%s:%d]"str"%s\n",__FILE__, __LINE__, tmp);\
  }while(0)
#else
#define PRINTH(p, size, str)
#endif

#include <time.h>
// 1/1000 seconds sleep
#define msleep(x) usleep((x)*1000)


typedef struct TimerTask_s TimerTask;
typedef struct TimerTaskData_s TimerTaskData;
typedef struct HostProtocol_s HostProtocol;
typedef struct Rawdata_s Rawdata;
typedef unsigned char int8u;

static const char HEX_TABLE[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
static const int8u empty_child[16] = "0000000000000000";

struct Rawdata_s
{
  int8u macid[17];
  time_t time;
  char v[5];
  char i[5];
  char freq[5];
  char pf[3];
  char w[7];
  char va[7];
  char wh[9];
  char rssi[3];
};

struct TimerTask_s
{
  time_t start_time;
  int delay_seconds;
};


struct TimerTaskData_s
{
  TimerTask* task;
  void (*handler)(void);
};


struct HostProtocol_s{
	//version number
	char	ver[3];

	//transaction sequence number
	char seq[3];

	//IEEE address
	char longAddress[17];

	//receiver sensitive
	char rssi[3];
    
    //zcl profile id
    char profileId[5];

	//zcl cluster id
	char clusterId[5];
    
	//source endpoint
	char srcEndpoint[3];

	//destination endpoint
	char destEndpoint[3];

	//zcl command id
	char cmdId[3];

    //profile wide or not
    char is_profile_wide[3];

	//zcl attribute id
	char attributeId[5];

	//response status reference ZCL status code
	char status[3];
    
    char v[9];
    char i[9];
    char freq[9];
    char pf[9];
    char w[9];
    char va[9];
    char wh[9];
    
};



void dbg(char* filename, int linenum, char* fmt, ...);

void disable_timer_task(TimerTask* task);
void enable_timer_task(TimerTask* task, int delay_seconds);
void run_timer_tasks(TimerTaskData tasks[]);

void init_serial(char*  device);
void close_uart_port(void);
void uart_handler(int status);
void receive_uart_data(void);
void reset_child_table();
void reset_error_child_table();
void add_child_table(char* eui64);
void dispatcher(HostProtocol *p);

void list_children_command();
void allow_join_command();
void mtorr_command();
void print_child_table_command();
void parse_response_command(char* payload, HostProtocol* p);

void ask_power_meter_command(int8u index, char* eui);

void add_rawdata(Rawdata *r);
void transfer_data(int8u check_error);
void serialize_to_tmp(int8u check_error);
void calculate_crc(Rawdata *r);

void save_eui64list(void);

#endif
