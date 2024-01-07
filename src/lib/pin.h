#ifndef PIN_H_
#define PIN_H_

#include "pin.h"

namespace dgpio {

template <class D>
class pin {
public:
    enum class status : unsigned {
        off  = 0u,
        on
    };

    enum class dir {
        in, // default for Pi
        out
    };

    pin(const unsigned gpio) : gpio(gpio) {}

	void init(const dir dir = dir::in,
			const status status = status::off) {
			D::init(gpio, dir, status);
	}

	void deinit() {
		D::deinit(gpio);
	}

	status get() {
		return D::get(gpio);
	}
	void set(const status status) {
		D::set(gpio, status);
	}

private:
	unsigned gpio;
};

} // namespace dgpio

#endif /* PIN_H_ */
