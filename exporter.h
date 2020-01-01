#ifndef __PROMETHEUS_MQTTBROKER_STATS_EXPORTER_EXPORTER_H__
#define __PROMETHEUS_MQTTBROKER_STATS_EXPORTER_EXPORTER_H__

#include <pthread.h>
extern pthread_mutex_t mqtt_mutex;

#include <stdarg.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h> // Note: Not required on Linux, only added for compatibility

/* POSIX.1-2001 */
#include <sys/select.h>

#define MHD_PLATFORM_H
#include <microhttpd.h>

int handle_http_request(void *, struct MHD_Connection *, const char*, const char*, const char*, const char*, size_t *, void **);

#define BYTES_RECEIVED_STR "# TYPE mqtt_broker_bytes_received_total counter\nmqtt_broker_bytes_received_total{broker=\"%s\"} %s\n# HELP mqtt_broker_bytes_received_total total number of bytes received since the broker started\n"
#define BYTES_RECEIVED_LEN 189

#define BYTES_SENT_STR "# TYPE mqtt_broker_bytes_sent_total counter\nmqtt_broker_bytes_sent_total{broker=\"%s\"} %s\n# HELP mqtt_broker_bytes_sent_total total number of bytes sent since the broker started\n"
#define BYTES_SENT_LEN 173

#define CLIENTS_EXPIRED_STR "# TYPE mqtt_broker_clients_expired gauge\nmqtt_broker_clients_expired{broker=\"%s\"} %s\n# HELP mqtt_broker_clients_expired number of disconnected persistent clients that have been expired and removed through the persistent_client_expiration option\n"
#define CLIENTS_EXPIRED_LEN 241

#define CLIENTS_MAXIMUM_STR "# TYPE mqtt_broker_clients_maximum gauge\nmqtt_broker_clients_maximum{broker=\"%s\"} %s\n# HELP mqtt_broker_clients_maximum maximum number of clients that have been connected to the broker at the same time\n"
#define CLIENTS_MAXIMUM_LEN 198

#define CLIENTS_TOTAL_STR "# TYPE mqtt_broker_total_clients gauge\nmqtt_broker_total_clients{broker=\"%s\"} %s\n# HELP mqtt_broker_total_clients total number of connected and disconnected clients with a persistent session currently connected and registered on the broker\n"
#define CLIENTS_TOTAL_LEN 236

#define CLIENTS_CONNECTED_STR "# TYPE mqtt_broker_clients_connected gauge\nmqtt_broker_clients_connected{broker=\"%s\"} %s\n# HELP mqtt_broker_clients_connected number of connected clients\n"
#define CLIENTS_CONNECTED_LEN 150

#define CLIENTS_ACTIVE_STR "# TYPE mqtt_broker_clients_active gauge\nmqtt_broker_clients_active{broker=\"%s\"} %s\n# HELP mqtt_broker_clients_active number of active clients\n"
#define CLIENTS_ACTIVE_LEN 138

#define CLIENTS_DISCONNECTED_STR "# TYPE mqtt_broker_clients_disconnected gauge\nmqtt_broker_clients_disconnected{broker=\"%s\"} %s\n# HELP mqtt_broker_clients_disconnected total number of persistent clients (with clean session disabled) that are registered at the broker but are currently disconnected\n"
#define CLIENTS_DISCONNECTED_LEN 261

#define CLIENTS_INACTIVE_STR "# TYPE mqtt_broker_clients_inactive gauge\nmqtt_broker_clients_inactive{broker=\"%s\"} %s\n# HELP mqtt_broker_clients_inactive total number of persistent clients (with clean session disabled) that are registered at the broker but are currently disconnected\n"
#define CLIENTS_INACTIVE_LEN 249

#define HEAP_CURRENT_STR "# TYPE mqtt_broker_heap_current gauge\nmqtt_broker_heap_current{broker=\"%s\"} %s\n# HELP mqtt_broker_heap_current size of the heap memory in use by mosquitto\n"
#define HEAP_CURRENT_LEN 151

#define HEAP_MAXIMUM_STR "# TYPE mqtt_broker_heap_maximum gauge\nmqtt_broker_heap_maximum{broker=\"%s\"} %s\n# HELP mqtt_broker_heap_maximum size of the heap memory in use by mosquitto\n"
#define HEAP_MAXIMUM_LEN 151

