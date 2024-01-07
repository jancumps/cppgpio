#ifndef MEMMAPDEVICE_H_
#define MEMMAPDEVICE_H_

#include <fcntl.h>    // for open
#include <sys/mman.h> // for mmap
#include "pin.h"

namespace dgpio {

class memmapdevice {
public:
	static void init(const unsigned gpio,
			const pin<memmapdevice>::dir dir,
			const pin<memmapdevice>::status status = pin<memmapdevice>::status::off);

	static void deinit(const unsigned gpio) {
		// https://linux.die.net/man/2/close
		// It is probably unwise to close file descriptors while they may
		// be in use by system calls in other threads in the same process
	}

	static pin<memmapdevice>::status get(const unsigned gpio) {
		return gpiomem[13] & (1 << gpio) ? pin<memmapdevice>::status::on : pin<memmapdevice>::status::off; // GPLEV0
	}

	static void set(const unsigned gpio, const pin<memmapdevice>::status status) {
		// always set status first
		switch (status) {
		case pin<memmapdevice>::status::on:
			gpiomem[7] |= (1 << gpio); // GPSET0
			break;
		case pin<memmapdevice>::status::off:
			gpiomem[10] |= (1 << gpio); // GPCLR0
			break;
		}
	}

private:
	static unsigned int *gpiomem;
	static int fdgpio;
};

} // namespace dgpio

#endif /* MEMMAPDEVICE_H_ */
