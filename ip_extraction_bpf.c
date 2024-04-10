#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <bpf/bpf_helpers.h>


#define bpf_debug(fmt, ...)						\
		({							\
			char ____fmt[] = fmt;				\
			bpf_trace_printk(____fmt, sizeof(____fmt),	\
				     ##__VA_ARGS__);			\
		})

struct bpf_map_def SEC("maps") blacklist = {
	.type        = BPF_MAP_TYPE_PERCPU_HASH,
	.key_size    = sizeof(__u32),
	.value_size  = sizeof(__u64),
	.max_entries = 100000,
	.map_flags   = BPF_F_NO_PREALLOC,
};

static __always_inline
__u32 parse_ipv4(struct xdp_md *ctx, __u64 l3_offset)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data     = (void *)(long)ctx->data;
	struct iphdr *iph = data + l3_offset;
	__u64 *value;
	__u32 ip_src;

	if ((struct iphdr *)iph + 1 > (struct iphdr *)data_end) {
		bpf_debug("Invalid IPv4 packet: L3off:%llu\n", l3_offset);
		return XDP_ABORTED;
	}
	ip_src = iph->saddr;

	bpf_debug("Valid IPv4 packet: raw saddr:0x%x\n", ip_src);

	value = bpf_map_lookup_elem(&blacklist, &ip_src);
	if (value) {
		*value += 1;
		return XDP_DROP;
	}
    return XDP_PASS;
}


SEC("socket")
int filter_packet(struct xdp_md *ctx)
{
	void *data_end = (void *)(long)ctx->data_end;
	void *data     = (void *)(long)ctx->data;
	struct ethhdr *eth = data;
	__u64 l3_offset = 0;
	__u32 action;

	action = parse_ipv4(ctx, l3_offset);
	return action;
}

char _license[] SEC("license") = "GPL";
