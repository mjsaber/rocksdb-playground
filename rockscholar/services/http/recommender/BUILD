package(default_visibility = ["//visibility:public"])

cc_library(
    name = "recommender_base",
    hdrs = [
        "Recommender.h",
    ],
    srcs = [
        "Recommender.cpp",
    ],
    deps = [
        "//services/http/data_source:data_source_base",
    ],
)

cc_library(
    name = "plain_recommender",
    hdrs = [
        "PlainRecommender.h",
    ],
    srcs = [
        "PlainRecommender.cpp"
    ],
    deps = [
        ":recommender_base",
    ],
)

cc_library(
    name = "lexical_recommender",
    # TODO(HOMEWORK-3): ADD YOUR CODE HERE.
)

cc_test(
    name = "lexical_recommender_test",
    srcs = [
        "LexicalRecommenderTest.cpp",
    ],
    deps = [
        ":lexical_recommender",
        "//external:gtest",
        "//external:gtest_main",
        "//external:glog",
    ],
)
