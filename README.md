# cppgpio
C++ embeddded GPIO library (supports sysfs, memmap register)

## Devices supported:
1. Raspberry Pi
    1. memory mapped register access (/dev/gpiomem)
    2. sysfs with C++ stream (/sys/class/gpio)
    3. sysfs with classic C file IO (/sys/class/gpio)

## Use example:
```
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
Use the include and typedef for the implementation you want to use. Your code will be the same regardless of the choice of GPIO driver selected. In this example, the memmap device driver is used. The library will talk directly to the ARM CPU's registers to control the pins.  

```
// #include <sysfsdevice_filehandler.h>
// typedef dgpio::pin<dgpio::sysfsdevice_filehandler> pin;

// #include "sysfsdevice_stream.h"
// typedef dgpio::pin<dgpio::sysfsdevice_stream> pin;

#include "memmapdevice.h"
typedef dgpio::pin<dgpio::memmapdevice> pin;
```

## API:
```
namespace dgpio {
template <class D>
class pin {
public:

    enum class status : unsigned { off  = 0u, on };
    enum class dir { in, out };

    pin(unsigned gpio);
	void init(dir dir = dir::in, status status = status::off);
	void deinit();

	status get();
	void set(status status);
	
};	
}
```
- constructor: `pin(gpio)`
- init: `init(dir, initial status)`
- deinit: `deinit()`
- getter: `get(): status`
- setter: `set(status)`

## Add to your project:
Get the latest version: 
`git clone https://github.com/jancumps/cppgpio.git`  
Add the `src/lib` folder to your include path  
Add the relevant cpp source file(s) from `src/lib` to your build list. Ideally, only build and link the one that you're using. See section [Small footprint](#small-footprint) below.  

## Examples:
There are 2 examples in the `src/` folder:  
- example.cpp: read or write a pin.
- blinky.cpp: blink a pin


## User requirements:
1. Raspberry Pi  
On Raspberry OS, the user that runs your program has to be in the `gpio` group. When using Raspberry's tools to create an image, your user will be in that group.  

## Small footprint:
The code uses C++ constructs that are resolved at compile time.  
To keep the firmware size as small as possible, only add the source file of the implementation that you use in the build. 

## Tested with:
- Raspberry Pi 3 Model B, raspbian bullseye 32 bit lite, gcc version 10.2.1 20210110 (Raspbian 10.2.1-6+rpi1)

## Related blog posts:
[C++ gpio library for Raspberry Pi - Pt 1: Design and How To Use](https://community.element14.com/products/raspberry-pi/b/blog/posts/c-gpio-library-for-raspberry-pi)  
[C++ gpio library for Raspberry Pi - Pt 2: Plug In drivers for direct register GPIO, or file system based GPIO](https://community.element14.com/products/raspberry-pi/b/blog/posts/c-gpio-library-for-raspberry-pi---pt-2-plug-in-drivers-for-direct-register-gpio-or-file-system-based-gpio)