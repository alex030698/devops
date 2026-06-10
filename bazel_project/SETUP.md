# Bazel C++ Project — Setup & Documentation

## Project Overview

A multi-module C++ project built with Bazel 7.5.0, consisting of 4 packages:

| Package | Type | Description |
|---------|------|-------------|
| `//math` | `cc_library` (static) | Math operations: sum, subtract, multiply, divide |
| `//read` | `cc_library` (static) | Console input: readLine, readInt, readDouble |
| `//write` | `cc_library` (static) | Console output: writeLine, writeResult, writeError |
| `//app` | `cc_binary` (executable) | Main application linking all 3 libraries |

Each package includes **Google Test (gtest)** unit tests via `cc_test`.

---

## Prerequisites

| Tool | Version | Purpose |
|------|---------|---------|
| Bazel | 7.5.0 | Build system (pinned via `.bazelversion`) |
| g++ | 13.3.0 | Default C++ compiler |
| g++-9 | 9.5.0 | Optional: GCC 9 compiler |
| clang++-18 | 18.1.3 | Optional: Clang 18 compiler |
| libgtest-dev | 1.14.0 | Google Test framework (system-installed) |

---

## Project Structure

```
bazel_project/
├── .bazelversion                # Pins Bazel to 7.5.0
├── .bazelrc                     # Build configs (compiler selection)
├── MODULE.bazel                 # Bzlmod module definition
├── WORKSPACE                    # External deps (system gtest)
├── third_party/
│   ├── BUILD                    # Package marker
│   └── gtest.BUILD              # BUILD file for system libgtest
├── math/
│   ├── BUILD                    # cc_library + cc_test
│   ├── src/math.h               # Header
│   ├── src/math.cpp             # Implementation
│   └── test/math_test.cpp       # 15 gtest tests
├── read/
│   ├── BUILD
│   ├── src/read.h
│   ├── src/read.cpp
│   └── test/read_test.cpp       # 6 gtest tests
├── write/
│   ├── BUILD
│   ├── src/write.h
│   ├── src/write.cpp
│   └── test/write_test.cpp      # 7 gtest tests
└── app/
    ├── BUILD                    # cc_binary + cc_test
    ├── src/main.cpp
    └── test/app_test.cpp        # 7 integration tests
```

---

## Build Commands

```bash
# Build all targets
bazel build //...

# Run all tests (35 total)
bazel test //...

# Run the application
bazel run //app

# Build/test a specific module
bazel build //math
bazel test //math:math_test

# Clean build outputs
bazel clean
bazel clean --expunge    # Full clean including server
```

---

## Compiler Selection

Compilers are selected via the `CC` environment variable. Switching compilers requires
`bazel clean --expunge` since the C++ toolchain is configured at server startup.

```bash
# GCC 9
bazel clean --expunge
bazel test //... --config=gcc9

# Clang 18
bazel clean --expunge
bazel test //... --config=clang18

# Or via environment variable directly
CC=gcc-9    bazel test //...
CC=clang-18 bazel test //...
```

---

## Google Test Integration

System-installed gtest (`/usr/lib/x86_64-linux-gnu/libgtest.a`) is integrated via
`new_local_repository` in `WORKSPACE`, with a custom BUILD file at `third_party/gtest.BUILD`.

Test targets depend on `@googletest//:gtest_main` which provides:
- The gtest library headers and static library
- The `main()` function (via `gtest_main`)
- pthread linking

---

## Setup Steps

### 1. Bazel Installation

Bazel 7.5.0 is used via Bazelisk (`/usr/bin/bazel` → `bazelisk`). The binary is cached at:
```
~/.cache/bazelisk/downloads/sha256/.../bin/bazel
```

### 2. JAVA_HOME Conflict

**Error encountered:**
```
Error in symlink: java.io.IOException: Could not create symlink from
.../external/remotejdk21_linux/WORKSPACE to .../external/local_jdk/WORKSPACE:
(File exists)
```

**Cause:** `JAVA_HOME` was set to another project's Bazel-managed JDK external directory,
which contained a `WORKSPACE` file causing symlink conflicts.

**Fix:** Unset `JAVA_HOME` before running Bazel so it uses the system JDK:
```bash
unset JAVA_HOME
```

### 3. Custom Toolchain Approach (Failed)

Initially attempted custom `cc_toolchain_config` definitions using
`unix_cc_toolchain_config.bzl`. This failed because:

- Missing required `tool_paths` keys (`llvm-cov`, `objcopy`)
- After fixing those, system include paths were not configured, causing:
  ```
  absolute path inclusion(s) found in rule: '/usr/include/stdc-predef.h'
  ```

**Resolution:** Switched to the simpler `CC` environment variable approach which triggers
Bazel's auto-detection of system include paths, linker paths, and toolchain features.

### 4. Compiler Installation

```bash
sudo apt-get install -y gcc-9 g++-9 clang-18
```
