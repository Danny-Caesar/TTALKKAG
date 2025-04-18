#include <unordered_map>
#include <inttypes.h>
#include <string>

class packet_identifier_manager
{
public:
    static packet_identifier_manager& get_instance();

    uint16_t issue_packet_identifier(const uint8_t qos);
    uint8_t get_packet_qos(const uint16_t packet_identifier);
    void acknowledge_packet_identifier(const uint16_t packet_identifier);

    void debug();
    
private:
    // Singleton patern
    packet_identifier_manager() = default;
    packet_identifier_manager(const packet_identifier_manager&) = delete;
    packet_identifier_manager operator=(const packet_identifier_manager&) = delete;

    static uint16_t _packet_identifier;
    static std::unordered_map<uint16_t, uint8_t> _packet_identifier_map;
};