#include "HttpServer.h"

#include "boost/asio.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "glog/logging.h"
#include "services/http/HttpSession.h"

#include <algorithm>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

namespace services {
namespace http {
namespace {

void initAcceptor(boost::asio::ip::tcp::endpoint endpoint,
                  boost::asio::ip::tcp::acceptor &acceptor) {
  boost::system::error_code errorCode;

  acceptor.open(endpoint.protocol(), errorCode);
  if (errorCode) {
    LOG(FATAL) << "Failed to open for acceptor: " << errorCode.message();
  }

  acceptor.set_option(boost::asio::socket_base::reuse_address(true));
  if (errorCode) {
    LOG(FATAL) << "Failed to set_option for acceptor: " << errorCode.message();
  }

  acceptor.bind(endpoint, errorCode);
  if (errorCode) {
    LOG(FATAL) << "Failed to bind for acceptor: " << errorCode.message();
  }

  acceptor.listen(boost::asio::socket_base::max_listen_connections, errorCode);
  if (errorCode) {
    LOG(FATAL) << "Failed to listen for acceptor: " << errorCode.message();
  }
}

} // namespace

HttpServer::HttpServer(const std::string &ip, int port, int threadCount)
    : ipAddr_(ip), port_(port), threadCount_(threadCount) {}

void HttpServer::doAccept(boost::asio::ip::tcp::acceptor &acceptor,
                          boost::asio::ip::tcp::socket &sock) {
  acceptor.async_accept(
      sock, [&acceptor, &sock, this](boost::system::error_code ec) mutable {
        if (ec) {
          LOG(ERROR) << "Failed to accept" << ec.message();
        } else {
          std::make_shared<HttpSession>(std::move(sock), router_)->start();
        }
        doAccept(acceptor, sock);
      });
}

void HttpServer::setRouter(std::shared_ptr<HttpRouter> router) {
  router_ = router;
}

void HttpServer::serve() {
  auto const address = boost::asio::ip::make_address(ipAddr_);
  auto const port = static_cast<unsigned short>(port_);
  auto const threadCount = std::max<int>(threadCount_, 1);

  ioContext_ = std::make_unique<boost::asio::io_context>(threadCount);
  boost::asio::ip::tcp::acceptor acceptor(*ioContext_);
  boost::asio::ip::tcp::socket sock(*ioContext_);

  initAcceptor(boost::asio::ip::tcp::endpoint{address, port}, acceptor);

  doAccept(acceptor, sock);

  for (int i = 0; i < threadCount - 1; i++) {
    std::thread th([this, i] {
      LOG(INFO) << "launching no." << i << " thread";
      ioContext_->run();
      LOG(INFO) << "Exiting no." << i << " thread";
    });
    th.detach();
  }
  LOG(INFO) << "launching no." << threadCount - 1 << " thread";
  ioContext_->run();
  LOG(INFO) << "Exiting no." << threadCount - 1 << " thread";
}

void HttpServer::stop() {
  if (ioContext_ && !ioContext_->stopped()) {
    ioContext_->stop();
    while (!ioContext_->stopped())
      ;
    ioContext_.reset();
  }
}

} // namespace http
} // namespace services
