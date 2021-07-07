import usb.core
import usb.util

dev = usb.core.find(idVendor=0x138a, idProduct=0x00ab)

if dev is None:
	raise ValueError("Fingerprint scanner not found :(")

else:
	print('Found the fingerprint scanner. Proceeding...\n\n')

dev.set_configuration()

# cfg = dev.get_active_configuration()


cfg = dev.get_active_configuration()
intf = cfg[(0,0)]

# print(intf)

for breh in intf:
	print(hex(breh.bEndpointAddress))

print(hex(usb.util.ENDPOINT_OUT))
print(int('0x80', base=16)) # = 

# ep = usb.util.find_descriptor(intf, custom_match = lambda e: usb.util.endpoint_direction(e.bEndpointAddress) == usb.util.ENDPOINT_OUT)

# print(ep)

# assert ep is not None

# # write the data
# ep.write('test')