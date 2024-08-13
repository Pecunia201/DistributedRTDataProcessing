#include "producer.h"
#include <iostream>
#include <glib.h>
#include <librdkafka/rdkafkacpp.h>

#include "common.c"

const std::string PREFIX = "[Kafka Producer] ";

rd_kafka_t* producer;
rd_kafka_conf_t* conf;
char errstr[512];

void dr_msg_cb(rd_kafka_t* kafka_handle, const rd_kafka_message_t* rkmessage, void* opaque) {
    if (rkmessage->err) {
        std::cerr << PREFIX << "Message delivery failed: " << rd_kafka_err2str(rkmessage->err) << std::endl;
    }
}

void initialise_kafka() {
    // Create client configuration
    conf = rd_kafka_conf_new();

    // Configuration
    rd_kafka_conf_set(conf, "bootstrap.servers", "localhost:65357", NULL, 0);
    rd_kafka_conf_set(conf, "acks", "all", NULL, 0);
    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    // Create the Producer instance.
    producer = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!producer) {
        std::cerr << PREFIX << "Failed to create new producer: " << errstr << std::endl;
        exit(1);
    }

    // Configuration object is now owned, and freed, by the rd_kafka_t instance.
    conf = NULL;
}

void cleanup_kafka() {
    // Block until the messages are all sent.
    std::cout << PREFIX << "Flushing final messages.." << std::endl;
    rd_kafka_flush(producer, 10 * 1000);

    if (rd_kafka_outq_len(producer) > 0) {
        std::cerr << PREFIX << rd_kafka_outq_len(producer) << " message(s) were not delivered" << std::endl;
    }

    rd_kafka_destroy(producer);
}

void produce_message(const std::string& topic, const std::string& key, const std::string& value) {
    rd_kafka_resp_err_t err;

    err = rd_kafka_producev(producer,
        RD_KAFKA_V_TOPIC(topic.c_str()),
        RD_KAFKA_V_MSGFLAGS(RD_KAFKA_MSG_F_COPY),
        RD_KAFKA_V_KEY((void*)key.c_str(), key.length()),
        RD_KAFKA_V_VALUE((void*)value.c_str(), value.length()),
        RD_KAFKA_V_OPAQUE(NULL),
        RD_KAFKA_V_END);

    if (err) {
        std::cerr << PREFIX << "Failed to produce to topic " << topic << ": " << rd_kafka_err2str(err) << std::endl;
    }
    else {
        std::cout << PREFIX << "Produced event to topic " << topic << ": key = " << key << " value = " << value << std::endl;
    }

    rd_kafka_poll(producer, 0);
}