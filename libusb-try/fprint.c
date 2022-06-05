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
	// All the pointers to handle the device
	libusb_device_handle *fprint_dev_handle = NULL;
	libusb_device *fprint_dev = NULL;
	
	libusb_context *context = NULL;

	int ret = libusb_init(&context);

	if(ret < 0)
	{
		perror("libusb_init");
		exit(1);
	}

	fprint_dev_handle = libusb_open_device_with_vid_pid(context, DEV_VID, DEV_PID);

	if(fprint_dev_handle == NULL)
	{
		printf("Fingerprint device not found :(\n");
		
		libusb_close(fprint_dev_handle);
		libusb_exit(context);
		return -1;
	}

	// fprint_dev = libusb_get_device(fprint_dev_handle);

	// Claiming the device if used by some other driver
	if(libusb_kernel_driver_active(fprint_dev_handle, 0) != 0)
	{
		printf("Kernel driver is active. Detach and try again\n");
		libusb_detach_kernel_driver(fprint_dev_handle, 0);
	}

	// TODO: Set configuration before claiming device using libusb_set_configuration(), if needed
	// Found '1' as the only bConfigurationValue from reverse-engineering the device
	// Also found '0' as the only bInterfaceNumber

	if(libusb_claim_interface(fprint_dev_handle, 0) == 0)
	{
		printf("Claimed the fingerprint scanner. I/O operations can now be performed\n");
	}

	else
	{
		printf("Failed to claim the device, something bad happened :(\n");
		return -1;
	}

	// Communicating with the device, gl

	unsigned char *stuff;
	stuff = (unsigned char*)malloc(sizeof(unsigned char) * 2);

	/*
		bmRequestType = 0xc0
		bRequest = 20
		wValue = 0x0000
		wIndex = 0x0000 (0)
		wLength = 2
		timeout = 0 (Unlimited timeout)
	*/

	int reply = libusb_control_transfer(fprint_dev_handle, 0xc0, 20, 0x0000, 0x0000, stuff, 2, 0);
	
	printf("Reply: %d\nStuff: '", reply);
	for(int i = 0; i < 2; i++)
		printf("%u", stuff[i]);
	printf("'\n");

	printf("Releasing the device...\n");
	libusb_release_interface(fprint_dev_handle, 0);
	

	libusb_close(fprint_dev_handle);
	libusb_exit(context);



	return(0);
}