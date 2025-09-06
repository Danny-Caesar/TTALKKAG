#ifndef PACKET_COUNTER_H
#define PACKET_COUNTER_H

#include <atomic>
#include <string>
#include <chrono>

class PacketCounter {
public:
    // 싱글톤 인스턴스를 가져오는 정적 메소드
    static PacketCounter& getInstance();

    // 수신된 바이트를 추가하는 메소드
    void add(size_t bytes);

    // 현재까지 집계된 총 바이트를 반환하는 메소드
    size_t get_total_bytes() const;

    double get_avg_pps() const;

    // 집계된 데이터를 파일에 저장하는 메소드
    void save_to_file(const std::string& filename) const;

    void set_start_time(std::chrono::high_resolution_clock::time_point t);
    void set_end_time(std::chrono::high_resolution_clock::time_point t);


private:
    // 싱글톤 패턴을 위한 private 생성자 및 소멸자
    PacketCounter() = default;
    ~PacketCounter() = default;
    PacketCounter(const PacketCounter&) = delete;
    PacketCounter& operator=(const PacketCounter&) = delete;

    // 여러 스레드에서 안전하게 접근할 수 있도록 std::atomic 사용
    std::atomic<size_t> _total_bytes{0};
    std::atomic<size_t> _total_packets{0};
    std::chrono::high_resolution_clock::time_point _start_time;
    std::chrono::high_resolution_clock::time_point _end_time;
};

#endif // PACKET_COUNTER_H