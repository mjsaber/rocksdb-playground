#pragma once

#include "services/http/HttpRequestHandler.h"

namespace services {
namespace http {
class StaticFileHandler : public HttpRequestHandler {
public:
  StaticFileHandler(const std::string &rootPath);

  void handle(std::shared_ptr<HttpSession> session) override;

private:
  const std::string rootPath_;
};
} // namespace http
} // namespace services
