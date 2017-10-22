#if !defined(IBUS_H)
#define IBUS_H

#include <stdint.h>

/*
  The data is serial data, 115200, 8N1.
  Messages arrive every 7 milliseconds, and are read constantly until a
  few tenths of a second after the transmitter is switched off.

  Packet format:
  20 40 CH0 CH1 CH2 CH3 CH4 CH5 CH6 CH7 CH8 CH9 CH10 CH11 CH12 CH13 SUM
  Channels are stored in little endian byte order.  Unused channels read
  5DC (first byte DC, second byte 05).

  Channel 0: Right horizontal: 3E8 -> 7D0
  Channel 1: Right vertical:   3E8 -> 7CF
  Channel 2: Left vertical:    3E8 -> 7D0
  Channel 3: Left horizontal:  3E8 -> 7CD
  Channel 4: Left pot:         3E8 -> 7D0
  Channel 5: Right pot:        3E8 -> 7D0

  The checksum starts at 0xFFFF, then subtract each byte except the 
  checksum bytes.
*/

struct ibus_state {
  uint_fast8_t state;
  uint_fast16_t checksum;
  uint_fast8_t datal;
  uint_fast8_t channel_count;
};

/**
 * Initialize the library state.
 * @param state A state structure; the same structure must be passed to
 *   successive calls to ibus_read
 * @param channel_count How many channels to read (maximum 14)
 */
void ibus_init(struct ibus_state* state, uint_fast8_t channel_count);

/**
 * Process the next byte from the receiver.
 * @param state The library state, initialized by ibus_init and passed to
 *   all calls to this function
 * @param data Where to write the channel data.  Must contain as many elements
 *   as passed to ibus_init.
 * @param ch The next character from the receiver
 * @return 0 if the end of the packet has been received, and the checksum
 *   is OK.
 */
int ibus_read(struct ibus_state* state, uint16_t* data, uint8_t ch);

#endif

