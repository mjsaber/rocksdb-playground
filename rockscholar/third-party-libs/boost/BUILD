package(default_visibility = ["//visibility:public"])

licenses(["notice"])

load("@cpp_project//third-party-libs/boost:boost.bzl", "boost_library")

config_setting(
    name = "linux_x86_64",
    constraint_values = [
        "@bazel_tools//platforms:linux",
        "@bazel_tools//platforms:x86_64",
    ],
)

config_setting(
    name = "osx_x86_64",
    constraint_values = [
        "@bazel_tools//platforms:osx",
        "@bazel_tools//platforms:x86_64",
    ],
)

basic_pkgs = [
    "archive",
    "context",
    "winapi",
    "config",
    "predef",
    "assert",
    "bind",
    "core",
    "static_assert",
    "endian",
    "utility",
    "type_traits",
    "throw_exception",
    "exception",
    "preprocessor",
    "move",
    "intrusive",
    "container_hash",
    "container",
    "integer",
    "detail",
    "optional",
    "smart_ptr",
    "mpl",
    "array",
    "function",
    "functional",
    "ratio",
    "typeof",
    "range",
    "algorithm",
    "io",
    "lexical_cast",
    "numeric",
    "tokenizer",
    "type_index",
    "concept_check",
    "concept",
    "iterator",
    "tuple",
    "unordered",
    "pool",
    "conversion",
    "function_types",
    "fusion",
    "rational",
    "spirit",
    "variant",
    "lambda",
    "foreach",
    "phoenix",
    "proto",
    "tti",
    "any",
    "math",
]

BOOST_CTX_ASM_SOURCES = select({
    ":linux_x86_64": [
        "libs/context/src/asm/jump_x86_64_sysv_elf_gas.S",
        "libs/context/src/asm/make_x86_64_sysv_elf_gas.S",
        "libs/context/src/asm/ontop_x86_64_sysv_elf_gas.S",
    ],
    ":osx_x86_64": [
        "libs/context/src/asm/jump_x86_64_sysv_macho_gas.S",
        "libs/context/src/asm/make_x86_64_sysv_macho_gas.S",
        "libs/context/src/asm/ontop_x86_64_sysv_macho_gas.S",
    ],
})

boost_library(
    name = "boost_basic",
    all_pkgs = basic_pkgs,
    hdrs = glob([
        "boost/*.hpp",
        "boost/*.h",
    ]) + [
        "libs/container/src/dlmalloc_2_8_6.c",
        "libs/container/src/dlmalloc_ext_2_8_6.c",
    ],
    deps = [],
    includes = [
        "libs/math/src/tr1/",
        "libs/container/src/",
    ],
    srcs = BOOST_CTX_ASM_SOURCES,
    exclude = [
        "libs/container/src/dlmalloc_2_8_6.c",
        "libs/container/src/dlmalloc_ext_2_8_6.c",
    ],
)

boost_library(
    name = "boost_asio",
    all_pkgs = ["asio"],
    deps = [
        ":boost_basic",
        ":boost_date_time",
        ":boost_regex",
    ],
)

boost_library(
    name = "boost_beast",
    all_pkgs = ["beast"],
    deps = [
        ":boost_basic",
        ":boost_asio",
        ":boost_system",
    ],
)

boost_library(
    name = "boost_thread",
    all_pkgs = ["thread"],
    hdrs = [
         "libs/thread/src/pthread/once_atomic.cpp",
    ],
    deps = [
        ":boost_basic",
        ":boost_atomic",
        ":boost_chrono",
        ":boost_date_time",
        ":boost_system",
    ],
    exclude = [
        "libs/thread/src/pthread/once_atomic.cpp",
    ],
)

boost_library(
    name = "boost_regex",
    all_pkgs = ["regex"],
    deps = [
        ":boost_basic",
    ],
)

boost_library(
    name = "boost_system",
    all_pkgs = ["system"],
    deps = [
        ":boost_basic",
    ],
)

boost_library(
    name = "boost_filesystem",
    all_pkgs = ["filesystem"],
    deps = [
        ":boost_basic",
        ":boost_system",
    ],
    copts = [
        "-Wno-deprecated-declarations",
    ],
)

#boost_library(
#    name = "boost_program_options",
#    all_pkgs = ["program_options"],
#    deps = [
#        ":boost_basic",
#    ],
#)

boost_library(
    name = "boost_random",
    all_pkgs = ["random"],
    deps = [
        ":boost_basic",
        ":boost_system",
    ],
)

boost_library(
    name = "boost_atomic",
    all_pkgs = ["atomic"],
    deps = [
        ":boost_basic",
    ],
)

boost_library(
    name = "boost_chrono",
    all_pkgs = ["chrono"],
    deps = [
        ":boost_basic",
        ":boost_system",
    ],
)

boost_library(
    name = "boost_coroutine",
    all_pkgs = ["coroutine"],
    deps = [
        ":boost_basic",
        ":boost_system",
        ":boost_thread",
    ],
)

boost_library(
    name = "boost_date_time",
    all_pkgs = ["date_time"],
    deps = [
        ":boost_basic",
    ],
)

boost_library(
    name = "boost_iostreams",
    all_pkgs = ["iostreams"],
    deps = [
        ":boost_basic",
        ":boost_random",
        ":boost_regex",
    ],
    linkopts = [
        "-lbz2",
        "-lz",
        "-llzma",
    ],
)

boost_library(
    name = "boost_locale",
    all_pkgs = ["locale"],
    deps = [
        ":boost_basic",
        ":boost_thread",
    ],
    copts = [
        "-Wno-deprecated-declarations",
        "-DBOOST_LOCALE_NO_WINAPI_BACKEND",
    ],
    linkopts = [
        "-licuio",
        "-licui18n",
        "-licuuc",
        "-licudata",
    ],
)

boost_library(
    name = "boost_serialization",
    all_pkgs = ["serialization"],
    deps = [
        ":boost_basic",
    ],
)

cc_library(
    name = "boost",
    deps = [
        ":boost_basic",
        ":boost_serialization",
        ":boost_asio",
        ":boost_beast",
        ":boost_locale",
        ":boost_iostreams",
        ":boost_thread",
        ":boost_regex",
        ":boost_system",
        ":boost_filesystem",
#        ":boost_program_options",
        ":boost_random",
        ":boost_atomic",
        ":boost_chrono",
        ":boost_coroutine",
        ":boost_date_time",
    ],
)

