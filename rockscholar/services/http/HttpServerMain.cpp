#include "gflags/gflags.h"
#include "glog/logging.h"
#include "rocksdb/db.h"
#include "services/http/HttpServer.h"
#include "services/http/HttpUtils.h"
#include "services/http/LambdaHandler.h"
#include "services/http/SearchHandler.h"
#include "services/http/SimpleHttpRouter.h"
#include "services/http/StaticFileHandler.h"
#include "services/http/data_source/RocksDbDataSource.h"
#include "services/http/recommender/PlainRecommender.h"
// TODO(HOMEWORK-4): ADD YOUR CODE HERE.

#include <iostream>
#include <memory>

DEFINE_string(host, "0.0.0.0",
              "IP Address that HTTP server will be serving on");
DEFINE_int32(port, 8080, "HTTP port of the service");
DEFINE_int32(num_threads, 8, "Number of threads in HTTP server");
DEFINE_string(root_path, "asserts", "Doc root path to serve the static files");
DEFINE_string(db_path, "/tmp/search_rocksdb/",
              "RocksDb path for RocksDbDataSource");

using namespace std;
using services::http::HttpSession;
using services::http::HttpUtils;

std::shared_ptr<rocksdb::DB> createDb(const std::string &path) {
  rocksdb::DB *db = nullptr;
  rocksdb::Options options;
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, path, &db);
  CHECK(status.ok()) << "Failed to create rocksdb instance at " << path
                     << " with error: " << status.ToString();
  return std::shared_ptr<rocksdb::DB>(db);
}

int main(int argc, char **argv) {
  // print log to files
  // google::InitGoogleLogging(argv[0]);

  // initialize the gflags
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  LOG(INFO) << "Starting HTTP Server...";
  LOG(INFO) << "Listen on port: " << FLAGS_port;
  LOG(INFO) << "Doc Root Path: " << FLAGS_root_path;

  services::http::HttpServer srv(FLAGS_host, FLAGS_port, FLAGS_num_threads);

  auto db = createDb(FLAGS_db_path);

  auto router = std::make_shared<services::http::SimpleHttpRouter>();

  // Register handlers...

  // TODO(HOMEWORK-4): UPDATE THE FOLLOWING LINE AND FIX INCLUDE.
  auto recommender =
      std::make_unique<services::http::recommender::PlainRecommender>();

  recommender->setDataSource(
      std::make_unique<services::http::data_source::RocksDbDataSource>(db));

  router->post("/api/search", std::make_shared<services::http::SearchHandler>(
                                  std::move(recommender)));

  // create lambda-style request handler
  router->get("/test_lambda",
              std::make_shared<services::http::LambdaHandler>(
                  [](std::shared_ptr<HttpSession> session) {
                    // put some code in this lambda function to respond to the
                    // request
                    LOG(INFO) << "handling request in lambda function!";
                    session->respond(HttpUtils::buildJsonResponse(
                        session, "{\"source\":\"lambda\"}"));
                  }));

  router->setFallback(static_pointer_cast<services::http::HttpRequestHandler>(
      std::make_shared<services::http::StaticFileHandler>(FLAGS_root_path)));

  srv.setRouter(router);

  try {
    srv.serve();
  } catch (const std::exception &ex) {
    LOG(FATAL) << "HTTP server failed to serve: " << ex.what();
  }
  return 0;
}
