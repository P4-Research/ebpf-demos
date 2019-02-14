#include <linux/bpf.h>
#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <linux/ip.h>

#include <linux/ipv6.h>

#define htons(x) ((__be16)___constant_swab16((x)))
#define SEC(NAME) __attribute__((section(NAME), used))

#define ARPOP_REQUEST 1
#define ARPOP_REPLY 2

struct arphdr
{
	__be16		ar_hrd;		/* format of hardware address	*/
	__be16		ar_pro;		/* format of protocol address	*/
	unsigned char	ar_hln;		/* length of hardware address	*/
	unsigned char	ar_pln;		/* length of protocol address	*/
	__be16		ar_op;		/* ARP opcode (command)		*/
	unsigned char	ar_sha[6];	/* sender hardware address	*/
	unsigned char	ar_sip[4];		/* sender IP address		*/
	unsigned char	ar_tha[6];	/* target hardware address	*/
	unsigned char	ar_tip[4];		/* target IP address		*/
};

static inline void determine_mac_addr(unsigned char *src_mac, unsigned char *ar_tip)
{
    unsigned char h1_mac[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 };
    unsigned char h2_mac[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };

    unsigned char h1_ip[4] = { 0x0a, 0x00, 0x00, 0x01};
    unsigned char h2_ip[4] = { 0x0a, 0x00, 0x00, 0x02};

    if (__builtin_memcmp(h2_ip, ar_tip, 4) == 0)
        __builtin_memcpy(src_mac, h2_mac, 6);

    if (__builtin_memcmp(h1_ip, ar_tip, 4) == 0)
	__builtin_memcpy(src_mac, h1_mac, 6);

}

static inline void construct_arp_reply(void *data, void *src_mac, void *dst_mac, void *sender_ip, void *target_ip)
{
    struct ethhdr *eth = data;

    __builtin_memcpy(eth->h_dest, dst_mac, 6);
    __builtin_memcpy(eth->h_source, src_mac, 6);

    struct arphdr *arp = data + sizeof(struct ethhdr);
    __builtin_memcpy(arp->ar_sha, src_mac, 6);
    __builtin_memcpy(arp->ar_tha, dst_mac, 6);
    __builtin_memcpy(arp->ar_sip, sender_ip, 4);
    __builtin_memcpy(arp->ar_tip, target_ip, 4);
    arp->ar_op = htons(2);

}

SEC("xdp_arp_resp")
int xdp_arp_resp_prog(struct xdp_md *ctx) {

    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;

    struct ethhdr *eth = data;

    __u16 h_proto;
    __u64 nh_off;
    unsigned char h_source[6];
    unsigned char h_dest[6];

    nh_off = sizeof(struct ethhdr);
    if (data + nh_off > data_end)
        return XDP_DROP;

    h_proto = eth->h_proto;
    __builtin_memcpy(h_dest, eth->h_dest, 6);
    __builtin_memcpy(h_source, eth->h_source, 6);

    if (h_proto == htons(ETH_P_ARP)) {

        struct arphdr *arph = data + nh_off;
        nh_off += sizeof(struct arphdr);
        if (data + nh_off > data_end)
             return XDP_DROP;
        __u16 arp_opcode = arph->ar_op;
        if (arp_opcode == htons(ARPOP_REQUEST)) {

             unsigned char src_mac[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	     determine_mac_addr(src_mac, arph->ar_tip);

             construct_arp_reply(data, &src_mac, &h_source, &arph->ar_tip, &arph->ar_sip);

             return XDP_TX;
        }
    }

    return XDP_PASS;
}


char _license[] SEC("license") = "GPL";
