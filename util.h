#ifndef __BADPHISH_MQTT_ZWAVE_UTIL_H__
#define __BADPHISH_MQTT_ZWAVE_UTIL_H__

char *uuidgen(void);

#ifndef HAVE_MEMSET

#include <stddef.h>
void *memset(void *, int, size_t);

#else /* HAVE_MEMSET */

#include <string.h>

#endif /* HAVE_MEMSET */

#endif /* __BADPHISH_MQTT_ZWAVE_UTIL_H__ */

