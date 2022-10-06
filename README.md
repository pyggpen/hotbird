# hotbird
A deployable sensor pack for temperature, using the ESP-01.

This code was originally supposed to be part of a deployable sensor family built around the ESP-01 and cheap sensors like the DHT-22 humidity sensor and the DS1820 temerpature devices - essentially anything the ESP-01 could read with one or two pins was fair game. I had plans for more devices, including voltage, current, and general-purpose input devices.

Unfortunately, I found that the ESP family has some nasty (documented) spurs right around the frequencies used by certain garage door openers, and other RF devices like the venerable X10 family. That, and coupled with the instability of the DHT sensors, I shelved the project for a while.

See https://pygg.xyz/projects/index.php/2022/04/03/the-gotchas-of-cheap-technology/ for my complete writeup on the subject.
