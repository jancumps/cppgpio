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

#include <stdlib.h>     /* atoi */

int main(int argc, char *const *argv) {
    if (argc < 2) {
		return -1;
    }

    int retval = 0;

    unsigned int gpio = atoi(argv[1]);
    pin::dir dir = pin::dir::in;
    pin::status status = pin::status::off;

    if (argc == 3) { // out
    	dir = pin::dir::out;
    	if (atoi(argv[2]) == 1) {
    		status = pin::status::on;
    	}
    }

	pin p = pin(gpio);
	p.init(dir);

	if (dir == pin::dir::in) {
		pin::status s = p.get();
		retval = s == pin::status::on ? 1 : 0;
	} else {
		p.set(status);
	}

	p.deinit();
	return retval;
}

