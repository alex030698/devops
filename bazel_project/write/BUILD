
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary", "cc_test")

cc_library(
    name = "write",
    srcs = ["src/write.cpp"],
    hdrs = ["src/write.h"],
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_test(
    name = "write_test",
    srcs = ["test/write_test.cpp"],
    deps = [
        ":write",
        "@googletest//:gtest_main",
    ],
)
