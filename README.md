# cppgpio
C++ embeddded GPIO library (supports sysfs, memmap register)

## Devices supported:
1. Raspberry Pi
    1. memory mapped register access (/dev/gpiomem)
    2. sysfs with C++ stream (/sys/class/gpio)
    3. sysfs with classic C file IO (/sys/class/gpio)

## Use example:
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

## small footprint:
The code uses C++ constructs that are resolved at compile time.  
To keep the firmware size as small as possible, only add the source file of the implementation that you use in the build. 

## Related blog posts:
[C++ gpio library for Raspberry Pi - Pt 1: Design and How To Use](https://community.element14.com/products/raspberry-pi/b/blog/posts/c-gpio-library-for-raspberry-pi)  
[C++ gpio library for Raspberry Pi - Pt 2: Plug In drivers for direct register GPIO, or file system based GPIO](https://community.element14.com/products/raspberry-pi/b/blog/posts/c-gpio-library-for-raspberry-pi---pt-2-plug-in-drivers-for-direct-register-gpio-or-file-system-based-gpio)