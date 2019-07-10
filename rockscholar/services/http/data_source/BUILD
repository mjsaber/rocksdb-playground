package(default_visibility = ["//visibility:public"])

cc_library(
    name = "data_source_base",
    hdrs = [
        "DataSource.h",
    ],
    srcs = [
    ],
    deps = [
    ],
)

cc_library(
    name = "random_data_source",
    hdrs = [
        "RandomDataSource.h",
    ],
    srcs = [
        "RandomDataSource.cpp"
    ],
    deps = [
        ":data_source_base",
    ],
)

cc_library(
    name = "rocksdb_data_source",
    hdrs = [
        "RocksDbDataSource.h",
    ],
    srcs = [
        "RocksDbDataSource.cpp"
    ],
    deps = [
        ":data_source_base",
        "//external:rocksdb",
        "//external:glog",
    ],
)

cc_library(
    name = "rocksdb_utils",
    hdrs = [
        "RocksDbUtils.h",
    ],
    srcs = [
        "RocksDbUtils.cpp"
    ],
    deps = [
        ":data_source_base",
        "//external:rocksdb",
        "//external:nlohmann_json",
        "//external:glog",
    ],
)

cc_test(
    name = "rocksdb_data_source_test",
    data = ["//prebuilt:search_rocksdb_data"],
    srcs = [
        "RocksDbDataSourceTest.cpp",
    ],
    deps = [
        ":rocksdb_data_source",
        "//external:gtest",
        "//external:gtest_main",
        "//external:glog",
        "//external:rocksdb",
    ],
)
