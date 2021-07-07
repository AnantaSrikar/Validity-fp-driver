# Validity-fp-driver
Attempting to write a [fprint](https://fprint.freedesktop.org) compatable driver for the Validity fingerprint scanner [138a:00ab](https://linux-hardware.org/index.php?id=usb:138a-00ab).

## Tasks
- [x] Reverse engineering the Windows driver by capturing the packets sent and recieved to the scanner.
- [x] Finding out all the endpoints and their types (Mentioned in [info.txt](https://github.com/AnantaSrikar/Validity-fp-driver/blob/master/info.txt))
- [ ] Writing a basic driver with [libusb](https://libusb.info/) and checking if all the endpoints work as intented.
- [ ] Understand how [libfprint](https://gitlab.freedesktop.org/libfprint/libfprint) works and make a compatible driver for it.