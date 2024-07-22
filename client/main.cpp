#include "client.hpp"
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>

std::unique_ptr<socket_test::Client> client;

void clientConnectHandler(const int sd) {
  std::cout << "client was connected" << std::endl;
}

void clientDisconnectHandler(const int sd) {
  std::cout << "client was disconnected" << std::endl;
}

int main(int argc, char **argv) {
  int err = 0;
  uint16_t port;
  unsigned short perSec;
  std::string clientName;

#ifndef DEBUG
  if (argc > 1) {
    port = htons(*(uint16_t *)argv[1]);
#else
  port = 8080;
  perSec = 3;
  clientName = "Client";
#endif // DEBUG

    try {
      client = std::make_unique<socket_test::Client>(
          (socket_test::Client::ClientDesc){
              .port = port, .perSec = perSec, .clientName = clientName});

      client->setConnectHandler(clientConnectHandler);
      client->setDisconnectHandler(clientDisconnectHandler);

      std::thread thread([]() {
        std::time_t time;
        char timeString[std::size("yyyy-mm-dd hh:mm:ss.ms")];
        std::string sendMessage;
        while (true) {
          client->start();

          time = std::time({});
          std::strftime(std::data(timeString), std::size(timeString), "%F %T ",
                        std::gmtime(&time));

          sendMessage = "[" + std::string(timeString) + "] " + "\"" +
                        client->getClientDesc().clientName + +"\"";

          client->sendData(sendMessage.c_str(), sendMessage.length());
          client->stop();

          const auto per = std::chrono::seconds(client->getClientDesc().perSec);
          std::this_thread::sleep_for(per);
        }
      });

      thread.join();

    } catch (const std::exception &e) {
      err = errno;

      std::cerr << "Exception: " << e.what() << std::endl;
      std::cerr << "Code: " << err << " Err: " << std::strerror(err)
                << std::endl;
    }
#ifndef DEBUG
  } else {
    err = EXIT_FAILURE;
  }
#endif // DEBUG
  return err;
}
