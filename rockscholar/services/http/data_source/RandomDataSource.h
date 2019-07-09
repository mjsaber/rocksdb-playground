#pragma once

#include "services/http/data_source/DataSource.h"

namespace services {
namespace http {
namespace data_source {
class RandomDataSource : public DataSource {
public:
  std::vector<QueryResult> query(const std::string &key) override;
};
} // namespace data_source
} // namespace http
} // namespace services
