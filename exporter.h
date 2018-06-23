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

#endif /* __PROMETHEUS_MQTTBROKER_STATS_EXPORTER_EXPORTER_H__ */

