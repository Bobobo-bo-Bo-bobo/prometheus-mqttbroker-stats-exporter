#include "prometheus-mqttbroker-stats-exporter.h"
#include "exporter.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>

const char *slash_html = "<html>\n<head>\n<title>MQTT broker statistic exporter</title>\n<head>\n<body>\n<h1>MQTT broker statistic exporter</h1>\n<p>\n<a href=\"/metrics\" alt=\"Metrics\">Metrics</a>\n</p>\n</body>\n</html>\n";
const char *http_404 = "404 - Not found\n";
const char *http_405 = "405 - Method not allowed\n";
const char *http_500 = "500 - Internal Server Error\n";

char *_build_bytes_received(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = BYTES_RECEIVED_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->bytes_received) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, BYTES_RECEIVED_STR, cfg->mqtt_host, cfg->broker_stats->bytes_received);
    }
    return result;
}

char *_build_bytes_sent(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = BYTES_SENT_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->bytes_sent) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, BYTES_SENT_STR, cfg->mqtt_host, cfg->broker_stats->bytes_sent);
    }
    return result;
}

char *_build_clients_expired(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = CLIENTS_EXPIRED_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->clients_expired) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, CLIENTS_EXPIRED_STR, cfg->mqtt_host, cfg->broker_stats->clients_expired);
    }
    return result;
}

char *_build_clients_maximum(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = CLIENTS_MAXIMUM_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->clients_maximum) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, CLIENTS_MAXIMUM_STR, cfg->mqtt_host, cfg->broker_stats->clients_maximum);
    }
    return result;
}

char *_build_clients_total(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = CLIENTS_TOTAL_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->clients_total) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, CLIENTS_TOTAL_STR, cfg->mqtt_host, cfg->broker_stats->clients_total);
    }
    return result;
}

char *_build_clients_connected(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = CLIENTS_CONNECTED_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->clients_connected) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, CLIENTS_CONNECTED_STR, cfg->mqtt_host, cfg->broker_stats->clients_connected);
    }
    return result;
}

char *_build_clients_active(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = CLIENTS_ACTIVE_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->clients_active) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, CLIENTS_ACTIVE_STR, cfg->mqtt_host, cfg->broker_stats->clients_active);
    }
    return result;
}

char *_build_clients_disconnected(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = CLIENTS_DISCONNECTED_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->clients_disconnected) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, CLIENTS_DISCONNECTED_STR, cfg->mqtt_host, cfg->broker_stats->clients_disconnected);
    }
    return result;
}

char *_build_clients_inactive(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = CLIENTS_INACTIVE_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->clients_inactive) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, CLIENTS_INACTIVE_STR, cfg->mqtt_host, cfg->broker_stats->clients_inactive);
    }
    return result;
}

char *_build_heap_current(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = HEAP_CURRENT_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->heap_current) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, HEAP_CURRENT_STR, cfg->mqtt_host, cfg->broker_stats->heap_current);
    }
    return result;
}

char *_build_heap_maximum(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = HEAP_MAXIMUM_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->heap_maximum) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, HEAP_MAXIMUM_STR, cfg->mqtt_host, cfg->broker_stats->heap_maximum);
    }
    return result;
}

char *_build_connections_1min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = CONNECTIONS_1MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->connections_1min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, CONNECTIONS_1MIN_STR, cfg->mqtt_host, cfg->broker_stats->connections_1min);
    }
    return result;
}

char *_build_connections_5min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = CONNECTIONS_5MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->connections_5min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, CONNECTIONS_5MIN_STR, cfg->mqtt_host, cfg->broker_stats->connections_5min);
    }
    return result;
}

char *_build_connections_15min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = CONNECTIONS_15MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->connections_15min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, CONNECTIONS_15MIN_STR, cfg->mqtt_host, cfg->broker_stats->connections_15min);
    }
    return result;
}