#define CONNECTIONS_1MIN_STR "# TYPE mqtt_broker_connections_1min gauge\nmqtt_broker_connections_1min{broker=\"%s\"} %s\n# HELP mqtt_broker_connections_1min moving average of the number of CONNECT packets received by the broker over different time intervals\n"
#define CONNECTIONS_1MIN_LEN 220

#define CONNECTIONS_5MIN_STR "# TYPE mqtt_broker_connections_5min gauge\nmqtt_broker_connections_5min{broker=\"%s\"} %s\n# HELP mqtt_broker_connections_5min moving average of the number of CONNECT packets received by the broker over different time intervals\n"
#define CONNECTIONS_5MIN_LEN 220

#define CONNECTIONS_15MIN_STR "# TYPE mqtt_broker_connections_15min gauge\nmqtt_broker_connections_15min{broker=\"%s\"} %s\n# HELP mqtt_broker_connections_15min moving average of the number of CONNECT packets received by the broker over different time intervals\n"
#define CONNECTIONS_15MIN_LEN 223

#define BYTES_RECEIVED_1MIN_STR "# TYPE mqtt_broker_bytes_received_1min gauge\nmqtt_broker_bytes_received_1min{broker=\"%s\"} %s\n# HELP mqtt_broker_bytes_received_1min moving average of the number of bytes received by the broker over different time intervals\n"
#define BYTES_RECEIVED_1MIN_LEN 219

#define BYTES_RECEIVED_5MIN_STR "# TYPE mqtt_broker_bytes_received_5min gauge\nmqtt_broker_bytes_received_5min{broker=\"%s\"} %s\n# HELP mqtt_broker_bytes_received_5min moving average of the number of bytes received by the broker over different time intervals\n"
#define BYTES_RECEIVED_5MIN_LEN 219

#define BYTES_RECEIVED_15MIN_STR "# TYPE mqtt_broker_bytes_received_15min gauge\nmqtt_broker_bytes_received_15min{broker=\"%s\"} %s\n# HELP mqtt_broker_bytes_received_15min moving average of the number of bytes received by the broker over different time intervals\n"
#define BYTES_RECEIVED_15MIN_LEN 222

#define BYTES_SENT_1MIN_STR "# TYPE mqtt_broker_bytes_sent_1min gauge\nmqtt_broker_bytes_sent_1min{broker=\"%s\"} %s\n# HELP mqtt_broker_bytes_sent_1min moving average of the number of bytes sent by the broker over different time intervals\n"
#define BYTES_SENT_1MIN_LEN 203

#define BYTES_SENT_5MIN_STR "# TYPE mqtt_broker_bytes_sent_5min gauge\nmqtt_broker_bytes_sent_5min{broker=\"%s\"} %s\n# HELP mqtt_broker_bytes_sent_5min moving average of the number of bytes sent by the broker over different time intervals\n"
#define BYTES_SENT_5MIN_LEN 203

#define BYTES_SENT_15MIN_STR "# TYPE mqtt_broker_bytes_sent_15min gauge\nmqtt_broker_bytes_sent_15min{broker=\"%s\"} %s\n# HELP mqtt_broker_bytes_sent_15min moving average of the number of bytes sent by the broker over different time intervals\n"
#define BYTES_SENT_15MIN_LEN 206

#define MESSAGES_RECEIVED_1MIN_STR "# TYPE mqtt_broker_messages_received_1min gauge\nmqtt_broker_messages_received_1min{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_received_1min moving average of the number of all types of MQTT messages received by the broker over different time intervals\n"
#define MESSAGES_RECEIVED_1MIN_LEN 249

#define MESSAGES_RECEIVED_5MIN_STR "# TYPE mqtt_broker_messages_received_5min gauge\nmqtt_broker_messages_received_5min{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_received_5min moving average of the number of all types of MQTT messages received by the broker over different time intervals\n"
#define MESSAGES_RECEIVED_5MIN_LEN 249

#define MESSAGES_RECEIVED_15MIN_STR "# TYPE mqtt_broker_messages_received_15min gauge\nmqtt_broker_messages_received_15min{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_received_15min moving average of the number of all types of MQTT messages received by the broker over different time intervals\n"
#define MESSAGES_RECEIVED_15MIN_LEN 252

