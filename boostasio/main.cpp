#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main() {
    try {
        boost::asio::io_context io_context;

        udp::socket socket(io_context, udp::endpoint(udp::v4(), 12345));
        std::cout << "UDP server is running on port 12345...\n";

        for (;;) {
            char data[1024];
            udp::endpoint remote_endpoint;
            boost::system::error_code error;

            std::size_t length = socket.receive_from(boost::asio::buffer(data), remote_endpoint, 0, error);

            if (error && error != boost::asio::error::message_size) {
                throw boost::system::system_error(error);
            }

            std::cout << "Received from " << remote_endpoint.address().to_string()
                      << ": " << std::string(data, length) << std::endl;

            socket.send_to(boost::asio::buffer(data, length), remote_endpoint, 0, error);
        }
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << "\n";
    }

    return 0;
}

