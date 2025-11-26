workspace(name = "counter")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# 本地的 GBBS 子模块
local_repository(
    name = "gbbs",
    path = "external/gbbs",
)

# parlaylib（GBBS 用的并行库）
git_repository(
    name = "parlaylib",
    remote = "https://github.com/ParAlg/parlaylib.git",
    commit = "352f5367a6b3bd2f0c6f05882cdd02e978e349ea",
    strip_prefix = "include/",
)

# googletest 1.11.0
http_archive(
    name = "googletest",
    urls = ["https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip"],
    strip_prefix = "googletest-release-1.11.0",
)

# abseil-cpp 20240116.2
http_archive(
    name = "abseil-cpp",
    urls = ["https://github.com/abseil/abseil-cpp/archive/refs/tags/20240116.2.tar.gz"],
    strip_prefix = "abseil-cpp-20240116.2",
)

# google benchmark 1.9.4
http_archive(
    name = "google_benchmark",
    urls = ["https://github.com/google/benchmark/archive/refs/tags/v1.9.4.tar.gz"],
    strip_prefix = "benchmark-1.9.4",
)
