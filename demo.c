#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>

#include "ibus.h"

/*
 * Demonstrates the ibus library.  Pass the serial stream from the receiver
 * to stdin, for example on Linux:
 *
 *     $ stty raw 115200 < /dev/ttyUSB0 && ./demo < /dev/ttyUSB0
 *
 */

int main(void) {
  uint8_t ch;
  int i, status;
  struct timespec time;
  struct ibus_state ibus_data;
  uint16_t data[6];

  ibus_init(&ibus_data, 6);

  while(1) {
    status = read(0, &ch, 1);
    if (status == 0)
      continue;
    else if (status == -1) {
      printf("Read error %d\n", errno);
      perror(NULL);
      exit(1);
    }

    if (ibus_read(&ibus_data, data, ch) == 0) {
      clock_gettime(CLOCK_MONOTONIC, &time);
      printf("%03ld ", time.tv_nsec / 1000000);

      for (i = 0; i < 6; i++)
        printf("%04X ", data[i]);

      printf("\n");
    }
  }
}

