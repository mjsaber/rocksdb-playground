#include "gflags/gflags.h"
#include "glog/logging.h"
#include "nlohmann/json.hpp"
#include "services/http/data_source/RocksDbUtils.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>
#include <set>

DEFINE_string(db_path, "/tmp/search_rocksdb", "RocksDb path");
DEFINE_string(json, "/tmp/data.json", "Json file that need to be loaded");

using json = nlohmann::json;
using namespace std;

std::string toLowerString(const std::string& str) {
    stringstream ss;
    for(char ch : str) {
        ss << (char)tolower(ch);
    }
    auto val = ss.str();
    return val;
}

std::vector<std::string> splitString(const std::string& str) {
    stringstream ss(str);
    std::vector<std::string> res;
    for(std::string line; std::getline(ss, line, ' ');) {
        if(line == "") {
            continue;
        }
        res.push_back(toLowerString(line));
    }
    return res;
}

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  LOG(INFO) << "Begin to load RocksDb " << FLAGS_db_path
            << " from file: " << FLAGS_json;
  rocksdb::DB *db = nullptr;
  rocksdb::Options options;
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, FLAGS_db_path, &db);
  CHECK(status.ok()) << "Failed to create rocksdb instance at "
                     << FLAGS_db_path;

  ifstream fs(FLAGS_json);
  unordered_map<string, set<uint32_t>> indexMap;
  for(std::string line; std::getline(fs, line);) {
      if(line == "") {
          continue;
      }
      json obj = json::parse(line);
      uint32_t id = services::http::data_source::RocksDbUtils::getObjectId(obj);
      string title = services::http::data_source::RocksDbUtils::getObjectTitle(obj);
      services::http::data_source::RocksDbUtils::storeObject(db, id, line);


      auto words = splitString(title);
      for(auto& word : words) {
          indexMap[word].insert(id);
      }
  }

  for(auto& p : indexMap) {
      vector<uint32_t> ids;
      for(auto val : p.second) {
          ids.push_back(val);
      }
      services::http::data_source::RocksDbUtils::storeIndex(db, p.first, ids);
  }
  LOG(INFO) << "Inserted " << indexMap.size() << " indices";

  LOG(INFO) << "Done with indexing";

  return 0;
}
