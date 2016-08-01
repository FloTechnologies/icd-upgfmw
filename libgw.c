#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mount.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "libgw.h"

int fd;
int data_coming = FALSE ; 

void flush_uart_data(void)
{
  char buffer[MAX_BUFFER_SIZE];

  bzero(buffer, MAX_BUFFER_SIZE);
  sprintf(buffer, "\r\n\r\n");
  write(fd, buffer, strlen(buffer));
}

void updfrm_command(char* cmd)
{
  char buffer[MAX_BUFFER_SIZE];

  bzero(buffer, MAX_BUFFER_SIZE);
  sprintf(buffer, "%s\r\n",cmd);
  write(fd, buffer, strlen(buffer));
}

void choose_upload_ebl(void)
{
  write(fd, "1", 1);
}

void choose_run_ebl(void)
{
  char buffer[MAX_BUFFER_SIZE] ;
  char cmd[] = "2\r\n";

  bzero(buffer, MAX_BUFFER_SIZE);
  memcpy(buffer, cmd, sizeof(cmd));
  write(fd, buffer, strlen(buffer));
}

void dbg(char* filename, int linenum, char* fmt, ...)
{
#ifdef RSX_DEBUG
  va_list args;

  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  return;
 #endif
}

void uart_handler(int status)
{
//can not do the work as receive_uart_data()
//because SIGIO always occuring
//and ohter house keeping work will not execute
  data_coming = TRUE;
}


void receive_uart_data(void)
{
    int len;
    char ch;
    time_t start;
    time_t end;

    if(data_coming == FALSE)
    {
      return;
    }

    start = time(NULL);
    while(data_coming)
    {
        len = read(fd, &ch , 1);
	
        if(len>0)
        {
	      printf("%c",ch);
        }
      end = time(NULL);
      if((end - start) > 2)
      {
    	data_coming = FALSE;
        return;
      }
    }
    data_coming = FALSE;
    return;
}

void init_serial_FW(char* device)
{
struct termios oldtio,newtio;
  struct sigaction saio;

  //fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
  fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
  if(fd < 0)
  {
    printf("OPEN RD FAILED, EXIT PROGRAM");
    exit(1);
  }else{
    printf("OPEN RD  : %s OK!!\r\n", device);
  }

  tcgetattr(fd,&oldtio); 
  bzero(&newtio, sizeof(newtio)); 

  saio.sa_handler = uart_handler;
  saio.sa_flags = 0;
  saio.sa_restorer = NULL;
  sigaction(SIGIO,&saio,NULL);

  //fcntl(fd, F_SETOWN, getpid());
  fcntl(fd, F_SETFL, FNDELAY);
  fcntl(fd, F_SETFL, FASYNC);
  newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD | CRTSCTS;
  newtio.c_iflag = IGNPAR; //ignore parity error
  newtio.c_oflag = 0;
  newtio.c_lflag = 0;//ICANON;
  newtio.c_cc[VMIN]=0;
  newtio.c_cc[VTIME]=0;
  tcflush(fd, TCIOFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);
}


void init_serial_noFW(char* device)
{
struct termios oldtio,newtio;
  struct sigaction saio;

  //fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
  fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
  if(fd < 0)
  {
    printf("OPEN RD FAILED, EXIT PROGRAM");
    exit(1);
  }else{
    printf("OPEN RD  : %s OK!!\r\n", device);
  }

  tcgetattr(fd,&oldtio); 
  bzero(&newtio, sizeof(newtio)); 

  saio.sa_handler = uart_handler;
  saio.sa_flags = 0;
  saio.sa_restorer = NULL;
  sigaction(SIGIO,&saio,NULL);

  //fcntl(fd, F_SETOWN, getpid());
  fcntl(fd, F_SETFL, FNDELAY);
  fcntl(fd, F_SETFL, FASYNC);
  newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR; //ignore parity error
  newtio.c_oflag = 0;
  newtio.c_lflag = 0;//ICANON;
  newtio.c_cc[VMIN]=0;
  newtio.c_cc[VTIME]=0;
  tcflush(fd, TCIOFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);
}

void close_uart_port(void)
{
    close(fd);
}
