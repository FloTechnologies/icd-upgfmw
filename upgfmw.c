#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mount.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "libgw.h"

/**
 * The EM357 needed more serious flow control handling
 * the parameter
 * param1 : command to goto the bootloader mode of the RX3573
 * param2 : the tty port that connect to RX2503 which is /dev/ttyS0 of RSG-3000
 * param3 : the ebl file
 * */

int main(int argc, char** argv)
{

  char cmd[1024];
  int status;

  printf("Rosonix Updfrm v3.2 %s %s\r\n", __DATE__, __TIME__);

  if(argc < 3 ){
	  printf("updfrm cmd device ebl\n\n");
	  return -1;
  }

  init_serial_FW(argv[2]);
  receive_uart_data();
  updfrm_command(argv[1]);
  flush_uart_data();
  usleep(200000);
  receive_uart_data();
  usleep(200000);
  receive_uart_data();
  close_uart_port();

  init_serial_noFW(argv[2]);
  flush_uart_data();
  usleep(200000);
  receive_uart_data();
  choose_upload_ebl(); //DO NOT SEND ANY chars after choose upload option, include \r\n
  close_uart_port();

  bzero(cmd, sizeof(cmd));
  sprintf(cmd, "picocom -fn -b115200 -n %s %s", argv[3], argv[2]); //The picocom has to be modified to support -n option
  status = system(cmd);

  init_serial_noFW(argv[2]);
  choose_run_ebl();
  flush_uart_data();
  usleep(500000UL);
  receive_uart_data();
  close_uart_port();

  return status;
  
}
