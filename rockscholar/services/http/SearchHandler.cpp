#include "services/http/SearchHandler.h"

#include "boost/beast/http.hpp"
#include "glog/logging.h"
#include "nlohmann/json.hpp"
#include "services/http/HttpUtils.h"
#include <sstream>
#include <unordered_map>
#include <vector>

namespace services {
namespace http {

SearchHandler::SearchHandler(
    std::unique_ptr<recommender::Recommender> recommender)
    : recommender_(std::move(recommender)) {}

void SearchHandler::handle(std::shared_ptr<HttpSession> session) {
  // TODO(HOMEWORK-1): ADD YOUR CODE HERE.
}
} // namespace http
} // namespace services
