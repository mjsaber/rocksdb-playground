#pragma once

#include "services/http/HttpRequestHandler.h"
#include "services/http/recommender/Recommender.h"
#include <memory>

namespace services {
namespace http {
class SearchHandler : public HttpRequestHandler {
public:
  SearchHandler(std::unique_ptr<recommender::Recommender> recommender);
  void handle(std::shared_ptr<HttpSession> session) override;

private:
  std::unique_ptr<recommender::Recommender> recommender_;
};
} // namespace http
} // namespace services
