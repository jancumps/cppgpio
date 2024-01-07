#include <sysfsdevice_stream.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <cassert>

#include <iostream>

namespace dgpio {

const std::string sysfsdevice_stream::device_path = std::string("/sys/class/gpio");

bool sysfsdevice_stream::exists(const unsigned gpio) {
	return std::filesystem::is_symlink(directoryname(gpio));
}

void sysfsdevice_stream::exp(const unsigned gpio) {
	std::ofstream of(exportname());
	of << std::unitbuf; // unbuffered
	of << gpio;
	of.close();
}

void sysfsdevice_stream::unexp(const unsigned gpio) {
	std::ofstream of(unexportname());
	of << std::unitbuf; // unbuffered
	of << gpio;
	of.close();
}

void sysfsdevice_stream::init(const unsigned gpio,
		const pin<sysfsdevice_stream>::dir dir,
		const pin<sysfsdevice_stream>::status status) {
	if (!exists(gpio)) { // export pin if needed
		exp(gpio);
	}
	std::ofstream of;
	while(!of.is_open()) { // when you're not root, it takes a bit to get a valid open file
		of.open(directionname(gpio));
	}
	of << std::unitbuf; // unbuffered
	switch (dir) { // set dir
	case pin<sysfsdevice_stream>::dir::in:
		of << "in";
		break;
	case pin<sysfsdevice_stream>::dir::out:
		of << "out";
		break;
	}
	of.close();
	set(gpio, status); //
}

void sysfsdevice_stream::deinit(const unsigned gpio) {
	if (exists(gpio)) {
		unexp(gpio);
	}
}

pin<sysfsdevice_stream>::status sysfsdevice_stream::get(const unsigned gpio) {
	pin<sysfsdevice_stream>::status retval = pin<sysfsdevice_stream>::status::off;
	std::ifstream is(valuename(gpio));
	std::string value;

	is >> value;
	is.close();
	if (value == "0") {
		retval = pin<sysfsdevice_stream>::status::off;
	} else if (value == "1") {
		retval = pin<sysfsdevice_stream>::status::on;
	} else {
		assert(false); // invalid
	}
	return retval;
}

void sysfsdevice_stream::set(const unsigned gpio,
		const pin<sysfsdevice_stream>::status status) {
	std::ofstream of;
	while(!of.is_open()) { // when you're not root, it may take a bit to get a valid open file
		of.open(valuename(gpio));
	}
	of << std::unitbuf; // unbuffered
	switch (status) {
	case pin<sysfsdevice_stream>::status::off:
		of << "0";
		break;
	case pin<sysfsdevice_stream>::status::on:
		of << "1";
		break;
	}
	of.close();
}

} // namespace dgpio
