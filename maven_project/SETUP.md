# C++ Maven Project — Setup & Troubleshooting Documentation

## Project Overview

A multi-module C++ project built with Maven, consisting of 4 modules:

| Module | Type | Description |
|--------|------|-------------|
| `math` | Static library | Math operations: sum, subtract, multiply, divide |
| `read` | Static library | Console input: readLine, readInt, readDouble |
| `write` | Static library | Console output: writeLine, writeResult, writeError |
| `app` | Executable | Main application linking all 3 libraries |

Each module includes **Google Test (gtest)** unit tests.

---

## Step 1 — Initial Project Creation (NAR Plugin Approach)

### What was done

Created a multi-module Maven project using the `nar-maven-plugin` (Native ARchive) to compile C++ code:

- Parent `pom.xml` with `<packaging>pom</packaging>` and 4 child modules
- Each library module used `<packaging>nar</packaging>` with `nar-maven-plugin` (`com.github.maven-nar:nar-maven-plugin:3.10.1`)
- C++ source files placed under `src/main/c++/` and headers under `src/main/include/`
- The `app` module was configured as an executable depending on the 3 library modules

### Error encountered

```
Unresolveable build extension: Plugin com.github.maven-nar:nar-maven-plugin:3.10.1
or one of its dependencies could not be resolved:
Failed to read artifact descriptor for com.github.maven-nar:nar-maven-plugin:jar:3.10.1:
Could not transfer artifact ... from/to central (https://repo.maven.apache.org/maven2):
transfer failed ... Unknown host repo.maven.apache.org: Name or service not known
```

### Root cause

The build environment has **no internet access** — `repo.maven.apache.org` is unreachable. The `nar-maven-plugin` is not available in the local Maven repository or as a system package.

---

## Step 2 — Switching to exec-maven-plugin + Makefiles

### What was done

Replaced the `nar-maven-plugin` approach entirely:

1. Changed all module `<packaging>` from `nar` to `pom`
2. Replaced `nar-maven-plugin` with `exec-maven-plugin` (`org.codehaus.mojo:exec-maven-plugin:3.1.0`)
3. Created a `Makefile` in each module for actual C++ compilation
4. Configured `exec-maven-plugin` to invoke `make` at each lifecycle phase:
   - `compile` phase → `make all`
   - `test` phase → `make test`
   - `clean` phase → `make clean`

### Why exec-maven-plugin

The `exec-maven-plugin` was available as a system package (`libexec-maven-plugin-java`) and could be installed via `apt`:

```bash
sudo apt-get install -y libexec-maven-plugin-java
```

This installed:
- `libexec-maven-plugin-java` (3.1.0-2)
- `libcommons-exec-java` (1.3-2) — dependency

The plugin JARs and POMs were placed in `/usr/share/maven-repo/org/codehaus/mojo/exec-maven-plugin/`.

---

## Step 3 — Installing exec-maven-plugin into Maven's Local Repository

### Error encountered

```
Plugin org.codehaus.mojo:exec-maven-plugin:3.1.0 or one of its dependencies
could not be resolved: Cannot access central ... in offline mode
```

Even though the plugin was installed via `apt` into `/usr/share/maven-repo/`, Maven could not find it in its local repository (`~/.m2/repository/`).

### Fix

Manually copied the plugin and all its dependencies from the system Maven repo into `~/.m2/repository/`:

