#pragma once

#include "services/http/HttpRequestHandler.h"
#include <memory>

namespace services {
namespace http {
class HealthCheckHandler : public HttpRequestHandler {
public:
    void handle(std::shared_ptr<HttpSession> session) override;
};
} // namespace http
} // namespace services
