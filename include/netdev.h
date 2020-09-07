#ifndef NETDEV_H
#define NETDEV_H
#include "syshead.h"
#include "ethernet.h"
#include "skbuff.h"
#include "utils.h"

#define BUFLEN 1600
#define MAX_ADDR_LEN 32

#define netdev_dbg(fmt, args...)                \
    do {                                        \
        print_debug("NETDEV: "fmt, ##args);     \
    } while (0)

struct eth_hdr;

/**
 * https://www.kernel.org/doc/html/latest/networking/netdev-FAQ.html
 *
 */
struct netdev {
    uint32_t addr;
    uint8_t addr_len;
    uint8_t hwaddr[6];
    uint32_t mtu; // Maximum Transmission Unit 最大传输单元 用来通知对方所能接受数据服务单元的最大尺寸，说明发送方能够接受的有效载荷大小
};

void netdev_init();
int netdev_transmit(struct sk_buff *skb, uint8_t *dst, uint16_t ethertype);
void *netdev_rx_loop();
void free_netdev();
struct netdev *netdev_get(uint32_t sip);
#endif
