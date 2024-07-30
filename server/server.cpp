#include "server.hpp"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <mutex>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace socket_test {

Server::Server(unsigned short port)
    : m_connectionQueueLength(5), m_port(port), m_soc(0),
      m_address({.sin_family = AF_INET, .sin_port = htons(m_port)}),
      m_handlers(
          (Handlers){.connect_handler = [](int) {},
                     .disconnect_handler = [](int) {},
                     .client_data_handler = [](int, const char *, size_t) {}}) {
  m_clients.clear();
  m_address.sin_addr.s_addr = INADDR_ANY;
}

Server::~Server() { close(m_soc); }

void Server::start() {
  int err;
  int flag = true;

  m_soc = socket(AF_INET, SOCK_STREAM, 0);
  if (m_soc == 0) {
    throw std::runtime_error("Create socket failed");
  }

  err = setsockopt(m_soc, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
  if (err < 0) {
    throw std::runtime_error("configure failed");
  }

  err = bind(m_soc, (struct sockaddr *)&m_address, sizeof(m_address));
  if (err < 0) {
    throw std::runtime_error("bind failed");
  }

  err = listen(m_soc, m_connectionQueueLength);
  if (err < 0) {
    throw std::runtime_error("listen failed");
  }

  std::cout << "Waiting for connections ..." << std::endl;
}

void Server::accepting() {
  int newClient;
  struct sockaddr_in client_addr = {0};
  size_t addrlen = sizeof(m_address);

  newClient =
      accept(m_soc, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen);

  if (newClient < 0) {
    std::cerr << "accept failed" << std::endl;
  } else {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_handlers.connect_handler(newClient);
    m_clients.insert(newClient);
  }
}

void Server::waitData() {
  std::lock_guard<std::mutex> lock(m_mutex);

  if (m_clients.size() > 0) {
    for (auto clientSocket : m_clients) {
      int answ = recv(clientSocket, m_buffer, sizeof(m_buffer), 0);

      if (answ <= 0) {
        m_handlers.disconnect_handler(clientSocket);
        disconnect(clientSocket);
        break;
      } else {
        m_handlers.client_data_handler(clientSocket, m_buffer, answ);
      }
    }
  }
}

void Server::disconnect(int clientSocket) {
  if (clientSocket) {
    close(clientSocket);
    m_clients.erase(clientSocket);
  }
}

} // namespace socket_test