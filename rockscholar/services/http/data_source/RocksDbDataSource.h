#pragma once

#include "services/http/data_source/DataSource.h"
#include "rocksdb/db.h"
#include <memory>

namespace services {
namespace http {
namespace data_source {
class RocksDbDataSource : public DataSource {
public:
  RocksDbDataSource(std::shared_ptr<rocksdb::DB> db);

  std::vector<QueryResult> query(const std::string &key) override;

private:
  std::shared_ptr<rocksdb::DB> db_;
};
} // namespace data_source
} // namespace http
} // namespace services
