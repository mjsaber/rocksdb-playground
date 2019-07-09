#include "glog/logging.h"
#include "rocksdb/db.h"
#include "services/http/data_source/RocksDbDataSource.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace std;
using services::http::data_source::QueryResult;
using services::http::data_source::RocksDbDataSource;

namespace {
const char kDbPath[] = "prebuilt/search_rocksdb";
const char kQueryString[] = "formula";
} // namespace

TEST(RocksDbDataSourceTest, testDataSource) {
  rocksdb::DB *db = nullptr;
  rocksdb::Options options;
  rocksdb::Status status =
      rocksdb::DB::OpenForReadOnly(options, kDbPath, &db, false);
  EXPECT_TRUE(status.ok());
  std::shared_ptr<rocksdb::DB> pDb(db);

  RocksDbDataSource rocksDb(pDb);
  auto results = rocksDb.query(kQueryString);
  QueryResult a = {0, "A Formula For All K-Gonal Numbers that Are Centered",
                   "Jawad Sadek, Russell Euler"},
              b = {0, "Restricted Sum Formula of Multiple Zeta Values",
                   "Haiping Yuan, Jianqiang Zhao"},
              c = {0, "McMahon's Formula via Free Fermions", "John Mangual"},
              d = {0,
                   "Regularization based on all-at-once formulations for "
                   "inverse problems",
                   "Barbara Kaltenbacher"};
  vector<QueryResult> expected = {a, b, c, d};

  auto cmp = [](const QueryResult &a, const QueryResult &b) {
    return a.title < b.title;
  };

  sort(results.begin(), results.end(), cmp);
  sort(expected.begin(), expected.end(), cmp);

  EXPECT_EQ(results.size(), expected.size());
  for (size_t i = 0; i < results.size(); i++) {
    EXPECT_EQ(results[i].title, expected[i].title);
    EXPECT_EQ(results[i].citation, expected[i].citation);
  }
}
