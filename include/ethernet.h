#ifndef ETHERNET_H_
#define ETHERNET_H_

#include "netdev.h"
#include "skbuff.h"
#include "syshead.h"
#include "utils.h"
#include <linux/if_ether.h>

#define ETH_HDR_LEN sizeof(struct eth_hdr)

#ifdef DEBUG_ETH
#define eth_dbg(msg, hdr)                                                                                              \
    do {                                                                                                               \
        print_debug("eth " msg " ("                                                                                    \
                    "dmac: %.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx, "                                                \
                    "smac: %.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx, "                                                \
                    "ethertype: %.4hx)",                                                                               \
                    hdr->dmac[0], hdr->dmac[1], hdr->dmac[2], hdr->dmac[3], hdr->dmac[4], hdr->dmac[5], hdr->smac[0],  \
                    hdr->smac[1], hdr->smac[2], hdr->smac[3], hdr->smac[4], hdr->smac[5], hdr->ethertype);             \
    } while (0)
#else
#define eth_dbg(msg, hdr)
#endif

struct sk_buff;
struct netdev;

uint8_t *skb_head(struct sk_buff *skb);

/**
 * 以太网帧定义
 *
 */
struct eth_hdr {
    uint8_t dmac[6];    // 目标 mac
    uint8_t smac[6];    // 源 mac
    uint16_t ethertype; // 在 linux/if_ether.h 中定义, 如 ETH_P_ARP, ETH_P_IP 等
    uint8_t payload[];
} __attribute__((packed)); // 取消对齐, 紧凑排列

static inline struct eth_hdr *eth_hdr(struct sk_buff *skb) {
    struct eth_hdr *hdr = (struct eth_hdr *)skb_head(skb);

    hdr->ethertype = ntohs(hdr->ethertype); // 网络字节顺序转换为主机字节顺序

    return hdr;
}

#endif
