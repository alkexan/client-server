#include "server.hpp"
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>
#include <vector>

std::mutex mutex;

std::vector<std::thread> threads;
std::unique_ptr<socket_test::Server> server;

void errorHandler(int err);

void disconnClientHandler(int client) {
  std::cout << client << " client was disconnected" << std::endl;
}

void connClientHandler(int client) {
  std::cout << client << " client was connected" << std::endl;
}

void clientDataHandler(int client, const char *buffer, size_t len) {
  std::cout << "received from " << client << ": \t" << std::string(buffer)
            << std::endl;
}

int main(int argc, char **argv) {
  int err = 0;
#ifndef DEBUG
  if (argc > 1) {
    uint16_t port = htons(*(uint16_t *)argv[1]);
#else
  uint16_t port = 8080;
#endif // DEBUG

    try {
      server = std::make_unique<socket_test::Server>(port);
      server->setConnectHandler(connClientHandler);
      server->setDisconnectHandler(disconnClientHandler);
      server->setClientDataHandler(clientDataHandler);

      server->start();

      threads.emplace_back([]() {
        while (true) {
          server->accepting();
        };
      });

      threads.emplace_back([]() {
        while (true) {
          server->waitData();
        };
      });

      for (auto &t : threads) {
        t.join();
      }
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
