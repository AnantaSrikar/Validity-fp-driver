import usb.core
import usb.util
import usb.control

ID_VENDOR = 0x138a
ID_PRODUCT = 0x00ab

def main():

	# Initialize the device
	dev = usb.core.find(idVendor=ID_VENDOR, idProduct=ID_PRODUCT)

	# Check if the scanner is found
	if dev is None:
		raise ValueError("Fingerprint scanner not found :(")

	else:
		print('Found the fingerprint scanner. Proceeding...\n')

	# print(dev)

	# Set the first configuration, which is the only configuration
	dev.set_configuration()

	# get an endpoint instance
	config = dev.get_active_configuration()
	
	interface = config[(0,0)]

	# CLEAR FEATURE Requests as done in windows driver
	endpoint_clr = [0x01, 0x81, 0x82, 0x83, 0x84]
	
	for endpoint in endpoint_clr:
		dev.clear_halt(endpoint)

	print("Finished clearing endpoints!")

	# Send URB_CONTROL IN packet
	bmRequestType = 0xc0	# Determines the transfer direction
	bRequest = 0x14	# 20
	wValue = 0
	wIndex = 0
	wLength = 2

	data = dev.ctrl_transfer(bmRequestType, bRequest, wValue, wIndex, wLength)

	# We should receive 0s
	for bit in data:
		if bit:
			print("Something is wrong with the scanner!")

	print("Received data: ", data)

	# Now, BULK OUT

if __name__ == "__main__":
	main()