char *_build_bytes_received_1min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = BYTES_RECEIVED_1MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->bytes_received_1min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, BYTES_RECEIVED_1MIN_STR, cfg->mqtt_host, cfg->broker_stats->bytes_received_1min);
    }
    return result;
}

char *_build_bytes_received_15min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = BYTES_RECEIVED_15MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->bytes_received_15min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, BYTES_RECEIVED_15MIN_STR, cfg->mqtt_host, cfg->broker_stats->bytes_received_15min);
    }
    return result;
}

char *_build_bytes_received_5min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = BYTES_RECEIVED_5MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->bytes_received_5min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, BYTES_RECEIVED_5MIN_STR, cfg->mqtt_host, cfg->broker_stats->bytes_received_5min);
    }
    return result;
}

char *_build_bytes_sent_1min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = BYTES_SENT_1MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->bytes_sent_1min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, BYTES_SENT_1MIN_STR, cfg->mqtt_host, cfg->broker_stats->bytes_sent_1min);
    }
    return result;
}

char *_build_bytes_sent_5min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = BYTES_SENT_5MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->bytes_sent_5min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, BYTES_SENT_5MIN_STR, cfg->mqtt_host, cfg->broker_stats->bytes_sent_5min);
    }
    return result;
}

char *_build_bytes_sent_15min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = BYTES_SENT_15MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->bytes_sent_15min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, BYTES_SENT_15MIN_STR, cfg->mqtt_host, cfg->broker_stats->bytes_sent_15min);
    }
    return result;
}

char *_build_messages_received_1min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_RECEIVED_1MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_received_1min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_RECEIVED_1MIN_STR, cfg->mqtt_host, cfg->broker_stats->messages_received_1min);
    }
    return result;
}

char *_build_messages_received_5min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_RECEIVED_5MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_received_5min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_RECEIVED_5MIN_STR, cfg->mqtt_host, cfg->broker_stats->messages_received_5min);
    }
    return result;
}

char *_build_messages_received_15min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_RECEIVED_15MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_received_15min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_RECEIVED_15MIN_STR, cfg->mqtt_host, cfg->broker_stats->messages_received_15min);
    }
    return result;
}

char *_build_messages_sent_1min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_SENT_1MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_sent_1min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_SENT_1MIN_STR, cfg->mqtt_host, cfg->broker_stats->messages_sent_1min);
    }
    return result;
}

char *_build_messages_sent_5min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_SENT_5MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_sent_5min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_SENT_5MIN_STR, cfg->mqtt_host, cfg->broker_stats->messages_sent_5min);
    }
    return result;
}

char *_build_messages_sent_15min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_SENT_15MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_sent_15min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_SENT_15MIN_STR, cfg->mqtt_host, cfg->broker_stats->messages_sent_15min);
    }
    return result;
}

char *_build_publish_dropped_1min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_DROPPED_1MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_dropped_1min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_DROPPED_1MIN_STR, cfg->mqtt_host, cfg->broker_stats->publish_dropped_1min);
    }
    return result;
}

char *_build_publish_dropped_5min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_DROPPED_5MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_dropped_5min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_DROPPED_5MIN_STR, cfg->mqtt_host, cfg->broker_stats->publish_dropped_5min);
    }
    return result;
}

char *_build_publish_dropped_15min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_DROPPED_15MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_dropped_15min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_DROPPED_15MIN_STR, cfg->mqtt_host, cfg->broker_stats->publish_dropped_15min);
    }
    return result;
}

char *_build_publish_received_1min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_RECEIVED_1MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_received_1min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_RECEIVED_1MIN_STR, cfg->mqtt_host, cfg->broker_stats->publish_received_1min);
    }
    return result;
}

char *_build_publish_received_5min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_RECEIVED_5MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_received_5min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_RECEIVED_5MIN_STR, cfg->mqtt_host, cfg->broker_stats->publish_received_5min);
    }
    return result;
}

