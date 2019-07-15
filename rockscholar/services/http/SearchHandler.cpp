#include "services/http/SearchHandler.h"

#include "boost/beast/http.hpp"
#include "boost/algorithm/string.hpp"
#include "glog/logging.h"
#include "nlohmann/json.hpp"
#include "services/http/HttpUtils.h"
#include <sstream>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

namespace services {
namespace http {

SearchHandler::SearchHandler(
    std::unique_ptr<recommender::Recommender> recommender)
    : recommender_(std::move(recommender)) {}

void SearchHandler::handle(std::shared_ptr<HttpSession> session) {
  auto& body = session->getRequest().body();
  json request_body;
  try {
      request_body = json::parse(body);
  } catch(json::parse_error) {
      session->respond(HttpUtils::buildJsonResponse(session, "Invalid JSON body: " + body));
      return;
  }
  std::string search_value = request_body["search_key"].get<std::string>();
  if (search_value == "null") {
      session->respond(HttpUtils::buildErrorResponse(session, boost::beast::http::status::bad_request, "Invalid JSON body: " + body));
      return;
  }
  boost::algorithm::to_lower(search_value);
  auto result = recommender_->recommend(search_value);
  json jsonResult(result);

  session->respond(HttpUtils::buildJsonResponse(session, jsonResult.dump()));
}

namespace data_source {
    void to_json(json& j, const QueryResult& queryResult) {
        j = json{{"title", queryResult.title}, {"citation", queryResult.citation}};
    };
} // namespace data_source
} // namespace http
} // namespace services

