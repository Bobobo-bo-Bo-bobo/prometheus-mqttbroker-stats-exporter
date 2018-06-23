#ifndef __PROMETHEUS_MQTTBROKER_STATS_EXPORTER_UTIL_H__
#define __PROMETHEUS_MQTTBROKER_STATS_EXPORTER_UTIL_H__

char *uuidgen(void);

#ifndef HAVE_MEMSET

#include <stddef.h>
void *memset(void *, int, size_t);

#else /* HAVE_MEMSET */

#include <string.h>

#endif /* HAVE_MEMSET */

#endif /* __PROMETHEUS_MQTTBROKER_STATS_EXPORTER_UTIL_H__ */

