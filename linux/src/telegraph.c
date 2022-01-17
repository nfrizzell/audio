#include <linux/module.h>
#include <linux/errno.h>
#include <linux/usb.h>
#include <linux/hid-ids.h>

const static struct usb_device_id telegraph_id_table[] = {
	/* ST-LINK/V2.1 */
	{ USB_DEVICE(USB_VENDOR_ID_STM_0, 0x374b) }
};

static int telegraph_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	
	return 0;
}

static void telegraph_disconnect(struct usb_interface *intf)
{
	
}

static struct usb_driver telegraph_driver = {
	.name = "telegraph",
	.probe = telegraph_probe,
	.disconnect = telegraph_disconnect,
	.id_table = telegraph_id_table
};

static int __init telegraph_init(void)
{
	int result;

	result = usb_register(&telegraph_driver);
	if (result) {
	
	}

	return result;
}

static void __exit telegraph_exit(void)
{
	usb_deregister(&telegraph_driver);
}

module_init(telegraph_init);
module_exit(telegraph_exit);

MODULE_AUTHOR("nfrizzell");
MODULE_LICENSE("GPL");
