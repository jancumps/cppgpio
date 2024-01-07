#include "memmapdevice.h"


namespace dgpio {

// static members to be initialised in the cpp file
// memory mapped registers
unsigned int *memmapdevice::gpiomem = nullptr;
// gpio memory map file interface
int memmapdevice::fdgpio = -1;

void memmapdevice::init(const unsigned gpio,
		const pin<memmapdevice>::dir dir,
		const pin<memmapdevice>::status status) {
	// inspiration
	// https://www.cs.uaf.edu/2016/fall/cs301/lecture/11_09_raspberry_pi.html
	if (fdgpio > 0) { // memmap already initialised?
		return;
	}
	// map gpio memory to gpiomem
	fdgpio=open("/dev/gpiomem", O_RDWR);
	if (fdgpio > 0) {
		gpiomem = (unsigned int *)mmap(0, 4096, PROT_READ+PROT_WRITE, MAP_SHARED, fdgpio,0);
	}

	set(gpio, status);

	unsigned int index = gpio / 10; // GPFSELn register to set this pin direction
	unsigned int offset = (gpio % 10) * 3; // lsb for this pin's 3 bit status in that register

	unsigned int curval = gpiomem[index];
	curval &= ~0b111 << (offset);

	switch (dir) {
	case pin<memmapdevice>::dir::in:
		break;
	case pin<memmapdevice>::dir::out:
		curval |= 0b1 << (offset);
		break;
	}


	gpiomem[index] = curval;
}

} // namespace dgpio
