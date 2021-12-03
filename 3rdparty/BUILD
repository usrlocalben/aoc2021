load("//:copts.bzl", "RQDQ_DEFAULT_COPTS")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = 'backward',
    hdrs = ["backward-cpp/backward.hpp",],
    includes = ["backward-cpp"],
    srcs = ["backward-cpp/backward.cpp",],
    alwayslink = True,
    copts = RQDQ_DEFAULT_COPTS,)

cc_library(
    name = 'fast_io',
    hdrs = glob(["fast_io/include/*.h", "fast_io/include/**/*.h"]),
    includes = ["fast_io/include"],
    srcs=[],
    copts = RQDQ_DEFAULT_COPTS,)
