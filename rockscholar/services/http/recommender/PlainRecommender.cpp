
#include "services/http/recommender/PlainRecommender.h"

namespace services {
namespace http {
namespace recommender {

std::vector<data_source::QueryResult> PlainRecommender::recommend(const std::string &key) {
  return getDataSource()->query(key);
}

} // namespace recommender
} // namespace http
} // namespace services
