#pragma once

#include <memory>

namespace services {
namespace http {

class HttpRequestHandler;
class HttpSession;

class HttpRouter {
public:
  virtual void get(const std::string &path,
                   std::shared_ptr<HttpRequestHandler> handler) = 0;

  virtual void post(const std::string &path,
                    std::shared_ptr<HttpRequestHandler> handler) = 0;

  virtual void setFallback(std::shared_ptr<HttpRequestHandler> handler) = 0;

  virtual void routeRequest(std::shared_ptr<HttpSession> session) = 0;

  virtual ~HttpRouter() {}
};
} // namespace http
} // namespace services
