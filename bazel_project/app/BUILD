load("@rules_cc//cc:defs.bzl", "cc_library", "cc_binary", "cc_test")

cc_binary(
    name = "app",
    srcs = ["src/main.cpp"],
    deps = [
        "//mathx",
        "//read",
        "//write",
    ],
)

cc_test(
    name = "app_test",
    srcs = ["test/app_test.cpp"],
    deps = [
        "//mathx",
        "//read",
        "//write",
        "@googletest//:gtest_main",
    ],
)
