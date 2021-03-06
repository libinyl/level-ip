#ifndef IPV4_H
#define IPV4_H
#include "ethernet.h"
#include "skbuff.h"
#include "sock.h"
#include "syshead.h"

#define IPV4 0x04
#define IP_TCP 0x06
#define ICMPV4 0x01

#define IP_HDR_LEN sizeof(struct iphdr)
#define ip_len(ip) (ip->len - (ip->ihl * 4))

#ifdef DEBUG_IP
#define ip_dbg(msg, hdr)                                                                                               \
    do {                                                                                                               \
        print_debug("ip " msg " (ihl: %hhu version: %hhu tos: %hhu "                                                   \
                    "len %hu id: %hu frag_offset: %hu ttl: %hhu "                                                      \
                    "proto: %hhu csum: %hx "                                                                           \
                    "saddr: %hhu.%hhu.%hhu.%hhu daddr: %hhu.%hhu.%hhu.%hhu)",                                          \
                    hdr->ihl, hdr->version, hdr->tos, hdr->len, hdr->id, hdr->frag_offset, hdr->ttl, hdr->proto,       \
                    hdr->csum, hdr->saddr >> 24, hdr->saddr >> 16, hdr->saddr >> 8, hdr->saddr >> 0, hdr->daddr >> 24, \
                    hdr->daddr >> 16, hdr->daddr >> 8, hdr->daddr >> 0);                                               \
    } while (0)
#else
#define ip_dbg(msg, hdr)
#endif
/**
 * https://en.wikipedia.org/wiki/IPv4
 */
struct iphdr {
    uint8_t ihl : 4; /* TODO: Support Big Endian hosts */ // ihl = Internet Header Length,  IPv4 header 长度 , >=5 ( 20
                                                          // 字节 ) && <= 15 (60 字节)
    uint8_t version : 4; // 版本
    uint8_t tos;         // type of service
    uint16_t len; // Total Length : header 和 data 的总长度. 最小为 20 字节, 即 header 长度, 最大为 65,535 字节.
    uint16_t id;          // 唯一 id
    uint16_t frag_offset; // Fragment Offset,
    uint8_t ttl;          // Time To Live
    uint8_t proto;        // 协议编号 https://en.wikipedia.org/wiki/List_of_IP_protocol_numbers
    uint16_t csum;        // Header Checksum https://en.wikipedia.org/wiki/IPv4_header_checksum
    uint32_t saddr;       // 发总者 ip 地址
    uint32_t daddr;       // 发送目标 ip 地址
    uint8_t data[];
} __attribute__((packed));

static inline struct iphdr *ip_hdr(const struct sk_buff *skb) { return (struct iphdr *)(skb->head + ETH_HDR_LEN); }

static inline uint32_t ip_parse(char *addr) {
    uint32_t dst = 0;

    if (inet_pton(AF_INET, addr, &dst) != 1) {
        perror("ERR: Parsing inet address failed");
        exit(1);
    }

    return ntohl(dst);
}

int ip_rcv(struct sk_buff *skb);
int ip_output(struct sock *sk, struct sk_buff *skb);

#endif
