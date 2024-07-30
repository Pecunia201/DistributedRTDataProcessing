// producer.h

#ifndef PRODUCER_H
#define PRODUCER_H

#include <string>

void initialise_kafka();
void cleanup_kafka();
void produce_message(const std::string& topic, const std::string& key, const std::string& value);

#endif // PRODUCER_H