#define MESSAGES_SENT_1MIN_STR "# TYPE mqtt_broker_messages_sent_1min gauge\nmqtt_broker_messages_sent_1min{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_sent_1min moving average of the number of all types of MQTT messages sent by the broker over different time intervals\n"
#define MESSAGES_SENT_1MIN_LEN 233

#define MESSAGES_SENT_5MIN_STR "# TYPE mqtt_broker_messages_sent_5min gauge\nmqtt_broker_messages_sent_5min{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_sent_5min moving average of the number of all types of MQTT messages sent by the broker over different time intervals\n"
#define MESSAGES_SENT_5MIN_LEN 233

#define MESSAGES_SENT_15MIN_STR "# TYPE mqtt_broker_messages_sent_15min gauge\nmqtt_broker_messages_sent_15min{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_sent_15min moving average of the number of all types of MQTT messages sent by the broker over different time intervals\n"
#define MESSAGES_SENT_15MIN_LEN 236

#define PUBLISH_DROPPED_1MIN_STR "# TYPE mqtt_broker_publish_dropped_1min gauge\nmqtt_broker_publish_dropped_1min{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_dropped_1min moving average of the number of publish messages dropped by the broker over different time intervals\n"
#define PUBLISH_DROPPED_1MIN_LEN 232

#define PUBLISH_DROPPED_5MIN_STR "# TYPE mqtt_broker_publish_dropped_5min gauge\nmqtt_broker_publish_dropped_5min{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_dropped_5min moving average of the number of publish messages dropped by the broker over different time intervals\n"
#define PUBLISH_DROPPED_5MIN_LEN 232

#define PUBLISH_DROPPED_15MIN_STR "# TYPE mqtt_broker_publish_dropped_15min gauge\nmqtt_broker_publish_dropped_15min{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_dropped_15min moving average of the number of publish messages dropped by the broker over different time intervals\n"
#define PUBLISH_DROPPED_15MIN_LEN 235

#define PUBLISH_RECEIVED_1MIN_STR "# TYPE mqtt_broker_publish_received_1min gauge\nmqtt_broker_publish_received_1min{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_received_1min moving average of the number of publish messages received by the broker over different time intervals\n"
#define PUBLISH_RECEIVED_1MIN_LEN 236

#define PUBLISH_RECEIVED_5MIN_STR "# TYPE mqtt_broker_publish_received_5min gauge\nmqtt_broker_publish_received_5min{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_received_5min moving average of the number of publish messages received by the broker over different time intervals\n"
#define PUBLISH_RECEIVED_5MIN_LEN 236

#define PUBLISH_RECEIVED_15MIN_STR "# TYPE mqtt_broker_publish_received_15min gauge\nmqtt_broker_publish_received_15min{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_received_15min moving average of the number of publish messages received by the broker over different time intervals\n"
#define PUBLISH_RECEIVED_15MIN_LEN 239

#define PUBLISH_SENT_1MIN_STR "# TYPE mqtt_broker_publish_sent_1min gauge\nmqtt_broker_publish_sent_1min{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_sent_1min moving average of the number of publish messages sent by the broker over different time intervals\n"
#define PUBLISH_SENT_1MIN_LEN 220

#define PUBLISH_SENT_5MIN_STR "# TYPE mqtt_broker_publish_sent_5min gauge\nmqtt_broker_publish_sent_5min{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_sent_5min moving average of the number of publish messages sent by the broker over different time intervals\n"
#define PUBLISH_SENT_5MIN_LEN 220

#define PUBLISH_SENT_15MIN_STR "# TYPE mqtt_broker_publish_sent_15min gauge\nmqtt_broker_publish_sent_15min{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_sent_15min moving average of the number of publish messages sent by the broker over different time intervals\n"
#define PUBLISH_SENT_15MIN_LEN 223

#define SOCKETS_1MIN_STR "# TYPE mqtt_broker_sockets_1min gauge\nmqtt_broker_sockets_1min{broker=\"%s\"} %s\n# HELP mqtt_broker_sockets_1min moving average of the number of socket connections opened to the broker over different time intervals\n"
#define SOCKETS_1MIN_LEN 209

