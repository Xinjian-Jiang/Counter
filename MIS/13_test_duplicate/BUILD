licenses(["notice"])

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "MIS",
    hdrs = ["MIS.h"],
    srcs = ["MIS.cc"], 
    deps = [
        "@gbbs//gbbs",
        "//include:counters",
    ],
)

cc_binary(
    name = "MIS_main",
    srcs = ["MIS.cc"], 
    deps = [":MIS"],
)