```bash
# exec-maven-plugin itself
mkdir -p ~/.m2/repository/org/codehaus/mojo/exec-maven-plugin/3.1.0
cp /usr/share/maven-repo/org/codehaus/mojo/exec-maven-plugin/3.1.0/exec-maven-plugin-3.1.0.{pom,jar} \
   ~/.m2/repository/org/codehaus/mojo/exec-maven-plugin/3.1.0/

# commons-exec (dependency)
mkdir -p ~/.m2/repository/org/apache/commons/commons-exec/1.3
cp /usr/share/maven-repo/org/apache/commons/commons-exec/1.3/commons-exec-1.3.{pom,jar} \
   ~/.m2/repository/org/apache/commons/commons-exec/1.3/

# commons-exec debian version
mkdir -p ~/.m2/repository/org/apache/commons/commons-exec/debian
cp /usr/share/maven-repo/org/apache/commons/commons-exec/debian/commons-exec-debian.{pom,jar} \
   ~/.m2/repository/org/apache/commons/commons-exec/debian/

# plexus-utils (debian version)
mkdir -p ~/.m2/repository/org/codehaus/plexus/plexus-utils/debian
cp /usr/share/maven-repo/org/codehaus/plexus/plexus-utils/debian/plexus-utils-debian.pom \
   ~/.m2/repository/org/codehaus/plexus/plexus-utils/debian/
cp /usr/share/java/plexus-utils.jar \
   ~/.m2/repository/org/codehaus/plexus/plexus-utils/debian/plexus-utils-debian.jar

# plexus-utils 1.1 (needed by maven-clean-plugin)
mkdir -p ~/.m2/repository/org/codehaus/plexus/plexus-utils/1.1
cp /usr/share/java/plexus-utils.jar \
   ~/.m2/repository/org/codehaus/plexus/plexus-utils/1.1/plexus-utils-1.1.jar
# + created a minimal POM

# plexus-component-annotations (1.5.5 and debian versions)
mkdir -p ~/.m2/repository/org/codehaus/plexus/plexus-component-annotations/{1.5.5,debian}
# Copied JARs and POMs from /usr/share/maven-repo and /usr/share/java
```

After this, `mvn -o compile` and `mvn -o test` worked.

---

## Step 4 — Creating Stub Maven Lifecycle Plugins

### Problem

Maven's default lifecycle binds several core plugins (`clean`, `install`, `site`, `deploy`) that were not available offline. Running `mvn -o clean` or `mvn -o install` failed.

### Solution

For each missing plugin, a **stub plugin JAR** was compiled and installed. The stub contains:
- A no-op Mojo class (compiled with javac)
- A `META-INF/maven/plugin.xml` descriptor
- A minimal POM with no dependencies

A JDK from a Bazel cache was used since `javac` was not on PATH:

```bash
JAVA_HOME=$(dirname $(dirname /home/qxz42vw/.cache/bazel/_bazel_qxz42vw/.../external/remotejdk21_linux/bin/javac))
JAVAC="$JAVA_HOME/bin/javac"
JAR="$JAVA_HOME/bin/jar"
```

### 4a. maven-clean-plugin:2.5

**Error:**
```
Plugin org.apache.maven.plugins:maven-clean-plugin:2.5 or one of its dependencies
could not be resolved: Cannot access central ... in offline mode
```

**Fix — created stub:**

```java
// org/apache/maven/plugin/clean/CleanMojo.java
package org.apache.maven.plugin.clean;
import org.apache.maven.plugin.AbstractMojo;
import org.apache.maven.plugin.MojoExecutionException;
public class CleanMojo extends AbstractMojo {
    public void execute() throws MojoExecutionException {
        getLog().info("Clean (no-op in C++ project - handled by make)");
    }
}
```

Compiled, packaged into JAR with `plugin.xml`, and installed to:
```
~/.m2/repository/org/apache/maven/plugins/maven-clean-plugin/2.5/
```

**Sub-error during this step:**
```
No plugin descriptor found at META-INF/maven/plugin.xml
```
Fixed by placing `plugin.xml` at `META-INF/maven/plugin.xml` (not under `META-INF/maven/plugin/`).

**Sub-error:**
```
Truncated class file
```
Fixed by compiling a real `.class` file with `javac` instead of creating an empty file.

### 4b. maven-install-plugin:2.4

**Error:**
```
Plugin org.apache.maven.plugins:maven-install-plugin:2.4 or one of its dependencies
could not be resolved
```

**Fix** — same approach: compiled `InstallMojo.java`, packaged with `plugin.xml`, installed to:
```
~/.m2/repository/org/apache/maven/plugins/maven-install-plugin/2.4/
```

### 4c. maven-site-plugin:3.3

**Error:**
```
Plugin org.apache.maven.plugins:maven-site-plugin:3.3 or one of its dependencies
could not be resolved
```

**Fix** — compiled `SiteMojo.java`, included both `site` and `attach-descriptor` goals in `plugin.xml`, installed to:
```
~/.m2/repository/org/apache/maven/plugins/maven-site-plugin/3.3/
```

### 4d. maven-deploy-plugin:2.7

**Error:**
```
Plugin org.apache.maven.plugins:maven-deploy-plugin:2.7 or one of its dependencies
could not be resolved
```

