#ifndef MYMESSAGE_H
#define MYMESSAGE_H

extern class Interface;
#include <memory>

struct MyMsg {
	Interface* pInterface;
	unsigned int time;
	unsigned int msg;
	std::shared_ptr<void> Data;
};

#endif