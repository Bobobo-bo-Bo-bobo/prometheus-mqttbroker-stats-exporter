#include "prometheus-mqttbroker-stats-exporter.h"
#include "mqtt_connect.h"
#include "util.h"

#include <mosquitto.h>
#include <stdio.h>
#include <stdlib.h>

void _mqtt_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str) {
    /*
    if (str) {
        fprintf(stderr, "MQTT level %d: %s\n", level, str);
    }
    */
}

void _mqtt_connect_callback(struct mosquitto *mosq, void *userdata, int result) {
    int rc;
    struct configuration *cfg = (struct configuration *) userdata;

    switch(result) {
        case 0: {
                    printf("INFO: MQTT connection to %s port %d established\n", cfg->mqtt_host, cfg->mqtt_port);
                    break;
                }
        case 1: {
                    fprintf(stderr, "ERROR: MQTT connection refused to %s port %d - unacceptable protocol version\n", cfg->mqtt_host, cfg->mqtt_port);
                    // XXX: Error handling needs improvements!
                    exit(1);
                }
        case 2: {
                    fprintf(stderr, "ERROR: MQTT connection refused to %s port %d - identifier rejected\n", cfg->mqtt_host, cfg->mqtt_port);
                    // XXX: Error handling needs improvements!
                    exit(2);
                    break;
                }
        case 3: {
                    fprintf(stderr, "ERROR: MQTT connection refused to %s port %d - broker unavailable\n", cfg->mqtt_host, cfg->mqtt_port);
                    // XXX: Error handling needs improvements!
                    exit(3);
                    break;
                }
        default: {
                     fprintf(stderr, "ERROR: MQTT connection refused to %s port %d - unknown error code %d\n", cfg->mqtt_host, cfg->mqtt_port, result);
                     exit(result);
                 }
    }

    rc = mosquitto_subscribe(mosq, NULL, MQTT_SYS_TOPIC, cfg->mqtt_qos);
    switch(rc) {
        case MOSQ_ERR_SUCCESS: {
                                   printf("INFO: Subscribed to topic %s on %s port %d with QoS %d\n", MQTT_SYS_TOPIC, cfg->mqtt_host, cfg->mqtt_port, cfg->mqtt_qos);
                                   break;
                               }
        case MOSQ_ERR_INVAL: {
                                 fprintf(stderr, "ERROR: Unable to subscribe to topic %s on %s port %d with QoS %d - input parameters were invalid\n", MQTT_SYS_TOPIC, cfg->mqtt_host, cfg->mqtt_port, cfg->mqtt_qos);
                                 exit(MOSQ_ERR_INVAL);
                                 break;
                             }
        case MOSQ_ERR_NOMEM: {
                                 fprintf(stderr, "ERROR: Unable to subscribe to topic %s on %s port %d with QoS %d - out of memory condition occurred\n", MQTT_SYS_TOPIC, cfg->mqtt_host, cfg->mqtt_port, cfg->mqtt_qos);
                                 exit(MOSQ_ERR_NOMEM);
                                 break;
                             }
        case MOSQ_ERR_NO_CONN: {
                                 fprintf(stderr, "ERROR: Unable to subscribe to topic %s on %s port %d with QoS %d - client is not connected to a broker\n", MQTT_SYS_TOPIC, cfg->mqtt_host, cfg->mqtt_port, cfg->mqtt_qos);
                                 exit(MOSQ_ERR_NO_CONN);
                                 break;
                             }

// not all versions of libmosquitto define MOSQ_ERR_MALFORMED_UTF8
#ifdef MOSQ_ERR_MALFORMED_UTF8
        case MOSQ_ERR_MALFORMED_UTF8: {
                                 fprintf(stderr, "ERROR: Unable to subscribe to topic %s on %s port %d with QoS %d - topic is not valid UTF-8\n", MQTT_SYS_TOPIC, cfg->mqtt_host, cfg->mqtt_port, cfg->mqtt_qos);
                                 exit(MOSQ_ERR_MALFORMED_UTF8);
                                 break;
                             }
#endif

        default: {
                     fprintf(stderr, "ERROR: Unable to subscribe to topic %s on %s port %d with QoS %d - unknown error code %d\n", MQTT_SYS_TOPIC, cfg->mqtt_host, cfg->mqtt_port, cfg->mqtt_qos, rc);
                     exit(rc);
                 }
    }
};

