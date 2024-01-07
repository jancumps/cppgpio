# cppgpio
C++ embeddded GPIO library (supports sysfs, memmap register)

## Devices supported:
1. Raspberry Pi
    1. memory mapped register access (/dev/gpiomem)
    2. sysfs with C++ stream (/sys/class/gpio)
    3. sysfs with classic C file IO (/sys/class/gpio)

## Use:
```
// #include <sysfsdevice_filehandler.h>
// typedef dgpio::pin<dgpio::sysfsdevice_filehandler> pin;

// #include "sysfsdevice_stream.h"
// typedef dgpio::pin<dgpio::sysfsdevice_stream> pin;

#include "memmapdevice.h"
typedef dgpio::pin<dgpio::memmapdevice> pin;

pin p21 = pin(21);
p21.init(pin::dir::out, pin::status::off);
p21.set(pin::status::on);

p21.init(pin::dir::in);
pin::status status = pin::status::on;
while (status != pin::status::off) {
	status = p21.get();
}

p21.deinit();
```
