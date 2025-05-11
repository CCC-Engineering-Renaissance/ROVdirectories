#include <boost/async_mqtt5/mqtt_client.hpp>
#include <boost/async_mqtt5/types.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <iostream>

int main() {
	boost::asio::io_context ioc;

	boost::mqtt5::mqtt_client<boost::asio::ip::tcp::socket> c(ioc);

	c.brokers("<your-mqtt-broker>", 1883)
		.credentials("<your-client-id>", "<client-username>", "<client-pwd>")
		.async_run(boost::asio::detached);

	c.async_publish<boost::mqtt5::qos_e::at_most_once>(
			"<topic>", "Hello world!",
			boost::mqtt5::retain_e::no, boost::mqtt5::publish_props {},
			[&c](boost::mqtt5::error_code ec) {
			std::cout << ec.message() << std::endl;
			c.async_disconnect(boost::asio::detached);  disconnect and close the Client
			}
			);

	ioc.run();
}
