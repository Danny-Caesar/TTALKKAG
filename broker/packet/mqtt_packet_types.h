#pragma once

#include <stdint.h>

enum class mqtt_packet_type : uint8_t {
    CONNECT     = 1,
    CONNACK     = 2,
    PUBLISH     = 3,
    PUBACK      = 4,
    SUBSCRIBE   = 8,
    SUBACK      = 9,
    PINGREQ     = 12,
    PINGRESP    = 13,
    DISCONNECT  = 14,
};