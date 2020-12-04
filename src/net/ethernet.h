/**
 * @file
 * @see https://en.wikipedia.org/wiki/Ethernet_frame
 * @see https://wiki.osdev.org/Network_Stack
 */
#ifndef NET_ETHERNET_H
#define NET_ETHERNET_H

#include <net/net.h>
#include <stdint.h>

// TODO: ideally, we'd check the endianness of the system but this project is
// for x86_64 only (i.e. it's little endian and we have to use those macros).
#define HTONS(n) ((((n)&0xFF) << 8) | (((n)&0xFF00) >> 8))
#define NTOHS(n) HTONS(n)
#define HTONL(n)                                                               \
  ((((n)&0x000000FF) << 24) | (((n)&0x0000FF00) << 8) |                        \
   (((n)&0x00FF0000) >> 8) | (((n)&0xFF000000) >> 24))
#define NTOHL(n) HTONL(n)

typedef struct ethernet_header
{
  uint8_t dst_mac[6];
  uint8_t src_mac[6];
  uint16_t ethertype;
} __attribute__((packed)) ethernet_header_t;

/**
 * Receives an Ethernet frame on an interface. This is a handler that gets
 * called by the lower layer (i.e. the network driver). After having decoded
 * the frame, this handler will likely call a handler of this layer (2) or the
 * layer above (3).
 *
 * @param interface the network interface that received the frame
 * @param data the data containing the Ethernet frame
 * @param len the length of the data
 */
void ethernet_receive_frame(net_interface_t* interface,
                            uint8_t* data,
                            uint32_t len);

/**
 * Sends an Ethernet frame over an interface.
 *
 * We are at the Ethernet level, so we can only communicate with machines in
 * the same LAN and we use MAC addresses for that. The destination MAC address
 * is therefore used by one of the machines in the LAN, usually the DNS server
 * or the "gateway" (but it could be any other machine for LAN exchanges). We
 * will use the MAC address of the gateway when we will send Internet packets
 * for instance.
 *
 * @param interface the network interface used to send the frame
 * @param dst_mac the destination MAC address
 * @param ethertype a value indicating which protocol is encapsulated in the
 * payload of the frame
 * @param data the encapsulated payload
 * @param len the length of the data
 */
void ethernet_transmit_frame(net_interface_t* interface,
                             uint8_t dst_mac[6],
                             uint16_t ethertype,
                             uint8_t* data,
                             uint32_t len);

#endif
