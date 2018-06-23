#include "prometheus-mqttbroker-stats-exporter.h"
#include "exporter.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>

const char *slash_html = "<html>\n<head>\n<title>MQTT broker statistic exporter</title>\n<head>\n<body>\n<h1>MQTT broker statistic exporter</h1>\n<p>\n<a href=\"/metrics\" alt=\"Metrics\">Metrics</a>\n</p>\n</body>\n</html>\n";
const char *http_404 = "405 - Not found\n";
const char *http_405 = "405 - Method not allowed\n";
const char *http_500 = "500 - Internal Server Error\n";

char *_build_bytes_received(const struct configuration *cfg) {
    char *result;
    size_t size;

    size = 171 + strlen(cfg->mqtt_host) + strlen(cfg->broker_stats->bytes_received) + 1;
    result = malloc(size);
    if (result) {
        snprintf(result, size, "# TYPE mqtt_broker_bytes_received counter\nmqtt_broker_bytes_received{broker=\"%s\"} %s\n# HELP mqtt_broker_bytes_received total number of bytes received since the broker started\n", cfg->mqtt_host, cfg->broker_stats->bytes_received);
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
        if (line) {
            len = strlen(line);
            result = realloc(result, result_len + len + 1); // make sure laste byte is 0
            if (!result) {
                fprintf(stderr, "ERROR: Memory allocation for metrics failed\n");
                abort();
            }
            memcpy(result+result_len, line, len);

            result_len += len + 1;
            result[result_len - 1] = 0;
            free(line);
        }
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

    if (strcmp(method, "GET") == 0) {
	// point it to /metrics if / is requested
	if (strcmp(url, "/") == 0) {
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
        } else if (strcmp(url, "/metrics") == 0) {
            // MHD_RESPMEM_PERSISTENT - Buffer is a persistent (static/global) buffer that won’t change for at least
            // the lifetime of the response, MHD should just use it, not free it, not copy it, just keep an alias to it.
            reply = MHD_create_response_from_buffer(strlen(slash_html), (void *) slash_html, MHD_RESPMEM_PERSISTENT);
            if (!reply) {
                fprintf(stderr, "ERROR: handle_http_request: Unable to generate response structure for /metrics");
                return MHD_NO;
            }

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
            metric = NULL;
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
        metric = NULL;
    }

    rc = MHD_queue_response(con, http_status, reply);
    MHD_destroy_response(reply);

    free(metric);
}
