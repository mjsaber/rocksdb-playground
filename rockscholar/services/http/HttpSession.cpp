#include "HttpSession.h"

#include "glog/logging.h"

namespace services {
namespace http {

HttpSession::HttpSession(boost::asio::ip::tcp::socket &&sock,
                         std::shared_ptr<HttpRouter> router)
    : socket_(std::move(sock)), router_(router),
      strand_(socket_.get_executor()) {}

const boost::beast::http::request<boost::beast::http::string_body> &
HttpSession::getRequest() const {
  return request_;
}

void HttpSession::onWrite(boost::system::error_code ec,
                          std::size_t /** bytes **/, bool close) {
  if (ec) {
    LOG(ERROR) << "Error when writing: " << ec.message();
    return;
  }
  if (close) {
    shutdown();
    return;
  }

  response_ = nullptr;

  start();
}

void HttpSession::onRead(boost::system::error_code ec,
                         std::size_t /** bytes **/) {
  if (ec == boost::beast::http::error::end_of_stream) {
    shutdown();
    return;
  }
  if (ec) {
    LOG(ERROR) << "Error when reading: " << ec.message();
    return;
  }
  router_->routeRequest(shared_from_this());
}

void HttpSession::start() {
  request_ = {};
  boost::beast::http::async_read(
      socket_, buffer_, request_,
      boost::asio::bind_executor(
          strand_, std::bind(&HttpSession::onRead, shared_from_this(),
                             std::placeholders::_1, std::placeholders::_2)));
}

void HttpSession::shutdown() {
  boost::system::error_code ec;
  socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
}

} // namespace http
} // namespace services
