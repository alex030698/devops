load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary", "cc_test")
cc_library(
    name = "mathx",
    srcs = ["src/mathx.cpp"],
    hdrs = ["src/mathx.h"],
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
)

cc_test(
    name = "mathx_test",
    srcs = ["test/mathx_test.cpp"],
    deps = [
        ":mathx",
        "@googletest//:gtest_main",
    ],
)
