#include "services/http/recommender/LexicalRecommender.h"

#include <algorithm>

namespace services {
namespace http {
namespace recommender {

std::vector<data_source::QueryResult> LexicalRecommender::recommend(const std::string &key) {
    auto result = getDataSource()->query(key);

    sort(result.begin(), result.end(),
         [](const data_source::QueryResult &a, const data_source::QueryResult &b) {
             return a.title < b.title;
         });
    return result;
}

} // namespace recommender
} // namespace http
} // namespace services
