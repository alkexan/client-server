#pragma once

#include <functional>
#include <netinet/in.h>
#include <string>

namespace socket_test {

class Client {

public:
  struct ClientDesc {
    unsigned short port;
    unsigned short perSec;
    std::string clientName;
  };

  Client(ClientDesc desc);
  ~Client();

  typedef std::function<void(int clientSocket)> connect_handler_t;

  void start();
  void stop();
  void sendData(const char *buffer, int len);

  void setConnectHandler(connect_handler_t hndl);
  void setDisconnectHandler(connect_handler_t hndl);

  const ClientDesc &getClientDesc() const;

private:
  int m_soc;
  ClientDesc m_desc;
  struct sockaddr_in m_address;

  struct Handlers {
    connect_handler_t connect_handler;
    connect_handler_t disconnect_handler;
  } m_handlers;
};

inline void Client::setConnectHandler(connect_handler_t hndl) {
  m_handlers.connect_handler = hndl;
}
inline void Client::setDisconnectHandler(connect_handler_t hndl) {
  m_handlers.disconnect_handler = hndl;
}

inline const Client::ClientDesc &Client::getClientDesc() const {
  return m_desc;
}

} // namespace socket_test