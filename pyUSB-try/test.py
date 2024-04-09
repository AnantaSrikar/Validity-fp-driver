import usb.core
import usb.util
import usb.control
import array

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
	data = [1]
	bulk_out_data = array.array('B', data)
	bytes_written = dev.write(0x01, bulk_out_data)
	print("Bytes written: ", bytes_written)

	# BULK IN
	urb_len = 38 # Bytes. Got this value off reverse engineering
	bulk_in_data = dev.read(0x81, urb_len)
	print("Bytes read: ", bulk_in_data)
	
if __name__ == "__main__":
	main()