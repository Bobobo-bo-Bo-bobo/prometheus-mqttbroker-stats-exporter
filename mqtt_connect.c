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

void _mqtt_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
    struct configuration *cfg = (struct configuration *) obj;
    if (message) {
        printf("> MQTT message on %s of %d bytes -> %s\n", message->topic, message->payloadlen, (char *)message->payload);
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


