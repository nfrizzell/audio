#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/usb.h>

#include "telegraph.h"

static struct usb_driver telegraph_driver;

const static struct usb_device_id telegraph_id_table[] = {
	/* ST-LINK/V2.1 */
	{ USB_DEVICE(USB_TELEGRAPH_VENDOR_ID, USB_TELEGRAPH_PRODUCT_ID) },
	{}
};

MODULE_DEVICE_TABLE(usb, telegraph_id_table);

static void telegraph_delete(struct kref *kref)
{
	struct telegraph_device *dev = to_telegraph_device(kref);

	usb_put_dev(dev->usb_dev);
	kfree(dev);
}

static int telegraph_open(struct inode *inode, struct file *file)
{
	struct telegraph_device *dev;
	struct usb_interface *interface;
	int subminor;

	subminor = iminor(inode);
	interface = usb_find_interface(&telegraph_driver, subminor);
	if (!interface) {
		printk(KERN_ERR "Can't find device for minor number %d");
		return -ENODEV;
	}

	dev = usb_get_intfdata(interface);
	if (!dev) 
		return -ENODEV;

	kref_get(&(dev->kref));
	file->private_data = dev;
	
	return 0;
}

static int telegraph_release(struct inode *inode, struct file *file)
{
	struct telegraph_device *dev;

	dev = (struct telegraph_device *)file->private_data;
	if (dev == NULL)
		return -ENODEV;
	
	kref_put(&dev->kref, telegraph_delete);
	return 0;
}

static ssize_t telegraph_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos)
{
	struct telegraph_device *dev;
	int retval = 0;

	dev = (struct telegraph_device *)file->private_data;

	// TODO

	return retval;
}

static ssize_t telegraph_write(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos)
{
	struct telegraph_device *dev;
	int retval = 0;
	struct urb *urb = NULL;
	char *buf = NULL;

	dev = (struct telegraph_device *)file->private_data;
	/* Empty input */
	if (count == 0)
		goto exit;

	urb = usb_alloc_urb(0, GFP_KERNEL);
	if (!urb) {
		retval = -ENOMEM;
		goto error;
	}

	buf = usb_alloc_coherent(dev->usb_dev, count, GFP_KERNEL, &(urb->transfer_dma));
	if (!buf) {
		retval = -ENOMEM;
		goto error;
	}

	if (copy_from_user(buf, user_buffer, count)) {
		retval = -EFAULT;
		goto error;
	}

	//TODO

exit:
	return count;

error:
	kfree(buf);
	return retval;

}

static struct file_operations telegraph_fops = {
	.owner = THIS_MODULE,
	.open = &telegraph_open,
	.release = &telegraph_release,
	.read = &telegraph_read,
	.write = &telegraph_write
};

static struct usb_class_driver telegraph_class_driver = {
	.name = "usb/telegraph%d",
	.fops = &telegraph_fops,
	.minor_base = USB_TELEGRAPH_MINOR_BASE
};

static int telegraph_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	struct telegraph_device *dev = NULL;
	struct usb_host_interface *interface_desc;
	struct usb_endpoint_descriptor *endpoint;
	int i;
	int retval = -ENOMEM;

	dev = kzalloc(sizeof(struct telegraph_device), GFP_KERNEL);
	if (dev == NULL) {
		printk(KERN_ERR "Failed to allocate memory for device");
		goto error;
	}
	kref_init(&dev->kref);

	dev->usb_dev = usb_get_dev(interface_to_usbdev(interface));
	dev->usb_intf = interface;

	for (i = 0; i < interface_desc->desc.bNumEndpoints; i++) {
		endpoint = &(interface_desc->endpoint[i].desc);

		/* Bulk out endpoint */
		if (!dev->bulk_out_endpoint_addr &&
		    !(endpoint->bEndpointAddress & USB_DIR_IN) &&
		    ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
		    USB_ENDPOINT_XFER_BULK)) {
			dev->bulk_out_endpoint_addr = endpoint->bEndpointAddress;
		}

		/* Control out endpoint */
		else if (!dev->ctrl_out_endpoint_addr &&
		    !(endpoint->bEndpointAddress & USB_DIR_IN) &&
		    ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
		    USB_ENDPOINT_XFER_CONTROL)) {
			dev->ctrl_out_endpoint_addr = endpoint->bEndpointAddress;
		}

		/* Control in endpoint */
		else if (!dev->ctrl_in_endpoint_addr &&
		    (endpoint->bEndpointAddress & USB_DIR_IN) &&
		    ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
		    USB_ENDPOINT_XFER_CONTROL)) {

		}
	}

	if (!(dev->bulk_out_endpoint_addr &&
	    !(dev->ctrl_in_endpoint_addr) &&
	    !(dev->ctrl_out_endpoint_addr))) {
		goto error;
	}

	/* Store the telegraph_device in the interface struct */
	usb_set_intfdata(interface, dev);

	retval = usb_register_dev(interface, &telegraph_class_driver);
	if (retval) {
		printk(KERN_ERR, "Minor number not available");
		goto error;
	}

	return 0;

error:
	/* Deallocate if no longer in use */
	if (dev)
		kref_put(&(dev->kref), telegraph_delete);
	return retval;
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
