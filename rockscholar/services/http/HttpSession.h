#pragma once

#include "boost/asio/bind_executor.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "boost/asio/strand.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include <functional>
#include <memory>

#include "services/http/HttpRouter.h"

namespace services {
namespace http {
class HttpSession : public std::enable_shared_from_this<HttpSession> {
private:
  void onWrite(boost::system::error_code ec, std::size_t bytes, bool close);

  void onRead(boost::system::error_code ec, std::size_t bytes);

public:
  HttpSession(boost::asio::ip::tcp::socket &&sock,
              std::shared_ptr<HttpRouter> router);

  void start();

  void shutdown();

  const boost::beast::http::request<boost::beast::http::string_body> &
  getRequest() const;

  template <bool isRequest, class Body, class Fields>
  void respond(boost::beast::http::message<isRequest, Body, Fields> &&msg) {
    auto res =
        std::make_shared<boost::beast::http::message<isRequest, Body, Fields>>(
            std::move(msg));

    response_ = res;
    boost::beast::http::async_write(
        socket_, *res,
        boost::asio::bind_executor(
            strand_, std::bind(&HttpSession::onWrite, shared_from_this(),
                               std::placeholders::_1, std::placeholders::_2,
                               res->need_eof())));
  }

private:
  boost::asio::ip::tcp::socket socket_;
  boost::beast::flat_buffer buffer_;
  boost::beast::http::request<boost::beast::http::string_body> request_;
  std::shared_ptr<void> response_;
  std::shared_ptr<HttpRouter> router_;
  boost::asio::strand<boost::asio::io_context::executor_type> strand_;
};
} // namespace http
} // namespace services
