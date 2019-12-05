#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/vmalloc.h>
#include <linux/if_packet.h>

struct net_device *ndev;
int data = 10;

static rx_handler_result_t rg_rx_handler(struct sk_buff **pskb);

static rx_handler_result_t rg_rx_handler(struct sk_buff **pskb)
{
	struct sk_buff *skb = *pskb;
	
	if(skb->pkt_type == PACKET_HOST){
		pr_info("Got a packet");
	}

	return RX_HANDLER_PASS;
}

static int __init ndev_module_init(void)
{
	int ret;

	const char *ifname = "lo";
	ndev = dev_get_by_name(&init_net,ifname);
	if(!ndev){
		return -1;
	}	

	pr_info("netdev_rx_handler module is loaded");
	
	ret = netdev_rx_handler_register(ndev,rg_rx_handler,&data);
	if(ret < 0){
        	printk(KERN_INFO "failed to register netdev_rx_handler.\n");
        }

	return 0;
}

static void __exit ndev_module_exit(void)
{
	pr_info("netdev_rx_handler is unloaded");
	netdev_rx_handler_unregister(ndev);
	dev_put(ndev);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ryusei Shiiba");
MODULE_DESCRIPTION("a netdev_rx_handler example");

module_init(ndev_module_init);
module_exit(ndev_module_exit);

