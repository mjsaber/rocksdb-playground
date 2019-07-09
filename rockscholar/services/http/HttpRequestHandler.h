#pragma once

#include "services/http/HttpSession.h"
#include <memory>

namespace services {
namespace http {
class HttpRequestHandler {
public:
  virtual void handle(std::shared_ptr<HttpSession> session) = 0;

  virtual ~HttpRequestHandler() {}
};
} // namespace http
} // namespace services
