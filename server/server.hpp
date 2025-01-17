#pragma once

#include <boost/lockfree/queue.hpp>
#include <cstddef>
#include <functional>
#include <netinet/in.h>

namespace socket_test {

class Server {

public:
  Server(unsigned short port);
  ~Server();

  typedef std::function<void(int clientSocket)> connect_handler_t;
  typedef std::function<void(int clientSocket, const char *buffer, size_t len)>
      client_data_handler_t;

  void start();
  void accepting();
  void processing();

  void setPort(unsigned short port);

  void setConnectHandler(connect_handler_t hndl);
  void setDisconnectHandler(connect_handler_t hndl);
  void setClientDataHandler(client_data_handler_t hndl);

private:
  const int m_connectionQueueLength;

  unsigned short m_port;
  int m_soc;

  char m_buffer[1024];

  struct sockaddr_in m_address;

  struct Handlers {
    connect_handler_t connect_handler;
    connect_handler_t disconnect_handler;
    client_data_handler_t client_data_handler;
  } m_handlers;

  boost::lockfree::queue<int> m_clients {QUEUE_NODES};
};

inline void Server::setPort(unsigned short port) { m_port = port; }

inline void Server::setConnectHandler(connect_handler_t hndl) {
  m_handlers.connect_handler = hndl;
}
inline void Server::setDisconnectHandler(connect_handler_t hndl) {
  m_handlers.disconnect_handler = hndl;
}
inline void Server::setClientDataHandler(client_data_handler_t hndl) {
  m_handlers.client_data_handler = hndl;
}

} // namespace socket_test