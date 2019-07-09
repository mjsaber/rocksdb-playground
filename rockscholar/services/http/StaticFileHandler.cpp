#include "StaticFileHandler.h"

#include "boost/beast/http.hpp"
#include "glog/logging.h"
#include "services/http/HttpUtils.h"
#include <string>

namespace beast_http = boost::beast::http;

namespace services {
namespace http {

StaticFileHandler::StaticFileHandler(const std::string &rootPath)
    : rootPath_(rootPath) {}

void StaticFileHandler::handle(std::shared_ptr<HttpSession> session) {
  const auto &req = session->getRequest();
  auto target = req.target().to_string();
  auto pos = target.find("?");
  if (pos != std::string::npos) {
    target.resize(pos);
  }
  LOG(INFO) << "Handle static file request: " << target;

  if (req.method() != beast_http::verb::get &&
      req.method() != beast_http::verb::head) {
    session->respond(HttpUtils::buildErrorResponse(
        session, beast_http::status::bad_request, "Unsupported HTTP method"));
    return;
  }

  if (target.empty() || target[0] != '/' ||
      target.find("..") != std::string::npos) {
    session->respond(HttpUtils::buildErrorResponse(
        session, beast_http::status::bad_request, "Illegal target"));
    return;
  }

  target = rootPath_ + "" + target;
  LOG(INFO) << "Looking for file: " << target;

  if (target.back() == '/') {
    target += "index.html";
  }

  boost::beast::error_code ec;
  beast_http::file_body::value_type body;
  body.open(target.c_str(), boost::beast::file_mode::scan, ec);

  if (ec == boost::system::errc::no_such_file_or_directory) {
    LOG(ERROR) << "File not found: " << target;
    session->respond(HttpUtils::buildErrorResponse(
        session, beast_http::status::not_found,
        "File Not Found: " + req.target().to_string()));
    return;
  }

  if (ec) {
    LOG(ERROR) << ec.message();
    session->respond(HttpUtils::buildErrorResponse(
        session, beast_http::status::internal_server_error, ec.message()));
    return;
  }

  auto sz = body.size();

  if (req.method() == beast_http::verb::head) {
    beast_http::response<beast_http::empty_body> res{beast_http::status::ok,
                                                     req.version()};
    res.set(beast_http::field::server, HttpUtils::getServerVersion());
    res.set(beast_http::field::content_type,
            HttpUtils::getMimeTypeForExtension(target));
    res.content_length(sz);
    res.keep_alive(req.keep_alive());
    session->respond(std::move(res));
    return;
  }

  beast_http::response<beast_http::file_body> res{
      std::piecewise_construct, std::make_tuple(std::move(body)),
      std::make_tuple(beast_http::status::ok, req.version())};
  res.set(beast_http::field::server, HttpUtils::getServerVersion());
  res.set(beast_http::field::content_type,
          HttpUtils::getMimeTypeForExtension(target));
  res.content_length(sz);
  res.keep_alive(req.keep_alive());
  session->respond(std::move(res));
}

} // namespace http
} // namespace services
