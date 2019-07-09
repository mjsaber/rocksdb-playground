#pragma once

#include "nlohmann/json.hpp"
#include "rocksdb/db.h"
#include "services/http/data_source/DataSource.h"

#include <string>
#include <vector>

namespace services {
namespace http {
namespace data_source {
class RocksDbUtils {
public:
  static std::string buildIndexKey(const std::string &key);

  static std::string buildObjectKey(uint32_t key);

  static std::vector<QueryResult> searchPrefix(rocksdb::DB *db,
                                               const std::string &prefix);

  static uint32_t getObjectId(nlohmann::json obj);

  static std::string getObjectTitle(nlohmann::json obj);

  static std::string getObjectCitation(nlohmann::json obj);

  static void storeObject(rocksdb::DB *db, uint32_t id,
                          const std::string &data);

  static void storeIndex(rocksdb::DB *db, const std::string &key,
                         const std::vector<uint32_t> &ids);
};
} // namespace data_source
} // namespace http
} // namespace services
