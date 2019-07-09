#pragma once

#include <string>
#include <vector>

namespace services {
namespace http {
namespace data_source {

struct QueryResult {
  uint32_t id;
  std::string title;
  std::string citation;
};

class DataSource {
public:
  virtual std::vector<QueryResult> query(const std::string &key) = 0;

  virtual ~DataSource() {}
};
} // namespace data_source
} // namespace http
} // namespace services
