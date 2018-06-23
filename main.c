#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <limits.h>
#include <errno.h>

#include "prometheus-mqttbroker-stats-exporter.h"
#include "usage.h"
#include "mqtt_connect.h"
#include "exporter.h"

const char *const short_opts = "hu:p:c:k:C:D:iH:P:K:Q:T:t:f:sd";
const struct option long_opts[] = {
    { "help", no_argument, NULL, 'h' },
    { "user", required_argument, NULL, 'u' },
    { "password", required_argument, NULL, 'p' },
    { "cert-file", required_argument, NULL, 'c' },
    { "key-file", required_argument, NULL, 'k' },
    { "ca-file", required_argument, NULL, 'C' },
    { "ca-dir", required_argument, NULL, 'D' },
    { "insecure", no_argument, NULL, 'i' },
    { "host", required_argument, NULL, 'H' },
    { "port", required_argument, NULL, 'P' },
    { "keepalive", required_argument, NULL, 'K' },
    { "qos", required_argument, NULL, 'Q' },
    { "topic", required_argument, NULL, 'T' },
    { "timeout", required_argument, NULL, 't' },
    { "password-file", required_argument, NULL, 'f' },
    { "ssl", no_argument, NULL, 's' },
    { "debug", no_argument, NULL, 'd' },
    { NULL, 0, NULL, 0 },
};

pthread_mutex_t mqtt_mutex = PTHREAD_MUTEX_INITIALIZER;
struct configuration *config;
 
