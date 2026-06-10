# Maven vs Bazel — C++ Project Comparison

Both `maven_project/` and `bazel_project/` implement the same C++ application with
identical source code, 3 library components (math, read, write), 1 executable (app),
and 35 Google Test unit tests.

---

## Project Structure Comparison

```
maven_project/                          bazel_project/
├── pom.xml              (parent)       ├── MODULE.bazel
├── SETUP.md                            ├── WORKSPACE
│                                       ├── .bazelversion
│                                       ├── .bazelrc
│                                       ├── SETUP.md
│                                       ├── third_party/
│                                       │   ├── BUILD
│                                       │   └── gtest.BUILD
├── math/                               ├── math/
│   ├── pom.xml                         │   ├── BUILD
│   ├── Makefile                        │   ├── src/math.h
│   └── src/                            │   ├── src/math.cpp
│       ├── main/c++/math.cpp           │   └── test/math_test.cpp
│       ├── main/include/math.h         │
│       └── test/c++/math_test.cpp      │
├── read/                               ├── read/
│   ├── pom.xml                         │   ├── BUILD
│   ├── Makefile                        │   ├── src/read.h
│   └── src/...                         │   ├── src/read.cpp
│                                       │   └── test/read_test.cpp
├── write/                              ├── write/
│   ├── pom.xml                         │   ├── BUILD
│   ├── Makefile                        │   ├── src/write.h
│   └── src/...                         │   ├── src/write.cpp
│                                       │   └── test/write_test.cpp
└── app/                                └── app/
    ├── pom.xml                             ├── BUILD
    ├── Makefile                            ├── src/main.cpp
    └── src/...                             └── test/app_test.cpp
```

### File count

| Metric | Maven | Bazel |
|--------|-------|-------|
| Config files per module | 2 (pom.xml + Makefile) | 1 (BUILD) |
| Root config files | 1 (pom.xml) | 4 (MODULE.bazel, WORKSPACE, .bazelrc, .bazelversion) |
| External dep config | 0 (system linker flags) | 2 (third_party/BUILD + gtest.BUILD) |
| **Total config files** | **9** | **10** |
| Source files | 12 | 12 (identical) |
| Directory depth for sources | 4 levels (src/main/c++/) | 2 levels (src/) |

---

## Build System Comparison

| Aspect | Maven | Bazel |
|--------|-------|-------|
| **Native C++ support** | No — requires plugin (`exec-maven-plugin` + Makefiles) | Yes — built-in `cc_library`, `cc_binary`, `cc_test` |
| **Build language** | XML (pom.xml) + Makefile | Starlark (BUILD files) |
| **Dependency declaration** | Implicit (Makefile `-L` and `-l` flags) | Explicit (`deps = ["//math"]`) |
| **Test framework** | Manual gtest linking in Makefile | `cc_test` with `@googletest//:gtest_main` dep |
| **Parallelism** | Sequential module builds (reactor order) | Automatic fine-grained parallelism |
| **Incremental builds** | Makefile-level (per module) | File-level (per source file) |
| **Caching** | None (rebuild from scratch) | Content-addressable cache |
| **Sandboxing** | None | Linux sandbox per action |
| **Reproducibility** | Low (depends on system state) | High (hermetic builds) |

---

## Build Commands Comparison

| Action | Maven | Bazel |
|--------|-------|-------|
| Build all | `mvn -o compile` | `bazel build //...` |
| Run tests | `mvn -o test` | `bazel test //...` |
| Clean | `mvn -o clean` | `bazel clean` |
| Full rebuild | `mvn -o clean install` | `bazel clean && bazel build //...` |
| Build one module | `mvn -o compile -pl math` | `bazel build //math` |
| Run one test | `mvn -o test -pl math` | `bazel test //math:math_test` |
| Use GCC 9 | `mvn -o test -Dcxx.compiler=g++-9` | `bazel test //... --config=gcc9` |
| Use Clang 18 | `mvn -o test -Dcxx.compiler=clang++-18` | `bazel test //... --config=clang18` |

---

## Compiler Switching Comparison

### Maven

Configured via a Maven property `cxx.compiler` (default: `g++`), passed to `make CXX=<value>`:

```bash
mvn -o clean test -Dcxx.compiler=g++-9
mvn -o clean test -Dcxx.compiler=clang++-18
```

No restart or cache clearing needed — each `clean test` rebuilds everything.

### Bazel

Configured via `--config=` flags in `.bazelrc`, which set `CC` environment variable:

```bash
bazel clean --expunge
bazel test //... --config=gcc9

bazel clean --expunge
bazel test //... --config=clang18
```

**Requires `bazel clean --expunge`** when switching compilers because the C++ toolchain
is auto-configured at Bazel server startup and cached.

---

## Performance Comparison

| Metric | Maven | Bazel |
|--------|-------|-------|
| **Cold build** (clean + compile) | ~1.5s | ~6-8s (includes server start) |
| **Incremental build** (no changes) | ~0.5s | ~0.2s |
| **Test execution** | ~4s (sequential) | ~0.5s (parallel) |
| **Compiler switch overhead** | None (just rebuild) | ~6s (expunge + server restart) |

> Note: Bazel has higher cold-start cost due to JVM server startup, but wins on
> incremental builds and test parallelism.

---

## Offline / Air-gapped Environment

### Maven — High friction

- Maven tries to download plugins from Maven Central for every lifecycle phase
- Required **4 hand-compiled stub plugins** (clean, install, site, deploy)
- Required manual installation of `exec-maven-plugin` + dependencies into `~/.m2/repository/`
- Must use `-o` (offline) flag on every command
- No native C++ support — relies on `exec-maven-plugin` calling `make`

### Bazel — Low friction

- Bazel binary cached locally by Bazelisk
- System gtest integrated via `new_local_repository` (no download needed)
- No additional plugins or stubs required
- Just needed to unset `JAVA_HOME` to avoid a symlink conflict

---

## Dependency Graph

Both projects have the same logical dependency structure:

```
app ──┬── math
      ├── read
      └── write
```

### Maven representation (implicit)

Dependencies are expressed in Makefiles via `-L` and `-l` flags. Maven reactor ordering
is defined by `<modules>` order in the parent POM. There is no Maven-level dependency
declaration between C++ modules.

### Bazel representation (explicit)

```python
# app/BUILD
cc_binary(
    name = "app",
    deps = ["//math", "//read", "//write"],
)
```

Bazel enforces dependency visibility, prevents circular dependencies, and only rebuilds
what's affected by changes.

---

## Pros & Cons Summary

### Maven

| Pros | Cons |
|------|------|
| Familiar to Java developers | No native C++ support |
| Rich ecosystem for JVM projects | XML verbosity |
| Standardized lifecycle phases | Requires Makefiles as a secondary build system |
| | Offline setup is extremely painful |
| | No incremental build intelligence |
| | Sequential module compilation |

### Bazel

| Pros | Cons |
|------|------|
| Native C++ support (`cc_*` rules) | Steeper learning curve |
| Fine-grained dependency tracking | JVM server startup cost |
| Parallel & incremental builds | Compiler switching requires server restart |
| Sandboxed, reproducible builds | More config files at project root |
| Works well offline with local deps | Starlark syntax less familiar |
| Built-in test runner with caching | |

---

## Recommendation

- **Use Bazel** for C++ projects — it has native support, better incremental builds,
  test parallelism, and far simpler offline setup.
- **Use Maven** only if the C++ project is part of a larger JVM-centric ecosystem
  where Maven is already the standard build tool.
