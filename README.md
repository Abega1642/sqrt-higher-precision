# √ Higher Precision — Constructing Square Roots of Natural Numbers

> A high-precision C++ implementation of a novel rational sequence that approximates square roots of natural numbers, inspired by mathematical analysis and powered by GMP and MPFR.

---

## 📘 Overview

This project is a C++ translation and extension of the original [Java implementation](https://github.com/Abega1642/on-the-construction-of-real-numbers) that explores a new rational sequence for approximating square roots of natural numbers. The algorithm is rooted in real analysis and number theory, and is capable of computing square roots with up to **7 million digits of precision**.

The core idea is to construct a rational sequence that converges to √m for any natural number m, using a recursive formulation derived from metric space topology and inspired by Euler’s techniques.


---

## 🚀 Features

- Compute √m for any natural number m with arbitrary precision
- Uses rational arithmetic (fractions) to maintain exactness
- Powered by GMP and MPFR for high-performance big number computation
- Includes unit tests comparing results against NASA-verified constants
- Designed for extensibility and mathematical rigor

---

## 🛠️ Dependencies

This C++ version uses:

- [GMP](https://gmplib.org/): GNU Multiple Precision Arithmetic Library
- [MPFR](https://www.mpfr.org/): Multiple Precision Floating-Point Reliable Library
- [GoogleTest](https://github.com/google/googletest): for unit testing
- [vcpkg](https://github.com/microsoft/vcpkg): for dependency management

---

## 📦 Installation

### 1. Clone the Repository

```bash
git clone https://github.com/your-username/sqrt-higher-precision.git
cd sqrt-higher-precision
```

### 2. Install Dependencies via vcpkg

```bash
/path/to/vcpkg/vcpkg install gmp mpfr gtest
```

### 3. Build the Project

```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

---

## 🧪 Running Tests

```bash
ctest
```

Tests compare the computed square roots to high-precision reference values (up to 6 million digits) from NASA’s APOD numerical datasets.

---

## 📚 Background

This project originated from a topology course challenge: construct a rational sequence that converges to √2. The result evolved into a general-purpose algorithm for approximating any √m using only rational operations.

The original Java version is available here:  
👉 [on-the-construction-of-real-numbers](https://github.com/Abega1642/on-the-construction-of-real-numbers)

---


> Author: Abegà Razafindratelo
