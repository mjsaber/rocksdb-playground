#include "HttpUtils.h"

#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "glog/logging.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace beast_http = boost::beast::http;

namespace services {
namespace http {
namespace {

const std::unordered_map<std::string, std::string> kExtensionMap = {
    {".htm", "text/html"},         {".html", "text/html"},
    {".php", "text/html"},         {".css", "text/css"},
    {".txt", "text/plain"},        {".js", "application/javascript"},
    {".json", "application/json"}, {".swf", "application/x-shockwave-flash"},
    {".flv", "video/x-flv"},       {".png", "image/png"},
    {".jpe", "image/jpeg"},        {".jpeg", "image/jpeg"},
    {".jpg", "image/jpeg"},        {".gif", "image/gif"},
    {".bmp", "image/bmp"},         {".ico", "image/vnd.microsoft.icon"},
    {".tiff", "image/tiff"},       {".tif", "image/tiff"},
    {".svg", "image/svg+xml"},     {".svgz", "image/svg+xml"},
};

const std::string kDefaultType = "application/text";

const std::string kServerVersion = "cpp-project";

} // namespace

std::string HttpUtils::getMimeTypeForExtension(const std::string &path) {
  auto pos = path.rfind(".");
  if (pos == std::string::npos) {
    return kDefaultType;
  }
  const auto ext = path.substr(pos);
  auto iter = kExtensionMap.find(ext);
  if (iter != kExtensionMap.end()) {
    return iter->second;
  }
  return kDefaultType;
}

beast_http::response<beast_http::string_body>
HttpUtils::buildErrorResponse(std::shared_ptr<HttpSession> session,
                              beast_http::status status,
                              const std::string &reason) {
  beast_http::response<beast_http::string_body> res{
      status, session->getRequest().version()};
  res.set(beast_http::field::server, getServerVersion());
  res.set(beast_http::field::content_type, "text/html");
  res.keep_alive(session->getRequest().keep_alive());
  res.body() = reason;
  res.prepare_payload();
  return res;
}

beast_http::response<beast_http::string_body>
HttpUtils::buildJsonResponse(std::shared_ptr<HttpSession> session,
                             std::string &&json) {
  beast_http::response<beast_http::string_body> res{
      beast_http::status::ok, session->getRequest().version()};
  res.set(beast_http::field::server, getServerVersion());
  res.set(beast_http::field::content_type, getMimeTypeForExtension(".json"));
  res.keep_alive(session->getRequest().keep_alive());
  res.body() = std::move(json);
  res.prepare_payload();
  return res;
}

std::string HttpUtils::getServerVersion() { return kServerVersion; }

} // namespace http
} // namespace services
