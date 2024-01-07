// uncomment the gpio handler that you want to use
//#define DPGIO_SYSFS_FILEHANDLER
//#define DPGIO_SYSFS_STREAM
#define DPGIO_MEMMAP

#include "pin.h"

#if defined DPGIO_SYSFS_FILEHANDLER
#include <sysfsdevice_filehandler.h>
typedef dgpio::pin<dgpio::sysfsdevice_filehandler> pin;
#elif defined DPGIO_SYSFS_STREAM
#include "sysfsdevice_stream.h"
typedef dgpio::pin<dgpio::sysfsdevice_stream> pin;
#elif defined DPGIO_MEMMAP
#include "memmapdevice.h"
typedef dgpio::pin<dgpio::memmapdevice> pin;
#endif

#include <stdlib.h> // atoi
#include <unistd.h> // usleep

int main(int argc, char *const *argv) {
	unsigned int gpio;
    if (argc < 2) {
		gpio = 21;
    } else {
    	gpio = atoi(argv[1]);
    }

	pin p = pin(gpio);
	p.init(pin::dir::out, pin::status::off);

	while(true) {
		p.set(p.get() == pin::status::off ? pin::status::on : pin::status::off);
		usleep(1000000);
	}
	p.deinit();
}

