#include <vector>
#include "mqtt_control_packet.h"
#include "fixed_header.h"

class puback_packet : public mqtt_control_packet
{
public:
    struct variable_header
    {
        uint16_t packet_identifier;
        
        static variable_header parse(const uint8_t* data, size_t size);
        void debug();
    } v_header;

    static std::unique_ptr<puback_packet> parse(const uint8_t* data, size_t size);
    static std::unique_ptr<puback_packet> create(uint16_t packet_identifier);
    
    std::vector<uint8_t> serialize() const
    {
        std::vector<uint8_t> packet;

        // 1. Fixed header
        packet.push_back(0x40); // PUBACK (4 << 4), flags
        // Remaining length
        auto remaining_len_bytes = fixed_header::encode_remaining_length(2);
        packet.insert(packet.end(), remaining_len_bytes.begin(), remaining_len_bytes.end());

        // 2. Variable header
        // Packet identifier
        packet.push_back((v_header.packet_identifier >> 8) & 0xFF);
        packet.push_back(v_header.packet_identifier & 0xFF);

        return packet;
    }
    
    void debug();
};