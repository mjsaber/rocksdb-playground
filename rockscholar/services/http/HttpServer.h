#pragma once

#include "services/http/HttpSession.h"
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace services {
namespace http {
class HttpServer {
public:
  HttpServer(const std::string &ip, int port, int threadCount);

  void setRouter(std::shared_ptr<HttpRouter> router);

  // will block the current thread until ctrl-C is pressed
  void serve();

  void stop();

private:
  void doAccept(boost::asio::ip::tcp::acceptor &acceptor,
                boost::asio::ip::tcp::socket &sock);

  const std::string ipAddr_;
  const int port_;
  const int threadCount_;
  std::shared_ptr<HttpRouter> router_;

  std::unique_ptr<boost::asio::io_context> ioContext_;
};
} // namespace http
} // namespace services