char *_build_publish_received_15min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_RECEIVED_15MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_received_15min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_RECEIVED_15MIN_STR, cfg->mqtt_host, cfg->broker_stats->publish_received_15min);
    }
    return result;
}

char *_build_publish_sent_1min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_SENT_1MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_sent_1min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_SENT_1MIN_STR, cfg->mqtt_host, cfg->broker_stats->publish_sent_1min);
    }
    return result;
}

char *_build_publish_sent_5min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_SENT_5MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_sent_5min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_SENT_5MIN_STR, cfg->mqtt_host, cfg->broker_stats->publish_sent_5min);
    }
    return result;
}

char *_build_publish_sent_15min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_SENT_15MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_sent_15min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_SENT_15MIN_STR, cfg->mqtt_host, cfg->broker_stats->publish_sent_15min);
    }
    return result;
}

char *_build_sockets_1min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = SOCKETS_1MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->sockets_1min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, SOCKETS_1MIN_STR, cfg->mqtt_host, cfg->broker_stats->sockets_1min);
    }
    return result;
}

char *_build_sockets_5min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = SOCKETS_5MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->sockets_5min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, SOCKETS_5MIN_STR, cfg->mqtt_host, cfg->broker_stats->sockets_5min);
    }
    return result;
}

char *_build_sockets_15min(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = SOCKETS_15MIN_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->sockets_15min) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, SOCKETS_15MIN_STR, cfg->mqtt_host, cfg->broker_stats->sockets_15min);
    }
    return result;
}

char *_build_messages_inflight(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_INFLIGHT_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_inflight) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_INFLIGHT_STR, cfg->mqtt_host, cfg->broker_stats->messages_inflight);
    }
    return result;
}

char *_build_messages_received(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_RECEIVED_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_received) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_RECEIVED_STR, cfg->mqtt_host, cfg->broker_stats->messages_received);
    }
    return result;
}

char *_build_messages_sent(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_SENT_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_sent) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_SENT_STR, cfg->mqtt_host, cfg->broker_stats->messages_sent);
    }
    return result;
}

char *_build_messages_stored(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_STORED_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_stored) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_STORED_STR, cfg->mqtt_host, cfg->broker_stats->messages_stored);
    }
    return result;
}

char *_build_publish_messages_dropped(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_MESSAGES_DROPPED_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_messages_dropped) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_MESSAGES_DROPPED_STR, cfg->mqtt_host, cfg->broker_stats->publish_messages_dropped);
    }
    return result;
}

char *_build_publish_messages_received(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_MESSAGES_RECEIVED_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_messages_received) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_MESSAGES_RECEIVED_STR, cfg->mqtt_host, cfg->broker_stats->publish_messages_received);
    }
    return result;
}

char *_build_publish_messages_sent(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_MESSAGES_SENT_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_messages_sent) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_MESSAGES_SENT_STR, cfg->mqtt_host, cfg->broker_stats->publish_messages_sent);
    }
    return result;
}

char *_build_publish_bytes_received(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_BYTES_RECEIVED_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_bytes_received) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_BYTES_RECEIVED_STR, cfg->mqtt_host, cfg->broker_stats->publish_bytes_received);
    }
    return result;
}

char *_build_publish_bytes_sent(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = PUBLISH_BYTES_SENT_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->publish_bytes_sent) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, PUBLISH_BYTES_SENT_STR, cfg->mqtt_host, cfg->broker_stats->publish_bytes_sent);
    }
    return result;
}

char *_build_messages_retained(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = MESSAGES_RETAINED_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->messages_retained) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, MESSAGES_RETAINED_STR, cfg->mqtt_host, cfg->broker_stats->messages_retained);
    }
    return result;
}

char *_build_subscriptions(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = SUBSCRIPTIONS_LEN + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->subscriptions) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, SUBSCRIPTIONS_STR, cfg->mqtt_host, cfg->broker_stats->subscriptions);
    }
    return result;
}

