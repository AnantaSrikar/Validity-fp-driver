#include<stdio.h>
#include<stdlib.h>
#include<libusb-1.0/libusb.h>

int main(int argc, char **argv)
{
	// Function prototypes
	void printDevices(libusb_device*);

	libusb_device **devs;
	libusb_context *context = NULL;

	size_t list;
	
	int ret = libusb_init(&context);

	if(ret < 0)
	{
		perror("libusb_init");
		exit(1);
	}

	list = libusb_get_device_list(context, &devs);

	printf("Found %d USB devices\n", list);

	if(list < 0)
	{
		fprintf(stderr, "Error in getting device list\n");
		
		libusb_free_device_list(devs, 1);
		libusb_exit(context);

		exit(1);
	}

	for(int i = 0; i < list; i++)
	{
		printDevices(devs[i]);
	}

	libusb_free_device_list(devs, 1);
	libusb_exit(context);

	return(0);
}

void printDevices(libusb_device *dev)
{
	struct libusb_device_descriptor desc;
	struct libusb_config_descriptor *config;

	const struct libusb_interface *inter;
	const struct libusb_interface_descriptor *interdesc;
	const struct libusb_endpoint_descriptor *endpointdesc;

	int ret = libusb_get_device_descriptor(dev, &desc);

	if(ret < 0)
	{
		printf("Error in getting device description\n");
		return;
	}

	printf("Number of pos configs = %d\n", desc.bNumConfigurations);
	printf("Device class : ProdID = %d:%d\n", desc.idVendor, desc.idProduct);

	libusb_get_config_descriptor(dev, 0, &config);

	printf("Interface: %d\n", config -> bNumInterfaces);

	for(int i = 0; i < config -> bNumInterfaces; i++)
	{
		inter = &config -> interface[i];

		printf("	Number of alt settings: %d\n", inter -> num_altsetting);

		for(int j = 0; j < inter -> num_altsetting; j++)
		{
			interdesc = &inter -> altsetting[j];
			printf("		Interface number = %d\n", interdesc -> bInterfaceNumber);
			printf("		Number of endpoints = %d\n", interdesc -> bNumEndpoints);

			for(int k = 0; k < interdesc -> bNumEndpoints; k++)
			{
				endpointdesc = &interdesc -> endpoint[k];
				printf("			Desc Type: %d\n", endpointdesc -> bDescriptorType);
				printf("			EP Addr: %d\n", endpointdesc -> bEndpointAddress);
			}
		}
	}

	printf("\n\n");
	libusb_free_config_descriptor(config);
}