int main(int argc, char** argv) {
    int gorc;
    int optindex;
    pthread_mutexattr_t zwave_mutex_attr;
    char *remain;
    long int converted;
    pthread_t mqtt_connection_thread;
    struct MHD_Daemon *httpd = NULL;

    config = (struct configuration *) malloc(sizeof(struct configuration));
    if (!config) {
        fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory\n", sizeof(struct configuration));
        exit(1);
    }

    memset((void *)config, 0, sizeof(struct configuration));

    // set defaults
    config->mqtt_ssl = false;
    config->mqtt_ssl_insecure = false;
    config->mqtt_port = 1883;
    config->http_port = 11883;
    config->mqtt_keepalive = 10;
    config->mqtt_connect_timeout = -1;
    config->debug = false;
    config->broker_stats = malloc(sizeof(struct statistics));

    if (!config->broker_stats) {
        fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory for broker statistics\n", sizeof(struct statistics));
        exit(1);
    }

    memset(config->broker_stats, 0, sizeof(struct statistics));

    // parse comman line parameters
    for (;;) {
        optindex = 0;
        gorc = getopt_long(argc, argv, short_opts, long_opts, &optindex);
        if (gorc == -1) {
            break;
        }

        switch(gorc) {
            case 'h': {
                          usage();
                          exit(0);
                      }
            case 'u': {
                          config->mqtt_user = strdup(optarg);
                          if (!config->mqtt_user) {
                              fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory for copying the MQTT user parameter\n", strlen(optarg));
                              exit(1);
                          }
                          break;
                      }
            case 'p': {
                          config->mqtt_password = strdup(optarg);
                          if (!config->mqtt_password) {
                              fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory for copying the MQTT password parameter\n", strlen(optarg));
                              exit(1);
                          }
                          break;
                      }
            case 'c': {
                          config->mqtt_ssl_certfile = strdup(optarg);
                          if (!config->mqtt_ssl_certfile) {
                              fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory for copying the MQTT SSL certificate file parameter\n", strlen(optarg));
                              exit(1);
                          }
                          break;
                      }
            case 'k': {
                          config->mqtt_ssl_keyfile = strdup(optarg);
                          if (!config->mqtt_ssl_keyfile) {
                              fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory for copying the MQTT SSL private key parameter\n", strlen(optarg));
                              exit(1);
                          }
                          break;
                      }
            case 'C': {
                          config->mqtt_ssl_cafile = strdup(optarg);
                          if (!config->mqtt_ssl_cafile) {
                              fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory for copying the MQTT SSL CA file parameter\n", strlen(optarg));
                              exit(1);
                          }
                          break;
                      }
            case 'D': {
                          config->mqtt_ssl_cadir = strdup(optarg);
                          if (!config->mqtt_ssl_cadir) {
                              fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory for copying the MQTT SSL CA directory parameter\n", strlen(optarg));
                              exit(1);
                          }
                          break;
                      }
            case 'i': {
                          config->mqtt_ssl_insecure = true;
                          break;
                      }
            case 'H': {
                          config->mqtt_host = strdup(optarg);
                          if (!config->mqtt_host) {
                              fprintf(stderr, "ERROR: Unable to allocate %d bytes of memory for copying the MQTT host parameter\n", strlen(optarg));
                              exit(1);
                          }
                          break;
                      }
            case 'P': {
                          converted = strtol(optarg, &remain, 10);
                          // check for integer over-/underflow or any other error
                          if ((errno == ERANGE && (converted == LONG_MAX || converted == LONG_MIN)) || (errno != 0 && converted == 0)) {
                              fprintf(stderr, "ERROR: Unable to convert %s to a number: %s\n", optarg, strerror(errno));
                              exit(errno);
                          }

                          // Shouldn't happen with getopt
                          if (optarg == remain) {
                              fprintf(stderr, "ERROR: No number found in %s\n", optarg);
                              exit(1);
                          }

                          if (*remain != 0) {
                              fprintf(stderr, "ERROR: Unable to convert %s to a number: non-numeric characters found\n", optarg);
                              exit(1);
                          }
                          config->mqtt_port = (int) converted;
                          break;
                      }
            case 'K':{
                          converted = strtol(optarg, &remain, 10);
                          // check for integer over-/underflow or any other error
                          if ((errno == ERANGE && (converted == LONG_MAX || converted == LONG_MIN)) || (errno != 0 && converted == 0)) {
                              fprintf(stderr, "ERROR: Unable to convert %s to a number: %s\n", optarg, strerror(errno));
                              exit(errno);
                          }

                          // Shouldn't happen with getopt
                          if (optarg == remain) {
                              fprintf(stderr, "ERROR: No number found in %s\n", optarg);
                              exit(1);
                          }

                          if (*remain != 0) {
                              fprintf(stderr, "ERROR: Unable to convert %s to a number: non-numeric characters found\n", optarg);
                              exit(1);
                          }
                          config->mqtt_keepalive = (int) converted;
                          break;
                  }
            case 'Q': {
                          converted = strtol(optarg, &remain, 10);
                          // check for integer over-/underflow or any other error
                          if ((errno == ERANGE && (converted == LONG_MAX || converted == LONG_MIN)) || (errno != 0 && converted == 0)) {
                              fprintf(stderr, "ERROR: Unable to convert %s to a number: %s\n", optarg, strerror(errno));
                              exit(errno);
                          }

                          // Shouldn't happen with getopt
                          if (optarg == remain) {
                              fprintf(stderr, "ERROR: No number found in %s\n", optarg);
                              exit(1);
                          }

                          if (*remain != 0) {
                              fprintf(stderr, "ERROR: Unable to convert %s to a number: non-numeric characters found\n", optarg);
                              exit(1);
                          }
                          config->mqtt_qos = (int) converted;
                          break;
                      }

            case 't': {
                          converted = strtol(optarg, &remain, 10);
                          // check for integer over-/underflow or any other error
                          if ((errno == ERANGE && (converted == LONG_MAX || converted == LONG_MIN)) || (errno != 0 && converted == 0)) {
                              fprintf(stderr, "ERROR: Unable to convert %s to a number: %s\n", optarg, strerror(errno));
                              exit(errno);
                          }

                          // Shouldn't happen with getopt
                          if (optarg == remain) {
                              fprintf(stderr, "ERROR: No number found in %s\n", optarg);
                              exit(1);
                          }

                          if (*remain != 0) {
                              fprintf(stderr, "ERROR: Unable to convert %s to a number: non-numeric characters found\n", optarg);
                              exit(1);
                          }
                          config->mqtt_connect_timeout = (int) converted;
                          break;
                      }
            case 'f': {
                          // TODO: read password from file
                          break;
                      }
            case 'd': {
                          config->debug = true;
                          break;
                      }
            default: {
                         fprintf(stderr, "ERROR: Unknown argument\n\n");
                         usage();
                         exit(1);
                     }
        };
    }

    // some basic sanity checks
    // Authentication: User/Password
    if (((config->mqtt_user) && (!config->mqtt_password)) || ((!config->mqtt_user) && (config->mqtt_password))) {
        fprintf(stderr, "ERROR: When using user/password authentication both, user and password must be set\n");
        exit(1);
    }

    // Authentication: SSL certificate
    if (((config->mqtt_ssl_certfile) && (!config->mqtt_ssl_keyfile)) || ((!config->mqtt_ssl_certfile) && (config->mqtt_ssl_keyfile))) {
        fprintf(stderr, "ERROR: When using SSL certificates for authentication, both certificate file and private key file must be set\n");
        exit(1);
    }

    // Authentication: either user/password or SSL client certificate but not both
    if (((config->mqtt_user) || (config->mqtt_password)) && ((config->mqtt_ssl_certfile) || (config->mqtt_ssl_keyfile))) {
        fprintf(stderr, "ERROR: Use either user/password or SSL client certificate for authentication\n");
        exit(1);
    }
/*
    // Authentication: either user/password or SSL client certificate but at lease one of them
    if ((!config->mqtt_user) && (!config->mqtt_password) && (!config->mqtt_ssl_certfile) && (!config->mqtt_ssl_keyfile)) {
        fprintf(stderr, "ERROR: Use one of user/password or SSL client certificates for authentication\n");
        exit(1);
    }
*/
    // Check port
    if ((config->mqtt_port <= 0) || (config->mqtt_port > 65535)) {
        fprintf(stderr, "ERROR: Invalid MQTT port %d\n", config->mqtt_port);
        exit(1);
    }

    // Check MQTT QoS
    if ((config->mqtt_qos < 0) || (config->mqtt_qos > 2)) {
        fprintf(stderr, "ERROR: Invalid MQTT QoS value, must be either 0, 1 or 2 but not %d\n", config->mqtt_qos);
        exit(2);
    }

    httpd = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, config->http_port, NULL, NULL, &handle_http_request, NULL, MHD_OPTION_END);
    if (!httpd) {
        fprintf(stderr, "ERROR: Unable to start HTTPD thread\n");
        exit(1);
    }

    // XXX: Check for return code of pthread_create !
    pthread_create(&mqtt_connection_thread, NULL, mqtt_connect, (void *) config);
    pthread_join(mqtt_connection_thread, NULL);

    free(config->mqtt_host);
    free(config->mqtt_password);
    free(config->mqtt_ssl_cadir);
    free(config->mqtt_ssl_cafile);
    free(config->mqtt_ssl_certfile);
    free(config->mqtt_ssl_keyfile);
    free(config->mqtt_user);
    free(config);
    exit(0);
}

