#include <sstream>
#include "utils.h"
namespace myUtils
{
std::wstring toWstring(int value)
{
	std::wstringstream wss;
	wss << value;
	return wss.str();
}
} // namespace myUtils