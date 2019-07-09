#include "glog/logging.h"
#include "nlohmann/json.hpp"
#include "services/http/data_source/RocksDbUtils.h"

#include <unordered_set>

using json = nlohmann::json;

namespace services {
namespace http {
namespace data_source {
namespace {
const std::string kIndexPrefix = "index_";
const std::string kObjectPrefix = "obj_";

const std::string kId = "id";
const std::string kTitle = "title";
const std::string kCitation = "citation";
} // namespace

std::string RocksDbUtils::buildIndexKey(const std::string &key) {
  return kIndexPrefix + key;
}

std::string RocksDbUtils::buildObjectKey(uint32_t key) {
  return kObjectPrefix + std::to_string(key);
}

uint32_t RocksDbUtils::getObjectId(json obj) {
  return obj[kId].get<uint32_t>();
}

std::string RocksDbUtils::getObjectTitle(json obj) {
  return obj[kTitle].get<std::string>();
}

std::string RocksDbUtils::getObjectCitation(json obj) {
  return obj[kCitation].get<std::string>();
}

std::vector<QueryResult> RocksDbUtils::searchPrefix(rocksdb::DB *db,
                                                    const std::string &prefix) {
  std::vector<QueryResult> res;
  std::unordered_set<uint32_t> idSet;
  std::vector<std::string> builtIds;
  std::vector<rocksdb::Slice> builtIdSlices;
  auto builtPrefix = buildIndexKey(prefix);
  std::unique_ptr<rocksdb::Iterator> iter(db->NewIterator(rocksdb::ReadOptions()));
  for (iter->Seek(builtPrefix);
       iter->Valid() && iter->key().starts_with(builtPrefix); iter->Next()) {
    try {
      auto obj = json::parse(iter->value().ToString());
      auto arr = obj.get<std::vector<uint32_t>>();
      for (uint32_t id : arr) {
        if (idSet.find(id) == idSet.end()) {
          builtIds.push_back(buildObjectKey(id));
          idSet.insert(id);
        }
      }
    } catch (const std::exception &ex) {
      LOG(ERROR) << "Failed to parse obj: '" << iter->value().ToString()
                 << "' with exception: " << ex.what();
    }
  }
  for (const auto &cur : builtIds) {
    builtIdSlices.push_back(rocksdb::Slice(cur));
  }
  std::vector<std::string> values;
  values.reserve(builtIds.size());
  auto statuses = db->MultiGet(rocksdb::ReadOptions(), builtIdSlices, &values);
  for (size_t i = 0; i < statuses.size(); i++) {
    LOG(INFO) << "get_id: " << builtIdSlices[i].ToString();
    if (!statuses[i].ok()) {
      continue;
    }
    try {
      LOG(INFO) << "cur: " << values[i];
      auto obj = json::parse(values[i]);
      res.push_back({.id = getObjectId(obj),
                     .title = getObjectTitle(obj),
                     .citation = getObjectCitation(obj)});
    } catch (const std::exception &ex) {
      LOG(ERROR) << "Failed to parse value object: '" << values[i]
                 << "' with exception: " << ex.what();
    }
  }
  return res;
}

void RocksDbUtils::storeObject(rocksdb::DB *db, uint32_t id,
                               const std::string &data) {
  db->Put(rocksdb::WriteOptions(), buildObjectKey(id), data);
}

void RocksDbUtils::storeIndex(rocksdb::DB *db, const std::string &key,
                              const std::vector<uint32_t> &ids) {
  json list(ids);
  db->Put(rocksdb::WriteOptions(), buildIndexKey(key), list.dump());
}

} // namespace data_source
} // namespace http
} // namespace services
