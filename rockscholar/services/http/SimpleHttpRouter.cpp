#include "SimpleHttpRouter.h"

#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "glog/logging.h"
#include <string>

namespace services {
namespace http {

void SimpleHttpRouter::get(const std::string &path,
                           std::shared_ptr<HttpRequestHandler> handler) {
  LOG(INFO) << "Register new path for GET: " << path;
  getHandlers_[path] = handler;
}

void SimpleHttpRouter::post(const std::string &path,
                            std::shared_ptr<HttpRequestHandler> handler) {
  LOG(INFO) << "Register new path for POST: " << path;
  postHandlers_[path] = handler;
}

void SimpleHttpRouter::setFallback(
    std::shared_ptr<HttpRequestHandler> handler) {
  LOG(INFO) << "Register fallback handler";
  defaultHandler_ = handler;
}

void SimpleHttpRouter::routeRequest(std::shared_ptr<HttpSession> session) {
  LOG(INFO) << "Routing request...";
  const auto method = session->getRequest().method();
  const auto fullTarget = session->getRequest().target().to_string();
  std::string target = fullTarget;
  auto pos = fullTarget.find("?");
  if (pos != std::string::npos) {
    target.resize(pos);
  }
  if (method == boost::beast::http::verb::get) {
    auto iter = getHandlers_.find(target);
    LOG(INFO) << "Trying to route a get request: " << target;
    if (iter != getHandlers_.end()) {
      LOG(INFO) << "Routing to get handler: " << target;
      iter->second->handle(session);
      return;
    }
  } else if (method == boost::beast::http::verb::post) {
    auto iter = postHandlers_.find(target);
    if (iter != postHandlers_.end()) {
      LOG(INFO) << "Routing to post handler: " << target;
      iter->second->handle(session);
      return;
    }
  }
  LOG(INFO) << "Routing to default handler...";
  defaultHandler_->handle(session);
}

} // namespace http
} // namespace services
