#include "client.hpp"
#include <arpa/inet.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace socket_test {

Client::Client(ClientDesc desc)
    : m_soc(0), m_desc(desc), m_address({
                                  .sin_family = AF_INET,
                                  .sin_port = htons(m_desc.port),
                                  .sin_addr = {0},
                              }),
      m_handlers((Handlers){
          .connect_handler = [](int) {}, .disconnect_handler = [](int) {},
          /*.client_data_handler = [](int, const char *, size_t) {}*/}) {
  m_address.sin_addr.s_addr = inet_addr("127.0.0.1");
}

Client::~Client() { close(m_soc); }

void Client::start() {
  int err = 0;

  m_soc = socket(AF_INET, SOCK_STREAM, 0);
  if (m_soc == 0) {
    throw std::runtime_error("Create socket failed");
  }

  err = connect(m_soc, (struct sockaddr *)&m_address, sizeof(m_address));
  if (err < 0) {
    throw std::runtime_error("Connection failed");
  } else {
    m_handlers.connect_handler(m_soc);
  }
}

void Client::stop() {
  m_handlers.disconnect_handler(m_soc);
  close(m_soc);
}

void Client::sendData(const char *buffer, int len) {
  int total = 0;
  int n;

  while (total < len) {
    n = send(m_soc, buffer + total, len - total, 0);
    if (n == -1) {
      throw std::runtime_error("Send failed");
    }
    total += n;
  }
}

} // namespace socket_test
