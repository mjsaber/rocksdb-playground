package(default_visibility = ["//visibility:public"])

licenses(["notice"])

config_setting(
    name = "linux",
    values = {"cpu": "linux"},
)

config_setting(
    name = "macos",
    values = {"cpu": "darwin"},
)

genrule(
    name = "build_version",
    srcs = [
        "util/build_version.cc.in",
    ],
    outs = [
        "util/build_version.cc",
    ],
    cmd = "sed -e s/@@GIT_SHA@@/c60df9d9e7629fe208a9b848293a5599f83d5e77/ -e s/@@GIT_DATE_TIME@@/Apr\ 30,\ 2018/ " +
          "external/rocksdb_repo/util/build_version.cc.in >> $(@)",
)

cc_library(
    name = "rocksdb",
    hdrs = glob([
        "**/*.h",
    ]),
    srcs = glob(
        [
            "cache/**/*.cc",
            "env/**/*.cc",
            "memtable/**/*.cc",
            "monitoring/**/*.cc",
            "options/**/*.cc",
            "port/**/*.cc",
            "table/**/*.cc",
            "tools/**/*.cc",
            "util/**/*.cc",
            "utilities/**/*.cc",
        ],
        exclude = [
            "**/*_bench.cc",
            "**/*_test.cc",
            "**/*_test2.cc",
            "**/win/**/*.cc",
            "tools/rdb/*",
            "utilities/env_librados.cc",
            "**/*_stress.cc",
            "tools/dump/rocksdb_dump.cc",
            "tools/sst_dump.cc",
            "tools/ldb.cc",
            "tools/dump/rocksdb_undump.cc",
            "utilities/column_aware_encoding_exp.cc",
            "utilities/persistent_cache/hash_table_bench.cc",
        ],
    ) + [
        ":build_version",
    ],
    includes = [
        ".",
        "util",
    ],
    copts = [
        "-DROCKSDB_PLATFORM_POSIX",
        "-DROCKSDB_LIB_IO_POSIX",
        "-DROCKSDB_SUPPORT_THREAD_LOCAL",
        "-DGFLAGS=1",
    ] + select({
        ":macos": [
            "-DOS_MACOSX",
        ],
        ":linux": [
            "-DOS_LINUX",
        ],
        "//conditions:default": [
            "-DOS_LINUX",
        ],
    }),
    deps = [
        "//external:gtest",
    ],
    linkopts = [
        "-lm",
        "-lpthread",
    ],
)
