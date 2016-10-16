/*
 * Publisher.hpp
 *
 *  Created on: Oct 15, 2016
 *      Author: Declan Easton
 */

#ifndef PUBLISHER_HPP_
#define PUBLISHER_HPP_

#include "MQTTClient.h"

class Publisher {
private:
	unsigned char *id;
	MQTTClient client;
public:
	Publisher();
	~Publisher();
	void Send(bool enable);
};



#endif /* PUBLISHER_HPP_ */
