#include <iostream>
#include <boost/asio.hpp>
#include <csignal>
#include "socket/socket_broker.h"
#include "subscription/subscription_manager.h"
#include "logo_display.h"
#include "packet_counter.h"

void signal_handler(int signum) {
    PacketCounter::getInstance().set_end_time(std::chrono::high_resolution_clock::now());
    std::cout << "\nCaught signal " << signum << ". Shutting down...\n";
    
    // PacketCounter에서 최종 데이터 가져오기
    const auto total_bytes = PacketCounter::getInstance().get_total_bytes();
    const auto avg_pps = PacketCounter::getInstance().get_avg_pps();
    
    // 1. 콘솔에 출력
    std::cout << "==============================================\n";
    std::cout << "Total processed control packet bytes: " << total_bytes << "\n";
    std::cout << "Average processed control packet per sec: " << avg_pps << "\n";
    std::cout << "==============================================\n";

    // 2. 파일에 저장
    PacketCounter::getInstance().save_to_file("packet_log.txt");

    // 프로그램 종료
    exit(signum);
}

int main()
{
    signal(SIGINT, signal_handler);
    PacketCounter::getInstance().set_start_time(std::chrono::high_resolution_clock::now());
    print_logo();
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::acceptor acceptor(
        io_context,
        boost::asio::ip::tcp::endpoint(
            boost::asio::ip::tcp::v4(),
            1883)
            //atoi(std::getenv("MQTT_PORT")))
    );

    std::function<void()> start_accept = [&]()
    {
        std::shared_ptr<boost::asio::ip::tcp::socket> socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context);

        acceptor.async_accept(*socket, [&, socket](const boost::system::error_code& ec)
        {
            if (!ec)
            {
                // tcp connection log
                boost::asio::ip::tcp::endpoint remote_ep = socket->remote_endpoint();
                std::string client_ip = remote_ep.address().to_string();
                // unsigned short client_port = remote_ep.port();
                //std::cout << "소켓에 " << client_ip << "::" << std::getenv("MQTT_PORT") << " 접속.\n";

                std::shared_ptr<socket_broker> broker = std::make_shared<socket_broker>(std::move(*socket));
                broker->start();  // Start read loop
            }

            start_accept();  // Keep accepting more clients
        });
    };

    start_accept();
    io_context.run();

    return 0;
}