// Append src metrics data to dst, src will be free()ed if freesrc is true
char *_append_metric_data(char *dst, char *src, bool free_src) {
    size_t src_len;
    size_t dst_len;
    char *result;
    
    if (!src) {
        return dst;
    }

    src_len = strlen(src);
    if (dst) {
        dst_len = strlen(dst);
    } else {
        dst_len = 0;
    }

    if (!src_len) {
        if (free_src) {
            free(src);
        }
    }

    result = realloc(dst, dst_len + src_len + 1); // +1 for terminating 0
    if (!result) {
        fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory for metric update\n",   dst_len + src_len + 1);
        if (free_src) {
            free(src);
        }
        return dst;
    }

    memcpy(result+dst_len, src, src_len);
    // force terminating 0
    result[dst_len + src_len] = 0;

    if (free_src) {
        free(src);
    }
    return result;
}

char *_build_metrics_string(struct configuration *cfg) {
    char *result = NULL;
    size_t result_len = 0;
    size_t len;
    char *line;

    if (cfg->broker_stats->bytes_received) {
        line = _build_bytes_received(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->bytes_sent) {
        line = _build_bytes_sent(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->clients_expired) {
        line = _build_clients_expired(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->clients_maximum) {
        line = _build_clients_maximum(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->clients_total) {
        line = _build_clients_total(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->clients_connected) {
        line = _build_clients_connected(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->clients_active) {
        line = _build_clients_active(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->clients_disconnected) {
        line = _build_clients_disconnected(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->clients_inactive) {
        line = _build_clients_inactive(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->heap_current) {
        line = _build_heap_current(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->heap_maximum) {
        line = _build_heap_maximum(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->connections_1min) {
        line = _build_connections_1min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->connections_5min) {
        line = _build_connections_5min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->connections_15min) {
        line = _build_connections_15min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->bytes_received_1min) {
        line = _build_bytes_received_1min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->bytes_received_15min) {
        line = _build_bytes_received_15min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->bytes_received_5min) {
        line = _build_bytes_received_5min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->bytes_sent_1min) {
        line = _build_bytes_sent_1min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->bytes_sent_5min) {
        line = _build_bytes_sent_5min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->bytes_sent_15min) {
        line = _build_bytes_sent_15min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_received_1min) {
        line = _build_messages_received_1min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_received_5min) {
        line = _build_messages_received_5min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_received_15min) {
        line = _build_messages_received_15min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_sent_1min) {
        line = _build_messages_sent_1min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_sent_5min) {
        line = _build_messages_sent_5min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_sent_15min) {
        line = _build_messages_sent_15min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_dropped_1min) {
        line = _build_publish_dropped_1min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_dropped_5min) {
        line = _build_publish_dropped_5min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_dropped_15min) {
        line = _build_publish_dropped_15min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_received_1min) {
        line = _build_publish_received_1min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_received_5min) {
        line = _build_publish_received_5min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_received_15min) {
        line = _build_publish_received_15min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_sent_1min) {
        line = _build_publish_sent_1min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_sent_5min) {
        line = _build_publish_sent_5min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_sent_15min) {
        line = _build_publish_sent_15min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->sockets_1min) {
        line = _build_sockets_1min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->sockets_5min) {
        line = _build_sockets_5min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->sockets_15min) {
        line = _build_sockets_15min(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_inflight) {
        line = _build_messages_inflight(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_received) {
        line = _build_messages_received(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_sent) {
        line = _build_messages_sent(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_stored) {
        line = _build_messages_stored(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_messages_dropped) {
        line = _build_publish_messages_dropped(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_messages_received) {
        line = _build_publish_messages_received(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_messages_sent) {
        line = _build_publish_messages_sent(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_bytes_received) {
        line = _build_publish_bytes_received(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->publish_bytes_sent) {
        line = _build_publish_bytes_sent(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->messages_retained) {
        line = _build_messages_retained(cfg);
        result = _append_metric_data(result, line, true);
    }

    if (cfg->broker_stats->subscriptions) {
        line = _build_subscriptions(cfg);
        result = _append_metric_data(result, line, true);
    }

    return result;
}

int handle_http_request(void *cls, struct MHD_Connection *con, const char *url, const char *method,
                        const char *version, const char *up_data, size_t *up_size, void **con_cls) {
    struct MHD_Response *reply = NULL;
    int rc = 0;
    unsigned int http_status = 500;
    char *metric =NULL;

    if (config->debug) {
        fprintf(stderr, "DEBUG: %s %s\n", method, url);
    }

    if (!strcmp(method, "GET")) {
	if (!strcmp(url, "/")) {
            // MHD_RESPMEM_PERSISTENT - Buffer is a persistent (static/global) buffer that won’t change for at least
            // the lifetime of the response, MHD should just use it, not free it, not copy it, just keep an alias to it.
            reply = MHD_create_response_from_buffer(strlen(slash_html), (void *) slash_html, MHD_RESPMEM_PERSISTENT);
            if (!reply) {
                fprintf(stderr, "ERROR: handle_http_request: Unable to generate response structure for /");
                return MHD_NO;
            }
            MHD_add_response_header(reply, "Content-Type", "text/html");

            http_status = 200;
            metric = NULL;
        } else if (!strcmp(url, "/metrics")) {
            pthread_mutex_lock(&mqtt_mutex);
            metric = _build_metrics_string(config);
            pthread_mutex_unlock(&mqtt_mutex);

            if (!metric) {
                // MHD_RESPMEM_PERSISTENT - Buffer is a persistent (static/global) buffer that won’t change for at least
                // the lifetime of the response, MHD should just use it, not free it, not copy it, just keep an alias to it.
                reply = MHD_create_response_from_buffer(strlen(http_500), (void *) http_500, MHD_RESPMEM_PERSISTENT);
                if (!reply) {
                    fprintf(stderr, "ERROR: handle_http_request: Unable to generate response structure for HTTP 500");
                    return MHD_NO;
                }
                MHD_add_response_header(reply, "Content-Type", "text/plain");
                // we only allow GET
                http_status = 500;
            } else {
                reply = MHD_create_response_from_buffer(strlen(metric), (void *)metric, MHD_RESPMEM_MUST_COPY);
                MHD_add_response_header(reply, "Content-Type", "text/plain");
                http_status = 200;
            }
        } else {
            // MHD_RESPMEM_PERSISTENT - Buffer is a persistent (static/global) buffer that won’t change for at least
            // the lifetime of the response, MHD should just use it, not free it, not copy it, just keep an alias to it.
            reply = MHD_create_response_from_buffer(strlen(http_404), (void *) http_404, MHD_RESPMEM_PERSISTENT);
            if (!reply) {
                fprintf(stderr, "ERROR: handle_http_request: Unable to generate response structure for HTTP 404");
                return MHD_NO;
            }
            MHD_add_response_header(reply, "Content-Type", "text/plain");
            // we only allow GET
            http_status = 404;
	}
    } else {
        // MHD_RESPMEM_PERSISTENT - Buffer is a persistent (static/global) buffer that won’t change for at least
        // the lifetime of the response, MHD should just use it, not free it, not copy it, just keep an alias to it.
        reply = MHD_create_response_from_buffer(strlen(http_405), (void *) http_405, MHD_RESPMEM_PERSISTENT);
        if (!reply) {
            fprintf(stderr, "ERROR: handle_http_request: Unable to generate response structure for HTTP 405");
            return MHD_NO;
        }
        MHD_add_response_header(reply, "Content-Type", "text/plain");
        // we only allow GET
        http_status = 405;
    }

    rc = MHD_queue_response(con, http_status, reply);
    MHD_destroy_response(reply);

    free(metric);
    return rc;
}
