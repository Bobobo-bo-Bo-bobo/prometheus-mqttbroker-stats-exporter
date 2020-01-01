#include "prometheus-mqttbroker-stats-exporter.h"

#include <uuid.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#ifndef HAVE_MEMSET

#include <stddef.h>
void *memset(void *s, int c, size_t n) {
    unsigned char* p=s;
    while(n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

#else /* HAVE_MEMSET */

#include <string.h>

#endif /* HAVE_MEMSET */

char *uuidgen(void) {
    char *result;
    uuid_t uuid;

    // UUID string is always 36 byte long + terminating \0
    result = (char *) malloc(37);
    if (!result) {
        // XXX: it is critical if we don't have enough memory to allocate 37 byte
        // OTOH mosquitto_new will generate a ugly random id
        return NULL;
    }

    uuid_generate(uuid);
    uuid_unparse(uuid, result);
    return result;
}

int read_password_from_file(const char *file, struct configuration *cfg) {
    char *buffer;
    FILE *fd;
    char *token;

    buffer = (char *) malloc(READ_BUFFER_SIZE);
    if (!buffer) {
        fprintf(stderr, "Unable to allocate read buffer\n");
        return ENOMEM;
    }
    memset((void *) buffer, 0, READ_BUFFER_SIZE);

    fd = fopen(file, "r");
    if (!fd) {
        free(buffer);
        return errno;
    }

    // XXX; In theory fread may return less than READ_BUFFER_SIZE bytes.
    //      Although unlikely it may be neccessary to handle such corner case
    fread((void *) buffer, sizeof(char), READ_BUFFER_SIZE, fd);
    if (ferror(fd)) {
        fprintf(stderr, "Can't read from %s\n", file);
        free(buffer);
        fclose(fd);
        return EIO;
    }

    // XXX: fclose mail fail too
    fclose(fd);

    // we only read the first line and don't care about the rest (if any)
    token = strtok(buffer, "\r\n");

    // token is NULL if the file is empty or contains only the delimeter
    if (token) {
        // if password has already set, free old string
        if (cfg->mqtt_password) {
            free(cfg->mqtt_password);
        }
        cfg->mqtt_password = strdup(token);
        if (!cfg->mqtt_password) {
            free(buffer);
            return ENOMEM;
        }
    }
    free(buffer);

    return 0;
}

