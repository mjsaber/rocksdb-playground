#include "glog/logging.h"
#include "services/http/recommender/LexicalRecommender.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <algorithm>
#include <string>
#include <vector>

using namespace std;
using namespace services::http::data_source;
using ::testing::Eq;
using ::testing::Return;

namespace {

class MockDataSource : public DataSource {
public:
  MOCK_METHOD1(query, vector<QueryResult>(const std::string &key));
};

TEST(LexicalRecommenderTest, testSorting) {
  const string queryKey("just_for_test");
  QueryResult a = {0, "A Formula For All K-Gonal Numbers that Are Centered",
                   "Jawad Sadek, Russell Euler"},
              b = {0, "Restricted Sum Formula of Multiple Zeta Values",
                   "Haiping Yuan, Jianqiang Zhao"},
              c = {0, "McMahon's Formula via Free Fermions", "John Mangual"},
              d = {0,
                   "Regularization based on all-at-once formulations for "
                   "inverse problems",
                   "Barbara Kaltenbacher"};
  unique_ptr<MockDataSource> mockDataSource = make_unique<MockDataSource>();
  MockDataSource *mockDataSourcePtr = mockDataSource.get();
  services::http::recommender::LexicalRecommender lexicalRecommender;
  lexicalRecommender.setDataSource(move(mockDataSource));

  EXPECT_CALL(*mockDataSourcePtr, query(Eq(queryKey)))
      .WillOnce(Return(vector<QueryResult>{a, b, c, d}));

  vector<QueryResult> results = lexicalRecommender.recommend(queryKey);
  vector<QueryResult> expected = {a, b, c, d};

  sort(expected.begin(), expected.end(),
       [](const QueryResult &a, const QueryResult &b) {
         return a.title < b.title;
       });

  EXPECT_EQ(results.size(), expected.size());
  for (size_t i = 0; i < results.size(); i++) {
    EXPECT_EQ(results[i].title, expected[i].title);
    EXPECT_EQ(results[i].citation, expected[i].citation);
  }
}

} // namespace
