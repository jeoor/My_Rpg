#include <sstream>
#include "utils.h"

namespace my_utils {
	std::wstring to_wstring(int value) {
		std::wstringstream wss;
		wss << value;
		return wss.str();
	}
}