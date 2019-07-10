cc_binary(
    name = "http-server-main",
    srcs = ["HttpServerMain.cpp"],
    deps = [
        ":http_server",
        ":static_file_handler",
        ":search_handler",
        ":lambda_handler",
        ":health_check_handler",
        ":simple_http_router",
        "//services/http/recommender:plain_recommender",
        # TODO(HOMEWORK-4): ADD YOUR CODE HERE.
        "//services/http/data_source:rocksdb_data_source",
        "//external:gflags",
        "//external:glog",
        "//external:rocksdb",
    ],
)

cc_library(
    name = "http_session",
    srcs = [
        "HttpSession.cpp",
    ],
    hdrs = [
        "HttpRouter.h",
        "HttpRequestHandler.h",
        "HttpSession.h",
    ],
    deps = [
        "//external:boost",
        "//external:glog",
    ],
)

cc_library(
    name = "simple_http_router",
    srcs = [
        "SimpleHttpRouter.cpp",
    ],
    hdrs = [
        "SimpleHttpRouter.h",
    ],
    deps = [
        ":http_session",
        "//external:boost",
        "//external:glog",
    ],
)

cc_library(
    name = "static_file_handler",
    srcs = [
        "StaticFileHandler.cpp",
    ],
    hdrs = [
        "StaticFileHandler.h",
    ],
    deps = [
        ":http_session",
        ":http_utils",
        "//external:boost",
        "//external:glog",
    ],
)

cc_library(
    name = "search_handler",
    srcs = [
        "SearchHandler.cpp",
    ],
    hdrs = [
        "SearchHandler.h",
    ],
    deps = [
        ":http_session",
        ":http_utils",
        "//services/http/recommender:recommender_base",
        "//external:boost",
        "//external:glog",
        "//external:nlohmann_json",
    ],
)

cc_library(
    name = "lambda_handler",
    srcs = [
        "LambdaHandler.cpp",
    ],
    hdrs = [
        "LambdaHandler.h",
    ],
    deps = [
        ":http_session",
    ],
)

cc_library(
    name = "http_server",
    srcs = [
        "HttpServer.cpp",
    ],
    hdrs = [
        "HttpServer.h",
    ],
    deps = [
        ":http_session",
        "//external:boost",
        "//external:glog",
    ],
)

cc_library(
    name = "http_utils",
    srcs = [
        "HttpUtils.cpp",
    ],
    hdrs = [
        "HttpUtils.h",
    ],
    deps = [
        ":http_session",
        "//external:boost",
        "//external:glog",
    ],
)

cc_library(
    name = "health_check_handler",
    srcs = [
        "HealthCheckHandler.cpp",
    ],
    hdrs = [
        "HealthCheckHandler.h",
    ],
    deps = [
        ":http_session",
        ":http_utils",
        "//external:boost",
        "//external:glog",
        "//external:nlohmann_json",
    ],
)

cc_test(
    name = "search_handler_test",
    srcs = [
        "SearchHandlerTest.cpp",
    ],
    deps = [
        ":http_server",
        ":simple_http_router",
        ":search_handler",
        "//external:gtest",
        "//external:gtest_main",
        "//services/http/recommender:recommender_base",
        "//services/http/data_source:data_source_base",
    ],
)
