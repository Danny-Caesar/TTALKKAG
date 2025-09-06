#include "packet_counter.h"
#include <fstream>
#include <iostream>

// C++11 이상에서 스레드에 안전한 싱글톤 인스턴스 생성
PacketCounter& PacketCounter::getInstance() {
    static PacketCounter instance;
    return instance;
}

void PacketCounter::add(size_t bytes) {
    // 원자적으로(atomically) 값을 더함
    _total_bytes += bytes;
    _total_packets++;
}

size_t PacketCounter::get_total_bytes() const {
    // 원자적으로 값을 읽어옴
    return _total_bytes.load();
}

double PacketCounter::get_avg_pps() const {
    std::chrono::duration<double> excution_seconds = _end_time - _start_time;
    return _total_packets / excution_seconds.count();
}

void PacketCounter::save_to_file(const std::string& filename) const {
    // std::ios_base::app 플래그를 추가하여 파일을 이어쓰기 모드로 엽니다.
    std::ofstream outfile(filename, std::ios_base::app);

    if (outfile.is_open()) {
        // 현재 시간을 가져와서 로그에 타임스탬프를 남깁니다.
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        
        // ctime은 자체적으로 개행 문자를 포함합니다.
        outfile << "----------------------------------------\n";
        outfile << "Log entry at: " << std::ctime(&now_time);
        outfile << "Total processed control packet bytes: " << get_total_bytes() << '\n';
        outfile << "Average processed control packet per sec: " << get_avg_pps() << '\n';
        outfile.close();
    } else {
        std::cerr << "Error: Could not open file '" << filename << "' for writing.\n";
    }
}

void PacketCounter::set_start_time(std::chrono::high_resolution_clock::time_point t){
    _start_time = t;
}

void PacketCounter::set_end_time(std::chrono::high_resolution_clock::time_point t){
    _end_time = t;
}