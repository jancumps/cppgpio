#include <filesystem>
#include <string>
#include <cassert>
#include <fcntl.h>
#include <sysfsdevice_filehandler.h>
#include <unistd.h>


namespace dgpio {

const std::string sysfsdevice_filehandler::device_path =
		std::string("/sys/class/gpio");

bool sysfsdevice_filehandler::exists(const unsigned gpio) {
	return std::filesystem::is_symlink(directoryname(gpio));
}

void sysfsdevice_filehandler::exp(const unsigned gpio) {

	FILE *f = fopen(exportname().c_str(), "w");
    fprintf(f, std::to_string(gpio).c_str());
    fclose(f);
}

void sysfsdevice_filehandler::unexp(const unsigned gpio) {
	FILE *f = fopen(unexportname().c_str(), "w");
    fprintf(f, std::to_string(gpio).c_str());
    fclose(f);
}

void sysfsdevice_filehandler::init(const unsigned gpio,
		const pin<sysfsdevice_filehandler>::dir dir,
		const pin<sysfsdevice_filehandler>::status status) {
	if (!exists(gpio)) {
		exp(gpio);
	}
	FILE *f = nullptr;
	while (f == nullptr) { // when you're not root, it takes a bit to get a valid pointer
		f = fopen(directionname(gpio).c_str(), "w");
	}
	switch (dir) {
	case pin<sysfsdevice_filehandler>::dir::in:
		fprintf(f, "in");
		break;
	case pin<sysfsdevice_filehandler>::dir::out:
		fprintf(f, "out");
		break;
	}
    fclose(f);
	set(gpio, status);
}

void sysfsdevice_filehandler::deinit(const unsigned gpio) {
	if (exists(gpio)) {
		unexp(gpio);
	}
}

pin<sysfsdevice_filehandler>::status sysfsdevice_filehandler::get(const unsigned gpio) {
	pin<sysfsdevice_filehandler>::status retval = pin<sysfsdevice_filehandler>::status::off;
    char value_str[1];
	int fd = open(valuename(gpio).c_str(), O_RDONLY);
	read(fd, value_str, 1);
    if (value_str[0] == '0') {
    	retval = pin<sysfsdevice_filehandler>::status::off;
	} else if (value_str[0] == '1') {
    	retval = pin<sysfsdevice_filehandler>::status::on;
	} else {
		assert(false); // invalid
	}
    close(fd);

	return retval;
}


void sysfsdevice_filehandler::set(const unsigned gpio,
		const pin<sysfsdevice_filehandler>::status status) {
	FILE *f = nullptr;
	while (f == nullptr) { // when you're not root, it may a bit to get a valid pointer
		f = fopen(valuename(gpio).c_str(), "w");
	}
	switch (status) {
	case pin<sysfsdevice_filehandler>::status::off:
		fprintf(f, "0");
		break;
	case pin<sysfsdevice_filehandler>::status::on:
		fprintf(f, "1");
		break;
	}
    fclose(f);
}

} // namespace dgpio
