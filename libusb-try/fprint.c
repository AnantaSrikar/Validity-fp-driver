#include<stdio.h>
#include<stdlib.h>
#include<libusb-1.0/libusb.h>

int main(int argc, char **argv)
{
	libusb_device_handle *fprint_dev_handle = NULL;
	libusb_device *fprint_dev = NULL;
	
	libusb_context *context = NULL;

	int ret = libusb_init(&context);

	if(ret < 0)
	{
		perror("libusb_init");
		exit(1);
	}

	fprint_dev_handle = libusb_open_device_with_vid_pid(context, 5002, 171);

	if(fprint_dev_handle == NULL)
	{
		printf("Fingerprint device not found :(\n");
		
		libusb_close(fprint_dev_handle);
		libusb_exit(context);
		return -1;
	}

	fprint_dev = libusb_get_device(fprint_dev_handle);

	

	libusb_close(fprint_dev_handle);
	libusb_exit(context);



	return(0);
}