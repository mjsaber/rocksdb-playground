workspace(name="cpp_project")

load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

git_repository(
    name = "com_github_gflags_gflags",
    remote = "https://github.com/gflags/gflags.git",
    commit = "e292e0452fcfd5a8ae055b59052fc041cbab4abf",
)

bind(
    name = "gflags",
    actual = "@com_github_gflags_gflags//:gflags",
)

git_repository(
    name = "com_github_glog",
    remote = "https://github.com/google/glog.git",
    commit = "2faa186e62d544e930305ffd8f8e507b2054cc9b",
)

bind(
    name = "glog",
    actual ="@com_github_glog//:glog",
)

git_repository(
    name = "com_google_googletest",
    remote = "https://github.com/google/googletest.git",
    commit = "145d05750b15324899473340c8dd5af50d125d33",
)

bind(
    name = "gtest",
    actual ="@com_google_googletest//:gtest",
)

bind(
    name = "gtest_main",
    actual ="@com_google_googletest//:gtest_main",
)

http_archive(
    name = "boost_repo",
    strip_prefix = "boost_1_68_0",
    url = "https://iweb.dl.sourceforge.net/project/boost/boost/1.68.0/boost_1_68_0.tar.bz2",
    build_file = "//third-party-libs/boost:BUILD",
)

bind(
    name = "boost",
    actual = "@boost_repo//:boost"
)

http_archive(
    name = "nlohmann_json_repo",
    strip_prefix = "json-3.1.2",
    url = "https://github.com/nlohmann/json/archive/v3.1.2.zip",
    build_file = "//third-party-libs/nlohmann:BUILD",
)

bind(
    name = "nlohmann_json",
    actual = "@nlohmann_json_repo//:nlohmann_json"
)

http_archive(
    name = "rocksdb_repo",
    strip_prefix = "rocksdb-5.13.1",
    url = "https://github.com/facebook/rocksdb/archive/v5.13.1.tar.gz",
    build_file = "//third-party-libs/rocksdb:BUILD",
)

bind(
    name = "rocksdb",
    actual = "@rocksdb_repo//:rocksdb"
)

