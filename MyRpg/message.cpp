#include "message.h"
Message &Message::getInstance()
{
	static Message instance;
	return instance;
}
bool Message::poll() { return peekmessage(&msg, EX_MOUSE | EX_KEY); }
