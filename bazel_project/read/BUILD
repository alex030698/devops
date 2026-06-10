load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary", "cc_test")
cc_library(
    name = "read",
    srcs = ["src/read.cpp"],
    hdrs = ["src/read.h"],
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_test(
    name = "read_test",
    srcs = ["test/read_test.cpp"],
    deps = [
        ":read",
        "@googletest//:gtest_main",
    ],
)
