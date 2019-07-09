#pragma once

#include "services/http/recommender/Recommender.h"

namespace services {
namespace http {
namespace recommender {
class PlainRecommender : public Recommender {
public:
  std::vector<data_source::QueryResult> recommend(const std::string &key) override;
};
} // namespace recommender
} // namespace http
} // namespace services