#define SOCKETS_5MIN_STR "# TYPE mqtt_broker_sockets_5min gauge\nmqtt_broker_sockets_5min{broker=\"%s\"} %s\n# HELP mqtt_broker_sockets_5min moving average of the number of socket connections opened to the broker over different time intervals\n"
#define SOCKETS_5MIN_LEN 209

#define SOCKETS_15MIN_STR "# TYPE mqtt_broker_sockets_15min gauge\nmqtt_broker_sockets_15min{broker=\"%s\"} %s\n# HELP mqtt_broker_sockets_15min moving average of the number of socket connections opened to the broker over different time intervals\n"
#define SOCKETS_15MIN_LEN 212

#define MESSAGES_INFLIGHT_STR "# TYPE mqtt_broker_messages_inflight gauge\nmqtt_broker_messages_inflight{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_inflight number of messages with QoS>0 that are awaiting acknowledgments\n"
#define MESSAGES_INFLIGHT_LEN 186

#define MESSAGES_RECEIVED_STR "# TYPE mqtt_broker_messages_received_total counter\nmqtt_broker_messages_received_total{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_received_total total number of messages of any type received since the broker started\n"
#define MESSAGES_RECEIVED_LEN 213

#define MESSAGES_SENT_STR "# TYPE mqtt_broker_messages_sent_total counter\nmqtt_broker_messages_sent_total{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_sent_total total number of messages of any type sent since the broker started\n"
#define MESSAGES_SENT_LEN 197

#define MESSAGES_STORED_STR "# TYPE mqtt_broker_messages_stored gauge\nmqtt_broker_messages_stored{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_stored number of messages currently held in the message store\n"
#define MESSAGES_STORED_LEN 171

#define MESSAGES_STORED_BYTES_STR "# TYPE mqtt_broker_messages_stored_bytes gauge\nmqtt_broker_messages_stored_bytes{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_stored_bytes number of bytes currently held in the message store\n"
#define MESSAGES_STORED_BYTES_LEN 186

#define PUBLISH_MESSAGES_DROPPED_STR "# TYPE mqtt_broker_publish_messages_dropped_total counter\nmqtt_broker_publish_messages_dropped_total{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_messages_dropped_total total number of publish messages that have been dropped due to inflight/queuing limits\n"
#define PUBLISH_MESSAGES_DROPPED_LEN 250

#define PUBLISH_MESSAGES_RECEIVED_STR "# TYPE mqtt_broker_publish_messages_received_total counter\nmqtt_broker_publish_messages_received_total{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_messages_received_total total number of PUBLISH messages received since the broker started\n"
#define PUBLISH_MESSAGES_RECEIVED_LEN 233

#define PUBLISH_MESSAGES_SENT_STR "# TYPE mqtt_broker_publish_messages_sent_total counter\nmqtt_broker_publish_messages_sent_total{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_messages_sent_total total number of PUBLISH messages sent since the broker started\n"
#define PUBLISH_MESSAGES_SENT_LEN 217

#define PUBLISH_BYTES_RECEIVED_STR "# TYPE mqtt_broker_publish_bytes_received_total counter\nmqtt_broker_publish_bytes_received_total{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_bytes_received_total total number of PUBLISH received since the broker started\n"
#define PUBLISH_BYTES_RECEIVED_LEN 215

#define PUBLISH_BYTES_SENT_STR "# TYPE mqtt_broker_publish_bytes_sent_total counter\nmqtt_broker_publish_bytes_sent_total{broker=\"%s\"} %s\n# HELP mqtt_broker_publish_bytes_sent_total total number of PUBLISH sent since the broker started\n"
#define PUBLISH_BYTES_SENT_LEN 199

#define MESSAGES_RETAINED_STR "# TYPE mqtt_broker_messages_retained gauge\nmqtt_broker_messages_retained{broker=\"%s\"} %s\n# HELP mqtt_broker_messages_retained number of retained messages active on the broker\n"
#define MESSAGES_RETAINED_LEN 171

#define SUBSCRIPTIONS_STR "# TYPE mqtt_broker_subscriptions gauge\nmqtt_broker_subscriptions{broker=\"%s\"} %s\n# HELP mqtt_broker_subscriptions number of subscriptions active on the broker\n"
#define SUBSCRIPTIONS_LEN 155

#endif /* __PROMETHEUS_MQTTBROKER_STATS_EXPORTER_EXPORTER_H__ */

