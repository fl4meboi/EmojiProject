// Copyright 2023 Dexter.Wan. All Rights Reserved. 
// EMail: 45141961@qq.com

#if !defined(MQTTPERSISTENCE_H)
#define MQTTPERSISTENCE_H

#if defined(__cplusplus)
 extern "C" {
#endif

#include "Clients.h"
#include "MQTTProperties.h"

/** Stem of the key for a sent PUBLISH QoS1 or QoS2 */
#define PERSISTENCE_PUBLISH_SENT "s-"
/** Stem of the key for a sent PUBREL */
#define PERSISTENCE_PUBREL "sc-"
/** Stem of the key for a received PUBLISH QoS2 */
#define PERSISTENCE_PUBLISH_RECEIVED "r-"

/** Stem of the key for a sent MQTT V5 PUBLISH QoS1 or QoS2 */
#define PERSISTENCE_V5_PUBLISH_SENT "s5-"
/** Stem of the key for a sent MQTT V5 PUBREL */
#define PERSISTENCE_V5_PUBREL "sc5-"
/** Stem of the key for a received MQTT V5 PUBLISH QoS2 */
#define PERSISTENCE_V5_PUBLISH_RECEIVED "r5-"

/** Stem of the key for an async client command */
#define PERSISTENCE_COMMAND_KEY "c-"
/** Stem of the key for an MQTT V5 async client command */
#define PERSISTENCE_V5_COMMAND_KEY "c5-"
/** Stem of the key for an client incoming message queue */
#define PERSISTENCE_QUEUE_KEY "q-"
/** Stem of the key for an MQTT V5 incoming message queue */
#define PERSISTENCE_V5_QUEUE_KEY "q5-"
/** Maximum length of a stem for a persistence key */
#define PERSISTENCE_MAX_STEM_LENGTH 4
/** Maximum allowed length of a persistence key */
#define PERSISTENCE_MAX_KEY_LENGTH 10
/** Maximum size of an integer sequence number appended to a persistence key */
#define PERSISTENCE_SEQNO_LIMIT 1000000 /*10^(PERSISTENCE_MAX_KEY_LENGTH - PERSISTENCE_MAX_STEM_LENGTH)*/

int MQTTPersistence_create(MQTTClient_persistence** per, int type, void* pcontext);
int MQTTPersistence_initialize(Clients* c, const char* serverURI);
int MQTTPersistence_close(Clients* c);
int MQTTPersistence_clear(Clients* c);
int MQTTPersistence_restorePackets(Clients* c);
void* MQTTPersistence_restorePacket(int MQTTVersion, char* buffer, size_t buflen);
void MQTTPersistence_insertInOrder(List* list, void* content, size_t size);
int MQTTPersistence_putPacket(SOCKET socket, char* buf0, size_t buf0len, int count,
						char** buffers, size_t* buflens, int htype, int msgId, int scr, int MQTTVersion);
int MQTTPersistence_remove(Clients* c, char* type, int qos, int msgId);
void MQTTPersistence_wrapMsgID(Clients *c);

typedef struct
{
	char struct_id[4];
	int struct_version;
	int payloadlen;
	void* payload;
	int qos;
	int retained;
	int dup;
	int msgid;
	MQTTProperties properties;
} MQTTPersistence_message;

typedef struct
{
	MQTTPersistence_message* msg;
	char* topicName;
	int topicLen;
	unsigned int seqno; /* only used on restore */
} MQTTPersistence_qEntry;

int MQTTPersistence_unpersistQueueEntry(Clients* client, MQTTPersistence_qEntry* qe);
int MQTTPersistence_persistQueueEntry(Clients* aclient, MQTTPersistence_qEntry* qe);
int MQTTPersistence_restoreMessageQueue(Clients* c);
#ifdef __cplusplus
     }
#endif

#endif
