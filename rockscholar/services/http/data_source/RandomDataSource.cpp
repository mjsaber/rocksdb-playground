
#include "services/http/data_source/RandomDataSource.h"

namespace services {
namespace http {
namespace data_source {

std::vector<QueryResult> RandomDataSource::query(const std::string &key) {
  std::vector<QueryResult> res;
  for (size_t i = 0; i < key.size(); i++) {
    res.push_back({.title = key + "_k" + std::to_string(i),
                   .citation = "v" + std::to_string(i)});
  }
  return res;
}

} // namespace data_source
} // namespace http
} // namespace services
