#include <iostream>
#include <mqtt/async_client.h>

const std::string SERVER_ADDRESS = "tcp://broker.hivemq.com:1883";  // Public MQTT broker
const std::string CLIENT_ID = "raspberry_pi_publisher";
const std::string TOPIC = "test/topic";

int main() {
    mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

    // Set up connection options
    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);

    try {
        // Connect to broker
        std::cout << "Connecting to MQTT broker at " << SERVER_ADDRESS << "..." << std::endl;
        client.connect(connOpts)->wait();
        std::cout << "Connected!" << std::endl;

        // Publish a message
        std::string payload = "Hello from Raspberry Pi!";
        mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, payload);
        pubmsg->set_qos(1);

        std::cout << "Publishing message: " << payload << std::endl;
        client.publish(pubmsg)->wait_for(std::chrono::seconds(10));

        // Disconnect
        client.disconnect()->wait();
        std::cout << "Disconnected." << std::endl;
    }
    catch (const mqtt::exception &exc) {
        std::cerr << "MQTT Exception: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}
