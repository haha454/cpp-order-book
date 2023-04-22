load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "lib",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    visibility = [
        "//app:__pkg__",
        "//test:__pkg__",
    ],
)
