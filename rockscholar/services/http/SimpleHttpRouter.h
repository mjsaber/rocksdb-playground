#pragma once

#include "services/http/HttpRequestHandler.h"
#include "services/http/HttpSession.h"
#include "services/http/HttpRouter.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace services {
namespace http {
class SimpleHttpRouter : public HttpRouter {
public:
  void get(const std::string &path,
           std::shared_ptr<HttpRequestHandler> handler) override;

  void post(const std::string &path,
            std::shared_ptr<HttpRequestHandler> handler) override;

  void setFallback(std::shared_ptr<HttpRequestHandler> handler) override;

  void routeRequest(std::shared_ptr<HttpSession> session) override;

private:
  std::unordered_map<std::string, std::shared_ptr<HttpRequestHandler>> getHandlers_;
  std::unordered_map<std::string, std::shared_ptr<HttpRequestHandler>> postHandlers_;
  std::shared_ptr<HttpRequestHandler> defaultHandler_;
};
} // namespace http
} // namespace services
