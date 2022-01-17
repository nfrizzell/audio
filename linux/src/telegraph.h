#ifndef __TELEGRAPH_H
#define __TELEGRAPH_H

#include <linux/usb.h>
#include <linux/kref.h>

#define USB_TELEGRAPH_VENDOR_ID 0x0483
#define USB_TELEGRAPH_PRODUCT_ID 0x374b

struct telegraph_device {
	struct usb_device * usb_dev;
	struct usb_interface * usb_intf;

	u8 bulk_out_endpoint_addr;
	u8 ctrl_in_endpoint_addr;
	u8 ctrl_out_endpoint_addr;

	struct kref kref;
};
#define to_telegraph_device(d) container_of(d, struct telegraph_device, kref)

#endif // __TELEGRAPH_H
