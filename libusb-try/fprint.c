/*
	Attempt to check all the endpoints of the fp-scanner
	Author: Srikar
*/

#include<stdio.h>
#include<stdlib.h>
#include<libusb-1.0/libusb.h>

// VID and PID of the FP driver
#define DEV_VID 0x138a
#define DEV_PID 0x00ab

int main(int argc, char **argv)
{

	// Function prototypes

	void print_device_descriptor(struct libusb_device_descriptor);
	void print_device_configuration(struct libusb_config_descriptor*);
	void check_safe(int);

	// All the pointers to handle the device
	libusb_device_handle *dev_handle = NULL;
	libusb_device *dev = NULL;
	libusb_context *context = NULL;

	int ret = libusb_init(&context);

	if(ret < 0)
	{
		perror("libusb_init");
		exit(1);
	}

	dev_handle = libusb_open_device_with_vid_pid(context, DEV_VID, DEV_PID);

	if(dev_handle == NULL)
	{
		printf("Fingerprint device not found :(\n");
		
		libusb_close(dev_handle);
		libusb_exit(context);
		return -1;
	}

	// dev = libusb_get_device(dev_handle);

	// Claiming the device if used by some other driver
	if(libusb_kernel_driver_active(dev_handle, 0) != 0)
	{
		printf("Kernel driver is active. Detach and try again\n");
		libusb_detach_kernel_driver(dev_handle, 0);
	}

	dev = libusb_get_device(dev_handle);

	// TODO: Set configuration before claiming device using libusb_set_configuration(), if needed
	// Found '1' as the only bConfigurationValue from reverse-engineering the device
	// Also found '0' as the only bInterfaceNumber

	if(libusb_claim_interface(dev_handle, 0) == 0)
	{
		printf("Claimed the fingerprint scanner. I/O operations can now be performed\n");
	}

	else
	{
		printf("Failed to claim the device, something bad happened :(\n");
		return -1;
	}

	// Communicating with the device, gl

	// DESCRIPTOR Request DEVICE @ 0x80

	printf("Attmpting to get DECRIPTOR for device...\n");

	struct libusb_device_descriptor device_descriptor;
	ret = libusb_get_device_descriptor(dev, &device_descriptor);
	check_safe(ret);
	
	print_device_descriptor(device_descriptor);

	// Getting current configuration
	struct libusb_config_descriptor *dev_config = malloc(sizeof(struct libusb_config_descriptor));
	ret = libusb_get_active_config_descriptor(dev, &dev_config);
	check_safe(ret);

	print_device_configuration(dev_config);

	printf("Releasing the device...\n");
	
	libusb_release_interface(dev_handle, 0);
	libusb_close(dev_handle);
	libusb_exit(context);

	return(0);
}

// Function to check if device interactions worked fine
void check_safe(int ret)
{
	if(ret != 0)
	{
		printf("Something bad happened: %d\n", ret);
		exit(-1);
	}
}

// Function to print all the device descriptor values
void print_device_descriptor(struct libusb_device_descriptor device_descriptor)
{
	printf("Need to print device decriptor here :p\n");
}

// Function to print the configs
void print_device_configuration(struct libusb_config_descriptor *dev_config)
{
	printf("\tbNumInterfaces = \t%d\n", dev_config -> bNumInterfaces);
	
	const struct libusb_interface *interface = dev_config -> interface;

	const struct libusb_interface_descriptor *interface_descriptor = interface -> altsetting;

	printf("\tbNumEndpoints: \t%d\n", interface_descriptor -> bNumEndpoints);

	for(int i = 0; i < interface_descriptor -> bNumEndpoints; i++)
	{
		printf("\t\tbEndpointAddress: 0x%01x\n", interface_descriptor -> endpoint[i].bEndpointAddress);
	}
}