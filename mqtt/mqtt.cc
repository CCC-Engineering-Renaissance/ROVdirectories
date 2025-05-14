#include <iostream>
#include <mqtt/async_client.h>

const std::string SERVER_ADDRESS = "192.168.8.120:1883";  // Public MQTT broker
const std::string CLIENT_ID = "raspberry_pi_publisher";
const std::string TOPIC = "test/topic";

class SubscriberCallback : public virtual mqtt::callback
{
	public:
		    void connection_lost(const std::string& cause) override
			        {
					        std::cout << "Connection lost: " << cause << std::endl;
						    }

		        void message_arrived(mqtt::const_message_ptr message) override
				    {
					            std::cout << "Message arrived: " << message->get_payload_str() << std::endl;
						        }

			    void delivery_complete(mqtt::delivery_token_ptr token) override
				        {
						        std::cout << "Message delivered" << std::endl;
							    }
};
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

		client.subscribe("controller/LeftJoystickX", 1)->wait();
		SubscriberCallback callback;
		 client.set_callback(callback);

		// Publish a message
		std::string payload = "Hello from Raspberry Pi!";
		mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, payload);
		pubmsg->set_qos(1);

		std::cout << "Publishing message: " << payload << std::endl;
		client.publish(pubmsg)->wait_for(std::chrono::seconds(10));
		while(1){}

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
