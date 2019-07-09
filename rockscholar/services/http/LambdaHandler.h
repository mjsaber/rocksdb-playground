#pragma once

#include "services/http/HttpRequestHandler.h"
#include <memory>
#include <functional>

namespace services {
namespace http {
class LambdaHandler : public HttpRequestHandler {
public:
  LambdaHandler(std::function<void(std::shared_ptr<HttpSession>)>&&);
  void handle(std::shared_ptr<HttpSession> session) override;

private:
  std::function<void(std::shared_ptr<HttpSession>)> func_;
};
} // namespace http
} // namespace services
