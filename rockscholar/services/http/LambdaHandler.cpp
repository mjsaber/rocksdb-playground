#include "services/http/LambdaHandler.h"

namespace services {
namespace http {

LambdaHandler::LambdaHandler(std::function<void(std::shared_ptr<HttpSession>)> &&func)
    : func_(std::move(func)) {}

void LambdaHandler::handle(std::shared_ptr<HttpSession> session) {
  func_(session);
}
} // namespace http
} // namespace services
