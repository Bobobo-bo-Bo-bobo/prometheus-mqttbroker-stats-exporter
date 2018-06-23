#ifndef __PROMETHEUS_MQTTBROKER_STATS_EXPORTER_CONNECT_H__
#define __PROMETHEUS_MQTTBROKER_STATS_EXPORTER_CONNECT_H__

#include "prometheus-mqttbroker-stats-exporter.h"
#include <mosquitto.h>

#define SSL_VERIFY_NONE 0
#define SSL_VERIFY_PEER 1

void *mqtt_connect(void *);

#endif /* __PROMETHEUS_MQTTBROKER_STATS_EXPORTER_CONNECT_H__ */