**Fix** — compiled `DeployMojo.java`, installed to:
```
~/.m2/repository/org/apache/maven/plugins/maven-deploy-plugin/2.7/
```

---

## Step 5 — Adding Unit Tests with Google Test

### What was done

Created gtest-based unit tests for each module:

| Module | Test File | Tests |
|--------|-----------|-------|
| math | `src/test/c++/math_test.cpp` | 15 tests (sum, subtract, multiply, divide + division by zero) |
| read | `src/test/c++/read_test.cpp` | 6 tests (readLine, readInt, readDouble with mocked stdin/stdout) |
| write | `src/test/c++/write_test.cpp` | 7 tests (writeLine, writeResult, writeError with captured output) |
| app | `src/test/c++/app_test.cpp` | 7 integration tests (all components together) |

**Total: 35 tests**

### Prerequisites

Google Test was already installed on the system:

```
libgtest-dev:amd64  1.14.0-1
```

Libraries located at:
- `/usr/lib/x86_64-linux-gnu/libgtest.a`
- `/usr/lib/x86_64-linux-gnu/libgtest_main.a`

---

## System Requirements Summary

### Already installed
| Package | Version | Purpose |
|---------|---------|---------|
| `g++` | 13.3.0 | C++ compiler |
| `make` | — | Build tool invoked by Maven |
| `maven` | 3.8.7 | Build orchestration |
| `java` (OpenJDK) | 21.0.11 | Maven runtime |
| `libgtest-dev` | 1.14.0-1 | Google Test framework |

### Installed during setup
| Package | Version | Purpose |
|---------|---------|---------|
| `libexec-maven-plugin-java` | 3.1.0-2 | Maven plugin to execute system commands |
| `libcommons-exec-java` | 1.3-2 | Dependency of exec-maven-plugin |

### Manually created stubs (in `~/.m2/repository/`)
| Plugin | Version | Reason |
|--------|---------|--------|
| `maven-clean-plugin` | 2.5 | No internet to download from Maven Central |
| `maven-install-plugin` | 2.4 | No internet to download from Maven Central |
| `maven-site-plugin` | 3.3 | No internet to download from Maven Central |
| `maven-deploy-plugin` | 2.7 | No internet to download from Maven Central |

---

## Build Commands

All commands require the `-o` (offline) flag since Maven Central is unreachable:

```bash
mvn -o compile          # Compile all modules
mvn -o test             # Compile + run all 35 gtest tests
mvn -o package          # Compile + test + package
mvn -o clean install    # Full clean build with install
mvn -o site             # Generate site (no-op stub)
mvn -o deploy           # Deploy (no-op stub)
```

---

## Compiler Selection

The compiler is configurable via the `cxx.compiler` Maven property (default: `g++`).

```bash
# Default (g++ 13.3.0)
mvn -o clean test

# GCC 9
mvn -o clean test -Dcxx.compiler=g++-9

# Clang 18
mvn -o clean test -Dcxx.compiler=clang++-18
```

The property is passed through to `make` as `CXX=<compiler>` in all module POMs.

### Installed compilers

| Compiler | Package | Version |
|----------|---------|---------|
| g++ | gcc/g++ (default) | 13.3.0 |
| g++-9 | gcc-9/g++-9 | 9.5.0 |
| clang++-18 | clang-18 | 18.1.3 |

---

## Final Project Structure

```
devops/
├── pom.xml                              # Parent POM (multi-module)
├── SETUP.md                             # This file
├── math/
│   ├── pom.xml
│   ├── Makefile
│   └── src/
│       ├── main/
│       │   ├── c++/math.cpp
│       │   └── include/math.h
│       └── test/c++/math_test.cpp       # 15 gtest tests
├── read/
│   ├── pom.xml
│   ├── Makefile
│   └── src/
│       ├── main/
│       │   ├── c++/read.cpp
│       │   └── include/read.h
│       └── test/c++/read_test.cpp       # 6 gtest tests
├── write/
│   ├── pom.xml
│   ├── Makefile
│   └── src/
│       ├── main/
│       │   ├── c++/write.cpp
│       │   └── include/write.h
│       └── test/c++/write_test.cpp      # 7 gtest tests
└── app/
    ├── pom.xml
    ├── Makefile
    └── src/
        ├── main/c++/main.cpp            # Executable
        └── test/c++/app_test.cpp        # 7 integration tests
```
