#ifndef SYSFSDEVICE_FILEHANDLER_H_
#define SYSFSDEVICE_FILEHANDLER_H_

#include <string>
#include "pin.h"

namespace dgpio {

class sysfsdevice_filehandler {
public:
	static void init(const unsigned gpio,
			const pin<sysfsdevice_filehandler>::dir dir,
			const pin<sysfsdevice_filehandler>::status status);
	static void deinit(unsigned gpio);
	static pin<sysfsdevice_filehandler>::status get(const unsigned gpio);
	static void set(const unsigned gpio,
			const pin<sysfsdevice_filehandler>::status status);


private:
	static bool exists (const unsigned gpio);
	static void exp(const unsigned gpio);
	static void unexp(const unsigned gpio);

	static const std::string device_path;
	inline static std::string directoryname(unsigned gpio) { return device_path + "/gpio" + std::to_string(gpio); }
	inline static std::string exportname() { return device_path + "/export"; }
	inline static std::string unexportname() { return device_path + "/unexport"; }
	inline static std::string directionname(const unsigned gpio) { return directoryname(gpio) + "/direction"; }
	inline static std::string valuename(const unsigned gpio) { return directoryname(gpio) + "/value"; }
};

} // namespace dgpio

#endif /* SYSFSDEVICE_FILEHANDLER_H_ */
