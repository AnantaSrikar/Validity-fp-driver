# TODO
### Replicate caputres packets to see if everything is working the same way
- [x] Make DESCRIPTOR Request DEVICE (0x80)
- [x] Make DESCRIPTOR Request CONFIGURATION (0x80)
- [x] Make another DESCRIPTOR Request CONFIGURATION (0x80) (Why?) (Turns out all the above three happen at device initialization by the OS!)
- [ ] Make an URB_CONTROL in (0x80) call - Done, need to check output again
- [ ] Make one URB_BULK out (0x01) followed by two URB_BULK in (0x81) a few times
- [ ] Finally, Make multiple URB_INTERRUPT in (0x83)

### Make libfprintd compatible driver
1. Understand libfprintd's required endpoints
2. Check existing implementation of validity drivers [here](https://gitlab.freedesktop.org/libfprint/libfprint/-/tree/master/libfprint/drivers)
3. Try to make modifications to existing driver code and make working driver
4. If above doesn't work, make a libfprint compatible driver from scratch