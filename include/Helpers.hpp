#pragma once

#define logerr() std::cout << "\033[31;1mError\033[0m [" << __FILE__ << ":" << __LINE__ << "]: "
#define logwarn() std::cout << "\033[33;1mWarning\033[0m [" << __FILE__ << ":" << __LINE__ << "]: "
#define logend() std::endl