void _mqtt_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos) {
    struct configuration *cfg = (struct configuration *) userdata;
    printf("INFO: Subscribed to topic %s on %s port %d with message id %d\n", MQTT_SYS_TOPIC, cfg->mqtt_host, cfg->mqtt_port, mid);
};

void _mqtt_disconnect_callback(struct mosquitto *mosq, void *obj, int rc) {
    struct configuration *cfg = (struct configuration *) obj;

    if (rc) {
        fprintf(stderr, "ERROR: Unexpected disconnect from %s port %d - code %d\n", cfg->mqtt_host, cfg->mqtt_port, rc);
        exit(rc); // XXX: Reconnect instead ?
    } else {
        printf("INFO: Disconnecting from %s port %d\n", cfg->mqtt_host, cfg->mqtt_port);
    };
};

char *_set_stat(char *ptr, char *data) {
    // release previous allocated memory
    // XXX: On Linux calling free with a NULL pointer will do nothing. What about other UNIX systems ?
    free(ptr);

    // we blindly assuming we have enough memory to copy the statistics payload
    return strdup(data);
}

void _update_broker_stats(struct configuration *cfg, const char *topic, char *data) {
    struct statistics *stats = cfg->broker_stats;

    if (!strcmp(topic, "$SYS/broker/clients/total")) {
        stats->clients_total = _set_stat(stats->clients_total, data);
    } else if (!strcmp(topic, "$SYS/broker/bytes/received")) {
        stats->bytes_received = _set_stat(stats->bytes_received, data);
    } else if (!strcmp(topic, "$SYS/broker/bytes/sent")) {
        stats->bytes_sent = _set_stat(stats->bytes_sent, data);
    } else if (!strcmp(topic, "$SYS/broker/clients/expired")) {
        stats->clients_expired = _set_stat(stats->clients_expired, data);
    } else if (!strcmp(topic, "$SYS/broker/clients/maximum")) {
        stats->clients_maximum = _set_stat(stats->clients_maximum, data);
    } else if (!strcmp(topic, "$SYS/broker/clients/connected")) {
        stats->clients_connected = _set_stat(stats->clients_connected, data);
    } else if (!strcmp(topic, "$SYS/broker/clients/active")) {
        stats->clients_active = _set_stat(stats->clients_active, data);
    } else if (!strcmp(topic, "$SYS/broker/heap/current")) {
        stats->heap_current = _set_stat(stats->heap_current, data);
    } else if (!strcmp(topic, "$SYS/broker/heap/maximum")) {
        stats->heap_maximum = _set_stat(stats->heap_maximum, data);
    } else if (!strcmp(topic, "$SYS/broker/clients/disconnected")) {
        stats->clients_disconnected = _set_stat(stats->clients_disconnected, data);
    } else if (!strcmp(topic, "$SYS/broker/clients/inactive")) {
        stats->clients_inactive = _set_stat(stats->clients_inactive, data);
    } else if (!strcmp(topic, "$SYS/broker/load/connections/1min")) {
        stats->connections_1min = _set_stat(stats->connections_1min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/connections/5min")) {
        stats->connections_5min = _set_stat(stats->connections_5min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/connections/15min")) {
        stats->connections_15min = _set_stat(stats->connections_15min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/bytes/received/1min")) {
        stats->bytes_received_1min = _set_stat(stats->bytes_received_1min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/bytes/received/5min")) {
        stats->bytes_received_5min = _set_stat(stats->bytes_received_5min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/bytes/received/15min")) {
        stats->bytes_received_15min = _set_stat(stats->bytes_received_15min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/bytes/sent/1min")) {
        stats->bytes_sent_1min = _set_stat(stats->bytes_sent_1min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/bytes/sent/5min")) {
        stats->bytes_sent_5min = _set_stat(stats->bytes_sent_5min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/bytes/sent/15min")) {
        stats->bytes_sent_15min = _set_stat(stats->bytes_sent_15min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/messages/received/1min")) {
        stats->messages_received_1min = _set_stat(stats->messages_received_1min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/messages/received/5min")) {
        stats->messages_received_5min = _set_stat(stats->messages_received_5min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/messages/received/15min")) {
        stats->messages_received_15min = _set_stat(stats->messages_received_15min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/messages/sent/1min")) {
        stats->messages_sent_1min = _set_stat(stats->messages_sent_1min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/messages/sent/5min")) {
        stats->messages_sent_5min = _set_stat(stats->messages_sent_5min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/messages/sent/15min")) {
        stats->messages_sent_15min = _set_stat(stats->messages_sent_15min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/publish/dropped/1min")) {
        stats->publish_dropped_1min = _set_stat(stats->publish_dropped_1min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/publish/dropped/5min")) {
        stats->publish_dropped_5min = _set_stat(stats->publish_dropped_5min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/publish/dropped/15min")) {
        stats->publish_dropped_15min = _set_stat(stats->publish_dropped_15min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/publish/received/1min")) {
        stats->publish_received_1min = _set_stat(stats->publish_received_1min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/publish/received/5min")) {
        stats->publish_received_5min = _set_stat(stats->publish_received_5min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/publish/received/15min")) {
        stats->publish_received_15min = _set_stat(stats->publish_received_15min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/publish/sent/1min")) {
        stats->publish_sent_1min = _set_stat(stats->publish_sent_1min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/publish/sent/5min")) {
        stats->publish_sent_5min = _set_stat(stats->publish_sent_5min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/publish/sent/15min")) {
        stats->publish_sent_15min = _set_stat(stats->publish_sent_15min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/sockets/1min")) {
        stats->sockets_1min = _set_stat(stats->sockets_1min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/sockets/5min")) {
        stats->sockets_5min = _set_stat(stats->sockets_5min, data);
    } else if (!strcmp(topic, "$SYS/broker/load/sockets/15min")) {
        stats->sockets_15min = _set_stat(stats->sockets_15min, data);
    } else if (!strcmp(topic, "$SYS/broker/messages/inflight")) {
        stats->messages_inflight = _set_stat(stats->messages_inflight, data);
    } else if (!strcmp(topic, "$SYS/broker/messages/received")) {
        stats->messages_received = _set_stat(stats->messages_received, data);
    } else if (!strcmp(topic, "$SYS/broker/messages/sent")) {
        stats->messages_sent = _set_stat(stats->messages_sent, data);
    // Note: Starting with Mosquitto 1.5 $SYS/broker/store/messages/count replaced $SYS/broker/messages/stored and $SYS/broker/store/messages/bytes was added
    } else if (!strcmp(topic, "$SYS/broker/messages/stored")) {
        stats->messages_stored = _set_stat(stats->messages_stored, data);
    // Note: Starting with Mosquitto 1.5 $SYS/broker/store/messages/count replaced $SYS/broker/messages/stored and $SYS/broker/store/messages/bytes was added
    } else if (!strcmp(topic, "$SYS/broker/store/messages/count")) {
        stats->messages_stored = _set_stat(stats->messages_stored, data);
    // Note: Starting with Mosquitto 1.5 $SYS/broker/store/messages/count replaced $SYS/broker/messages/stored and $SYS/broker/store/messages/bytes was added
    } else if (!strcmp(topic, "$SYS/broker/store/messages/bytes")) {
        stats->messages_stored_bytes = _set_stat(stats->messages_stored_bytes, data);
    } else if (!strcmp(topic, "$SYS/broker/publish/messages/dropped")) {
        stats->publish_messages_dropped = _set_stat(stats->publish_messages_dropped, data);
    } else if (!strcmp(topic, "$SYS/broker/publish/messages/received")) {
        stats->publish_messages_received = _set_stat(stats->publish_messages_received, data);
    } else if (!strcmp(topic, "$SYS/broker/publish/messages/sent")) {
        stats->publish_messages_sent = _set_stat(stats->publish_messages_sent, data);
    } else if (!strcmp(topic, "$SYS/broker/publish/bytes/received")) {
        stats->publish_bytes_received = _set_stat(stats->publish_bytes_received, data);
    } else if (!strcmp(topic, "$SYS/broker/publish/bytes/sent")) {
        stats->publish_bytes_sent = _set_stat(stats->publish_bytes_sent, data);
    } else if (!strcmp(topic, "$SYS/broker/retained messages/count")) {
        stats->messages_retained = _set_stat(stats->messages_retained, data);
    } else if (!strcmp(topic, "$SYS/broker/subscriptions/count")) {
        stats->subscriptions = _set_stat(stats->subscriptions, data);
    } else {
        if (cfg->debug) {
            // don't report for version, timestamp and uptime
            if (!(!strcmp(topic, "$SYS/broker/version") || !strcmp(topic, "$SYS/broker/timestamp") || !strcmp(topic, "$SYS/broker/uptime"))) {
                fprintf(stderr, "Unaccounted topic %s = %s\n", topic, data);
            }
        }
    }
}

void _mqtt_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
    struct configuration *cfg = (struct configuration *) obj;
    if (message) {
        pthread_mutex_lock(&mqtt_mutex);
        _update_broker_stats(cfg, message->topic, (char *) message->payload);
        pthread_mutex_unlock(&mqtt_mutex);
    }
};

void *mqtt_connect(void *ptr) {
    struct configuration *cfg = (struct configuration *) ptr;
    struct mosquitto *mqtt = NULL;
    int mqtt_rc;
    char *mqtt_id;

    // mosquitto_lib_init _ALWAYS_ returns MOSQ_ERR_SUCCESS
    mosquitto_lib_init();

    mqtt_id = uuidgen();
    if (mqtt_id) {
        printf("INFO: Connecting with client ID %s to broker at %s port %d\n", mqtt_id, cfg->mqtt_host, cfg->mqtt_port);
    } else {
        printf("INFO: Connecting with random client ID to broker at %s port %d\n", cfg->mqtt_host, cfg->mqtt_port);
    }

    mqtt = mosquitto_new(mqtt_id, true, ptr);
    if (!mqtt) {
        fprintf(stderr, "ERROR: Unable to initialize MQTT client structure\n");
        abort();
    }

    // store MQTT structure pointer in configuration for access from the worker thread
    pthread_mutex_lock(&mqtt_mutex);
    cfg->mqtt_handle = mqtt;
    pthread_mutex_unlock(&mqtt_mutex);

    // notify libmosquitto that we are threaded too!
    mosquitto_threaded_set(mqtt, true);

    // setup basic SSL
    if (cfg->mqtt_ssl) {
        if (cfg->mqtt_ssl_insecure) {
            mqtt_rc = mosquitto_tls_opts_set(mqtt, SSL_VERIFY_NONE, NULL, NULL);
        } else {
            mqtt_rc = mosquitto_tls_opts_set(mqtt, SSL_VERIFY_PEER, NULL, NULL);
        }

        if (mqtt_rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "ERROR: Unable to set MQTT TLS options, code %d\n", mqtt_rc);
            exit(mqtt_rc);
        }
    }

    // Connect by either using user/password or client certificate
    // XXX: There is a third option, "pre-shared key over TLS" - mosquitto_tls_psk_set
    if (cfg->mqtt_user) {
        // CA file or CA directory is mandatory when using SSL/TLS. Otherwise the connection will be rejceted (server: "...SSL routines:ssl3_get_record:wrong version number.")
        if (cfg->mqtt_ssl) {
            mqtt_rc = mosquitto_tls_set(mqtt, cfg->mqtt_ssl_cafile, cfg->mqtt_ssl_cadir, NULL, NULL, NULL);
            if (mqtt_rc != MOSQ_ERR_SUCCESS) {
                fprintf(stderr, "ERROR: Unable to set MQTT client certificate, code %d\n", mqtt_rc);
                exit(mqtt_rc);
            }
        }

        mqtt_rc = mosquitto_username_pw_set(mqtt, cfg->mqtt_user, cfg->mqtt_password);
        if (mqtt_rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "ERROR: Unable to set MQTT username/password, code %d\n", mqtt_rc);
            exit(mqtt_rc);
        }
    } else if (cfg->mqtt_ssl_certfile) {
        mqtt_rc = mosquitto_tls_set(mqtt, cfg->mqtt_ssl_cafile, cfg->mqtt_ssl_cadir, cfg->mqtt_ssl_certfile, cfg->mqtt_ssl_keyfile, NULL);
        if (mqtt_rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "ERROR: Unable to set MQTT client certificate, code %d\n", mqtt_rc);
            exit(mqtt_rc);
        }
    }
    

    // set callback handlers
    mosquitto_log_callback_set(mqtt, _mqtt_log_callback);
    mosquitto_connect_callback_set(mqtt, _mqtt_connect_callback);
    mosquitto_subscribe_callback_set(mqtt, _mqtt_subscribe_callback);
    mosquitto_message_callback_set(mqtt, _mqtt_message_callback);
    mosquitto_disconnect_callback_set(mqtt, _mqtt_disconnect_callback);

    if (mosquitto_connect(mqtt, cfg->mqtt_host, cfg->mqtt_port, cfg->mqtt_keepalive) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "ERROR: Can't connect to %s port %d\n", cfg->mqtt_host, cfg->mqtt_port);
        // XXX: Error handling needs improvements!
        abort();
    };

    mqtt_rc = mosquitto_loop_forever(mqtt, cfg->mqtt_connect_timeout, 1);
    switch(mqtt_rc) {
        case MOSQ_ERR_SUCCESS: {
                                   break;
                               }
        case MOSQ_ERR_INVAL: {
                                 fprintf(stderr, "ERROR: MQTT loop failed: MOSQ_ERR_INVAL (input parameters were invalid)\n");
                                 break;
                             }
        case MOSQ_ERR_NOMEM: {
                                 fprintf(stderr, "ERROR: MQTT loop failed: MOSQ_ERR_NOMEM (out of memory condition occurred)\n");
                                 break;
                             }
        case MOSQ_ERR_NO_CONN: {
                                   fprintf(stderr, "ERROR: MQTT loop failed: MOSQ_ERR_NO_CONN (client isn't connected to a broker)\n");
                                   break;
                               }
        case MOSQ_ERR_CONN_LOST: {
                                     fprintf(stderr, "ERROR: MQTT loop failed: MOSQ_ERR_CONN_LOST (connection to the broker was lost)\n");
                                     break;
                                 }
        case MOSQ_ERR_PROTOCOL: {
                                    fprintf(stderr, "ERROR: MQTT loop failed: MOSQ_ERR_PROTOCOL (protocol error communicating with the broker)\n");
                                    break;
                                }
        case MOSQ_ERR_ERRNO: {
                                 fprintf(stderr, "ERROR: MQTT loop failed: MOSQ_ERR_ERRNO (system call returned an error)\n");
                                 break;
                             }
        default:{
                    fprintf(stderr, "ERROR: MQTT loop failed: unknown error\n");
                }
    }

    // clean up when we are done, mosquitto_lib_cleanup also always return MOSQ_ERR_SUCCESS
    mosquitto_lib_cleanup();

    free(mqtt_id);
    return NULL;
}

