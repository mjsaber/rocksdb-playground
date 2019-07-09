#pragma once

#include "boost/beast/http.hpp"
#include "services/http/HttpSession.h"
#include <string>

namespace services {
namespace http {
class HttpUtils {
public:
  static std::string getMimeTypeForExtension(const std::string &ext);

  static boost::beast::http::response<boost::beast::http::string_body>
  buildErrorResponse(std::shared_ptr<HttpSession> session,
                     boost::beast::http::status status,
                     const std::string &reason);

  static boost::beast::http::response<boost::beast::http::string_body>
  buildJsonResponse(std::shared_ptr<HttpSession> sesion, std::string &&json);

  static std::string getServerVersion();
};
} // namespace http
} // namespace services
