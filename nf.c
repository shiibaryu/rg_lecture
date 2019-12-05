#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/netfilter.h>
#include <linux/vmalloc.h>
#include <linux/netfilter_ipv4.h>

static struct nf_hook_ops nf_host;

static unsigned int nf_udp_hook(void *priv, struct sk_buff *skb,
                              const struct nf_hook_state *state);

unsigned int nf_udp_hook(void *priv,struct sk_buff *skb,const struct nf_hook_state *state)  {
	if(skb->pkt_type == PACKET_HOST){
    		pr_info("Hello packet!");
	}
    	return NF_ACCEPT;
}

static int __init nf_module_init(void)
{
	pr_info("nf module is loaded");

	nf_host.hook = nf_udp_hook;
	nf_host.hooknum = 0;
	nf_host.pf = PF_INET;
	nf_host.priority = NF_IP_PRI_FIRST;

	nf_register_net_hook(&init_net,&nf_host);

	return 0;
}

static void __exit nf_module_exit(void)
{
	pr_info("nf module is unloaded");
	nf_unregister_net_hook(&init_net,&nf_host);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ryusei Shiiba");
MODULE_DESCRIPTION("a netfilter example");

module_init(nf_module_init);
module_exit(nf_module_exit);

