**_Note:_** Because I'm running my own servers for several years, main development is done at at https://git.ypbind.de/cgit/prometheus-mqttbroker-stats-exporter/

----

Export MQTT broker statistics from `$SYS/#` topic for Prometheus
================================================================

## Preface
MQTT brokers like [Mosquitto](https://mosquitto.org/) provide statistics using the special MQTT topic `$SYS/#`

This program subscribes to the `$SYS/#` topic of the MQTT broker and exports the data provided on this topic
for [Prometheus](https://prometheus.io).

[prometheus-mqttbroker-stats-exporter](https://git.ypbind.de/cgit/prometheus-icinga2stats-exporte-mqttbroker-stats-exporter/about/) has been
tested with the Mosquitto MQTT server.

## Requirements
### Mosquitto
MQTT access is done using the Mosquitto library.

Mosquitto is dual licensed under the Eclipse Public License 1.0 and the
Eclipse Distribution License 1.0 as described in the mosquitto-edl-v10.txt and mosquitto-epl-v10.txt files
found in this repository.

### GNU Libmicrohttpd
The HTTP server is realised using the [GNU Libmicrohttpd](https://www.gnu.org/software/libmicrohttpd/) library.

GNU Libmicrohttpd is licensed under the [GNU Lesser General Public License, version 2.1](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html).

## Command line parameters

   * `-u <user>` / `--user=<user>` - Authenticate as `<user>` on the MQTT broker
   * `-p <password>` / `--password=<password>` - Use `<password>` for authentication as `<user>` when connecting to the MQTT broker
   * `-c <cert_file>` / `--cert-file=<cert_file>` - Authenticate with SSL client certificate. Public SSL key will be read from `<cert_file>`
   * `-k <key_file>` / `--key-file=<key_file>` - Use private SSL key from `<key_file>`for SSL client certificate authentication. The key file must be unencrypted.
   * `-C <ca_file>` / `--ca-file=<ca_file>` - For SSL connection `<ca_file>` contains the CA certificate
   * `-D <ca_dir>` / `--ca-dir=<ca_dir>` - For SSL connection `<ca_dir>` contains CA certificate
   * `-i` / `--insecure` - Don't verify SSL certificate of the MQTT broker
   * `-H <host>` / `--host=<host>` - Hostname or address of MQTT broker to connect to. _This option is mandatory_
   * `-P <port>` / `--port=<port>` - Connect to MQTT broker at port `<port>`. Default is port 1883
   * `-K <keepalive>` / `--keepalive=<keepalive>` - Send keep alive packets to the MQTT broker every `<keepalive>` seconds. Default is 10 seconds
   * `-Q <qos>` / `--qos=<qos>` - Set MQTT QoS (0,1 or 2). Default is QoS 0.
   * `-t <timeout>`/ `--timeout=<timeout>` - MQTT connection timeout in ms. Default is 1000 ms
   * `-f <file>` / `--password-file=<file>` - Read password for user/password authentication from `<file>`
   * `-s` / `--ssl` - Use SSL for MQTT connection
   * `-d` / `--debug` - Enable debug output
   * `-l <port>` / `--listen=<port>` - Run on `<port>` for metrics export to Prometheus. Default: 11883

## Exported metrics

<table>
    <tr>
        <th>MQTT topic</th>
        <th>Metric name</th>
        <th>Metric type</th>
        <th>Description</th>
    </tr>
    <tr>
        <td>$SYS/broker/bytes/received</td>
        <td>mqtt_broker_bytes_received_total</td>
        <td>counter</td>
        <td>total number of bytes received since the broker started</td>
    </tr>
    <tr>
        <td>$SYS/broker/bytes/sent/<td>
        <td>mqtt_broker_bytes_sent_total</td>
        <td>counter</td>
        <td>total number of bytes sent since the broker started</td>
    </tr>
    <tr>
        <td>$SYS/broker/clients/expired</td>
        <td>mqtt_broker_clients_expired</td>
        <td>gauge</td>
        <td>number of disconnected persistent clients that have been expired and removed through the persistent_client_expiration option</td>
    </tr>
    <tr>
        <td>$SYS/broker/clients/maximum</td>
        <td>mqtt_broker_clients_maximum</td>
        <td>gauge</td>
        <td>maximum number of clients that have been connected to the broker at the same time</td>
    </tr>
    <tr>
        <td>$SYS/broker/clients/total</td>
        <td>mqtt_broker_total_clients</td>
        <td>gauge</td>
        <td>total number of connected and disconnected clients with a persistent session currently connected and registered on the broker</td>
    </tr>
    <tr>
        <td>$SYS/broker/clients/connected,</td>
        <td>mqtt_broker_clients_connected</td>
        <td>gauge</td>
        <td>number of connected clients</td>
    </tr>
    <tr>
        <td>$SYS/broker/clients/active</td>
        <td>mqtt_broker_clients_active</td>
        <td>gauge</td>
        <td>number of active clients</td>
    </tr>
    <tr>
        <td>$SYS/broker/clients/disconnected,</td>
        <td>mqtt_broker_clients_disconnected</td>
        <td>gauge</td>
        <td>number of persistent clients (with clean session disabled) that are registered at the broker but are currently disconnected</td>
    </tr>
    <tr>
        <td>$SYS/broker/clients/inactive</td>
        <td>mqtt_broker_clients_inactive</td>
        <td>gauge</td>
        <td>number of persistent clients (with clean session disabled) that are registered at the broker but are currently disconnected</td>
    </tr>
    <tr>
        <td>SYS/broker/heap/current</td>
        <td>mqtt_broker_heap_current</td>
        <td>gauge</td>
        <td>size of the heap memory in use by mosquitto</td>
    </tr>
    <tr>
        <td>$SYS/broker/heap/maximum</td>
        <td>mqtt_broker_heap_maximum</td>
        <td>gauge</td>
        <td>maximal size of the heap memory in use by mosquitto</td>
    </tr>
    <tr>
        <td>$SYS/broker/load/connections/+</td>
        <td>mqtt_broker_connections_1min</td>
        <td>gauge</td>
        <td>moving average of the number of CONNECT packets received by the broker over different time intervals</td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_connections_5min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_connections_15min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td>$SYS/broker/load/bytes/received/+</td>
        <td>mqtt_broker_bytes_received_1min</td>
        <td>gauge</td>
        <td>moving average of the number of bytes received by the broker over different time intervals</td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_bytes_received_5min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_bytes_received_15min</td>
        <td>gauge</td>
        <td></td>
    </tr>

    <tr>
        <td>$SYS/broker/load/messages/sent/+</td>
        <td>mqtt_broker_bytes_sent_1min</td>
        <td>gauge</td>
        <td>moving average of the number of bytes sent by the broker over different time intervals</td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_bytes_sent_5min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_bytes_sent_15min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td>$SYS/broker/load/messages/received/+</td>
        <td>mqtt_broker_messages_received_1min</td>
        <td>gauge</td>
        <td>moving average of the number of all types of MQTT messages received by the broker over different time intervals</td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_messages_received_5min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_messages_received_15min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td>$SYS/broker/load/messages/sent/+</td>
        <td>mqtt_broker_messages_sent_1min</td>
        <td>gauge</td>
        <td>moving average of the number of all types of MQTT messages sent by the broker over different time intervals</td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_messages_sent_5min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_messages_sent_15min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td>$SYS/broker/load/publish/sent/+</td>
        <td>mqtt_broker_publish_sent_1min</td>
        <td>gauge</td>
        <td>moving average of the number of publish messages sent by the broker over different time intervals</td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_publish_sent_5min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_publish_sent_15min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td>$SYS/broker/load/publish/received/+</td>
        <td>mqtt_broker_publish_received_1min</td>
        <td>gauge</td>
        <td>moving average of the number of publish messages received by the broker over different time intervals</td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_publish_received_5min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_publish_received_15min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td>$SYS/broker/load/sockets/+</td>
        <td>mqtt_broker_sockets_1min</td>
        <td>gauge</td>
        <td>moving average of the number of socket connections opened to the broker over different time intervals</td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_sockets_5min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td></td>
        <td>mqtt_broker_sockets_15min</td>
        <td>gauge</td>
        <td></td>
    </tr>
    <tr>
        <td>$SYS/broker/messages/received</td>
        <td>mqtt_broker_messages_received_total</td>
        <td>counter</td>
        <td>total number of messages of any type received since the broker started</td>
    </tr>
    <tr>
        <td>$SYS/broker/messages/sent</td>
        <td>mqtt_broker_messages_sent_total</td>
        <td>counter</td>
        <td>total number of messages of any type sent since the broker started</td>
    </tr>

    <tr>
        <td>$SYS/broker/messages/stored</td>
        <td>mqtt_broker_messages_stored</td>
        <td>gauge</td>
        <td>number of messages currently held in the message store</td>
    </tr>
    <tr>
        <td>$SYS/broker/publish/messages/dropped</td>
        <td>mqtt_broker_publish_messages_dropped_total</td>
        <td>counter</td>
        <td>total number of publish messages that have been dropped due to inflight/queuing limits</td>
    </tr>
    <tr>
        <td>$SYS/broker/publish/messages/received</td>
        <td>mqtt_broker_publish_messages_received_total</td>
        <td>counter</td>
        <td>total number of PUBLISH messages received since the broker started</td>
    </tr>
    <tr>
        <td>$SYS/broker/publish/messages/sent</td>
        <td>mqtt_broker_publish_messages_sent_total</td>
        <td>counter</td>
        <td>total number of PUBLISH messages sent since the broker started</td>
    </tr>
    <tr>
        <td>$SYS/broker/publish/bytes/received</td>
        <td>mqtt_broker_publish_bytes_received_total</td>
        <td>counter</td>
        <td>total number of PUBLISH received since the broker started</td>
    </tr>
    <tr>
        <td>$SYS/broker/publish/bytes/sent</td>
        <td>mqtt_broker_publish_bytes_sent_total</td>
        <td>counter</td>
        <td>total number of PUBLISH sent since the broker started</td>
    </tr>
    <tr>
        <td>$SYS/broker/retained messages/count</td>
        <td>mqtt_broker_messages_retained</td>
        <td>gauge</td>
        <td>number of retained messages active on the broker</td>
    </tr>
    <tr>
        <td>$SYS/broker/subscriptions/count</td>
        <td>mqtt_broker_subscriptions</td>
        <td>gauge</td>
        <td>number of subscriptions active on the broker</td>
    </tr>
</table>

---

## License
This program is licenses under [GLPv3](http://www.gnu.org/copyleft/gpl.html).

