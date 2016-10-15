/*
 * Author: Jessica Gomez <jessica.gomez.hernandez@intel.com>
 * Copyright (c) 2015 - 2016 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * @file
 * @ingroup basic
 * @brief On board LED blink C++
 *
 * Blink the on board LED by writing a digital value to an output pin
 * using the IO mraa library.
 *
 * @date 29/09/2015
 */

#include "mraa.hpp"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include "Publisher.hpp"

/*
 * On board LED blink C++ example
 *
 * Demonstrate how to blink the on board LED, writing a digital value to an
 * output pin using the MRAA library.
 * No external hardware is needed.
 *
 * - digital out: on board LED
 *
 * Additional linker flags: none
 */

#define ADDRESS     "tcp://localhost:1883"
#define CLIENTIDSUB "ExampleClientSub"
#define TOPIC       "MQTT Examples"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;
int done = 0;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
    deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;

    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: %s\n", message->payload);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    done += 1;

    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
//	int rc;
//    MQTTClient subClient;
//	MQTTClient_connectOptions subConn_opts = MQTTClient_connectOptions_initializer;
//
//	MQTTClient_create(&subClient, ADDRESS, CLIENTIDSUB,
//		MQTTCLIENT_PERSISTENCE_NONE, NULL);
//	subConn_opts.keepAliveInterval = 20;
//	subConn_opts.cleansession = 1;
//
//	MQTTClient_setCallbacks(subClient, NULL, connlost, msgarrvd, delivered);
//
//	if ((rc = MQTTClient_connect(subClient, &subConn_opts)) != MQTTCLIENT_SUCCESS)
//	{
//		printf("Failed to connect, return code %d\n", rc);
//		exit(-1);
//	}
//
//	MQTTClient_subscribe(subClient, TOPIC, QOS);
	printf("Start");

	Publisher publisher = Publisher();
	publisher.Send(true);
	publisher.Send(true);
	publisher.Send(false);
	publisher.Send(false);
	publisher.Send(true);

	printf("Done");

//    while(done < 5) {
//    }
//
//
//    MQTTClient_disconnect(subClient, 10000);
//    MQTTClient_destroy(&subClient);

    return 0;
}
