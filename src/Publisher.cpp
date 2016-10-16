#include "Publisher.hpp"
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>


#define ADDRESS     "tcp://52.25.184.170"
//#define ADDRESS     "tcp://localhost:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "sensorData"
#define QOS         1
#define TIMEOUT     10000L

unsigned char *getMACAddress()
{
    struct ifreq ifr;
    struct ifconf ifc;
    char buf[1024];
    int success = 0;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1) { /* handle error*/ };

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { /* handle error */ }

    struct ifreq* it = ifc.ifc_req;
    const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                    success = 1;
                    break;
                }
            }
        }
        else { /* handle error */ }
    }

    unsigned char mac_address[6];

    if (success) memcpy(mac_address, ifr.ifr_hwaddr.sa_data, 6);

    return mac_address;
}

Publisher::Publisher() {
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	int rc;

	MQTTClient_create(&client, ADDRESS, CLIENTID,
		MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect, return code %d\n", rc);
		exit(-1);
	}

	id = getMACAddress();
}

void Publisher::Send(bool enable) {
	char buffer[40];
	int length = sprintf(buffer, "{'id':%s,'active':%i}", id, enable);
	char *message = new char[length];
	strncpy(message, buffer, length);
	//char *message = "Test";
	//int length = strlen(message);
	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	pubmsg.payload = (void *)(message);
	pubmsg.payloadlen = length;
	pubmsg.qos = QOS;
	pubmsg.retained = 0;

	MQTTClient_deliveryToken token;
	MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
	delete message;
}

Publisher::~Publisher() {
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

}
