#include "services/http/data_source/RocksDbDataSource.h"
#include "services/http/data_source/RocksDbUtils.h"
#include "glog/logging.h"

namespace services {
namespace http {
namespace data_source {

RocksDbDataSource::RocksDbDataSource(std::shared_ptr<rocksdb::DB> instance) : db_(instance) {
  LOG(INFO) << "rocksdb open successfully";
}

std::vector<QueryResult> RocksDbDataSource::query(const std::string &key) {
  return services::http::data_source::RocksDbUtils::searchPrefix(db_.get(), key);
}

} // namespace data_source
} // namespace http
} // namespace services
