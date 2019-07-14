#pragma once

#include "services/http/recommender/Recommender.h"

namespace services {
namespace http {
namespace recommender {
class LexicalRecommender : public Recommender {
public:
  std::vector<data_source::QueryResult> recommend(const std::string &key) override;
};
} // namespace recommender
} // namespace http
} // namespace services