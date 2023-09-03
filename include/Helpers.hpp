#pragma once

#include <string>

#define logerr() std::cout << "\033[31;1mError\033[0m [" << __FILE__ << ":" << __LINE__ << "]: "
#define logwarn() std::cout << "\033[33;1mWarning\033[0m [" << __FILE__ << ":" << __LINE__ << "]: "
#define logend() std::endl

namespace Time {
	static inline std::string GetTime(const std::string& format) {
		std::string result;
		std::time_t _time = std::time(nullptr);
		struct tm* time = std::localtime(&_time);

		auto size = 1;

		auto vres = std::strftime(&result[0], size, format.c_str(), time);

		while(vres == 0) {
			size *= 2;
			result.resize(size);

			vres = std::strftime(&result[0], size, format.c_str(), time);
		}

		result.resize(vres);

		return result;
	}
};