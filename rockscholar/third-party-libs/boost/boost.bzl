
def build_hdrs(all_pkgs):
    hdrs = []
    for pkg in all_pkgs:
        hdrs += [
            "boost/" + pkg + "/**/*.h",
            "boost/" + pkg + "/**/*.hpp",
            "boost/" + pkg + "/**/*.ipp",
            "libs/" + pkg + "/src/**/*.h",
            "libs/" + pkg + "/src/**/*.hpp",
            "libs/" + pkg + "/src/**/*.ipp",
        ]
    return hdrs

def build_srcs(all_pkgs):
    srcs = []
    for pkg in all_pkgs:
        srcs += [
            "libs/" + pkg + "/src/**/*.cpp",
            "libs/" + pkg + "/src/**/*.ipp",
            "libs/" + pkg + "/src/**/*.c",
        ]
    return srcs

def boost_library(name, all_pkgs = [], includes = [], hdrs = [], srcs = [], deps = [], copts = [], exclude = [], linkopts = []):
    return native.cc_library(
        name = name,
        visibility = ["//visibility:public"],
        includes = ["."] + includes,
        hdrs = native.glob(build_hdrs(all_pkgs)) + hdrs,
        srcs = native.glob(
            build_srcs(all_pkgs),
            exclude = [
                "**/**/untested.cpp",
                "**/windows/**/*.cpp",
                "**/win32/**/*.cpp",
            ] + exclude,
        ) + srcs,
        deps = deps,
        linkopts = [
            "-pthread",
        ] + linkopts,
        defines = [
            "BOOST_USE_UCONTEXT=1",
            "BOOST_COROUTINES_NO_DEPRECATION_WARNING=1",
            "BOOST_LOCALE_WITH_ICU=1",
        ],
        copts = copts,
    )
