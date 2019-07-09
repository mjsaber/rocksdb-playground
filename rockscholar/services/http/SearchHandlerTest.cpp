#include "boost/asio.hpp"
#include "boost/beast/core.hpp"
#include "boost/beast/http.hpp"
#include "boost/beast/version.hpp"
#include "services/http/HttpServer.h"
#include "services/http/SearchHandler.h"
#include "services/http/SimpleHttpRouter.h"
#include "services/http/data_source/DataSource.h"
#include "services/http/recommender/Recommender.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;
using ::testing::Eq;
using ::testing::Return;
namespace http = boost::beast::http;

namespace {
int32_t kPort = 8765;
char kHost[] = "127.0.0.1";
char kPath[] = "/search";
int32_t kThreadNum = 2;

string postJson(const string &jsonData) {
  boost::asio::io_service ios;
  boost::asio::ip::tcp::resolver r(ios);
  boost::asio::ip::tcp::socket sock(ios);
  boost::asio::connect(sock, r.resolve(kHost, to_string(kPort)));

  http::request<http::string_body> req;
  req.method(http::verb::post);
  req.target(kPath);
  req.version(11);
  req.set(http::field::content_type, "application/json");
  req.set(http::field::content_length, to_string(jsonData.size()));
  req.set(http::field::user_agent, "boost/beast");
  req.body() = jsonData;
  http::write(sock, req);

  boost::beast::flat_buffer buffer;
  http::response<http::dynamic_body> resp;
  http::read(sock, buffer, resp);

  string res = boost::beast::buffers_to_string(resp.body().data());

  // Shutdown the socket.
  boost::system::error_code ec;
  sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

  return res;
}

class MockRecommender : public services::http::recommender::Recommender {
public:
  MOCK_METHOD1(recommend, vector<services::http::data_source::QueryResult>(
                              const string &key));
  /*
    return {{1, "number one", "I am one"},
            {3, "number three", "It was three"},
            {6, "#6", "That is six"}};
  }
  */
};

class WrappedRecommender : public services::http::recommender::Recommender {
private:
  MockRecommender *mockRecommender_;

public:
  void updateRecommender(MockRecommender *mockRecommender) {
    mockRecommender_ = mockRecommender;
  }

  void resetRecommender() { mockRecommender_ = nullptr; }

  vector<services::http::data_source::QueryResult>
  recommend(const string &key) {
    if (mockRecommender_) {
      return mockRecommender_->recommend(key);
    }
    return {};
  }
};

class SearchHandlerTest : public testing::Test {
protected:
  static unique_ptr<services::http::HttpServer> server_;
  static WrappedRecommender *wrappedRecommender_;

  static void SetUpTestCase() {
    server_ = make_unique<services::http::HttpServer>(kHost, kPort, kThreadNum);
    auto router = make_shared<services::http::SimpleHttpRouter>();
    auto recommender = make_unique<WrappedRecommender>();
    wrappedRecommender_ = recommender.get();

    router->post(kPath,
                 make_shared<services::http::SearchHandler>(move(recommender)));
    server_->setRouter(router);

    // Create thread to run server.
    thread th([]() { SearchHandlerTest::server_->serve(); });

    // Detach thread so that we don't need to keep `th`.
    th.detach();

    // Wait for server to start.
    sleep(1);
  }

  static void TearDownTestCase() {
    // Stop the server.
    server_->stop();
    server_.reset();

    // Allow sometime for all threads to be settled down.
    sleep(1);
  }
};

unique_ptr<services::http::HttpServer> SearchHandlerTest::server_;
WrappedRecommender *SearchHandlerTest::wrappedRecommender_ = nullptr;

TEST_F(SearchHandlerTest, TestInvalidInput) {
  EXPECT_EQ(postJson(""), "Invalid JSON body: ");
}

TEST_F(SearchHandlerTest, TestEmpty) {
  EXPECT_EQ(postJson("{\"search_key\": \"123\"}"), "[]");
}

TEST_F(SearchHandlerTest, TestSimple) {
  MockRecommender mockRecommender;
  EXPECT_CALL(mockRecommender, recommend("not-related"))
      .WillOnce(Return(vector<services::http::data_source::QueryResult>{
          {1, "number one", "I am one"},
          {3, "number three", "It was three"},
          {6, "#6", "That is six"}}));

  wrappedRecommender_->updateRecommender(&mockRecommender);

  EXPECT_EQ(postJson("{\"search_key\": \"not-related\"}"),
            "[{\"citation\":\"I am one\",\"title\":\"number "
            "one\"},{\"citation\":\"It was three\",\"title\":\"number "
            "three\"},{\"citation\":\"That is six\",\"title\":\"#6\"}]");

  wrappedRecommender_->resetRecommender();
}

} // namespace

/*
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
*/
