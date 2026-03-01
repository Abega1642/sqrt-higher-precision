# Higher-Precision Square Root Construction

**Algorithmic implementation of a rational convergent series for computing square roots of natural numbers to arbitrary precision.**

This repository contains the C++ implementation of a novel rational series whose partial sums converge to $\sqrt{m}$ for any natural number $m$. The mathematical foundation — including the formal proof of convergence and the derivation of the recurrence — is documented in a companion paper:

> [on-square-root-construction-article](https://github.com/Abega1642/on-square-root-construction-article) *(currently private; public release forthcoming)*

The implementation is capable of producing verified decimal expansions of square roots up to **10 million digits of precision**, validated against the high-precision reference values published by NASA's Astronomy Picture of the Day numerical archive.

---

## Table of Contents

1. [Mathematical Background](#1-mathematical-background)
2. [The Series](#2-the-series)
3. [Algorithmic Structure](#3-algorithmic-structure)
4. [Validation Against NASA Reference Data](#4-validation-against-nasa-reference-data)
5. [Performance](#5-performance)
6. [Dependencies](#6-dependencies)
7. [Build Instructions](#7-build-instructions)
8. [Running the Tests](#8-running-the-tests)
9. [Relation to the Java Predecessor](#9-relation-to-the-java-predecessor)
10. [Author](#10-author)

---

## 1. Mathematical Background

The genesis of this work lies in a classical result from real analysis: the closure of $\mathbb{Q}$ in $\mathbb{R}$ under the standard metric topology, expressed as

$$\overline{\mathbb{Q}} = \mathbb{R}$$

This states that every real number is an accumulation point of the rationals. In particular, every irrational number $\alpha \in \mathbb{R} \setminus \mathbb{Q}$ is the limit of some sequence $(x_n)_{n \geq 0} \subset \mathbb{Q}$.

During a course on the topology of vector spaces, Professor Dr. Christalin posed the following constructive challenge: exhibit explicitly a rational sequence converging to $\sqrt{2}$. The standard responses — Newton-Raphson iteration, Heron's method, or continued fraction convergents — are well known. This work takes a different route.

While performing manual decimal computations of $\sqrt{2}$, $\sqrt{3}$, and $\sqrt{5}$, a structural pattern appeared in the partial sums. That pattern was formalized into a general series $(x_n^{(m)})_{n \geq 0}$ depending on the natural number $m$, whose limit is $\sqrt{m}$. The construction uses only rational arithmetic at every step, making it amenable to exact computation via arbitrary-precision integer arithmetic.

The formal proof that $x_n^{(m)} \to \sqrt{m}$ as $n \to \infty$, together with the derivation and analysis of the recurrence relation, is contained in the companion article referenced above.

---

## 2. The Series

Let $m \in \mathbb{N}$ be a non-perfect-square natural number, and let $r = \lceil \sqrt{m} \rceil$. Define the deviation $\delta = r^2 - m > 0$.

The series $\left(x_k\right)_{k \geq 0}$ is defined by a rational recurrence whose $k$-th term is computed from the two preceding terms via the relation

$$x_{k+1} = x_k \cdot \left( \left(\frac{x_k}{x_{k-1}}\right)^2 - 2 \right)$$

with the partial sum accumulating the negated reciprocals of successive terms:

$$S_k = r - \frac{1}{x_0} - \frac{1}{x_1} - \cdots - \frac{1}{x_k}$$

The series satisfies $S_k \to \sqrt{m}$ as $k \to \infty$. Each term $x_k$ and each partial sum $S_k$ is a rational number; no floating-point arithmetic is involved in the construction of the approximation.

The seed terms $x_0$ and $x_1$ are computed directly from $r$, $m$, and $\delta$ using closed-form rational expressions derived in the companion article.

---

## 3. Algorithmic Structure

The implementation is organized around two classes.

**`Fraction`** provides exact rational arithmetic over arbitrary-precision integers using GMP (`mpz_class`). All four arithmetic operations, as well as inversion and negation, are implemented with Knuth-style cross-reduction to minimize the size of intermediate numerators and denominators. The `simplify()` operation (GCD reduction) is applied at each recurrence step to prevent unbounded coefficient growth across iterations. Decimal expansion to a requested number of digits is produced via MPFR at the final output step only.

**`SquareRoot`** encodes the mathematical parameters $(m, r, \delta)$ and exposes the method `k_th_value(k)`, which returns the rational partial sum $S_k$ as a `Fraction`. The recurrence is computed iteratively with scratch-space GMP variables declared outside the loop to eliminate per-iteration heap allocation.

The method `Fraction::get_value(d)` converts the final rational result to a decimal string with $d$ digits using MPFR at precision $\lceil d \cdot \log_2 10 \rceil + 128$ bits.

---

## 4. Validation Against NASA Reference Data

The correctness of the computed values is verified against the high-precision decimal expansions of square roots published by Robert Nemiroff and Jerry Bonnell at:

> https://apod.nasa.gov/htmltest/gifcity/

The reference files used are:

| Value | Source |
|-------|--------|
| $\sqrt{2}$ | https://apod.nasa.gov/htmltest/gifcity/sqrt2.1mil |
| $\sqrt{3}$ | https://apod.nasa.gov/htmltest/gifcity/sqrt3.1mil |
| $\sqrt{5}$ | https://apod.nasa.gov/htmltest/gifcity/sqrt5.1mil |
| $\sqrt{6}$ | https://apod.nasa.gov/htmltest/gifcity/sqrt6.1mil |
| $\sqrt{7}$ | https://apod.nasa.gov/htmltest/gifcity/sqrt7.1mil |

The test suite in `tests/test_sqrt.cpp` computes $S_k$ for each target value, converts to decimal, and performs a character-by-character comparison against the corresponding reference file for up to 10 million digits. All comparisons pass at the digit counts reported in the performance section below.

---

## 5. Performance

The following measurements were obtained by running `k_th_value(k)` followed by `get_value(d)` on a single locked CPU core (performance governor, turbo disabled, FIFO scheduling) to minimize environmental variance. The reported duration is the minimum observed over repeated warm runs.

| $m$ | $k$ | Precision (digits) | Duration (s) |
|-----|-----|--------------------|--------------|
| 2   | 22  | 6,000,002          | 3.03634      |
| 3   | 19  | 1,000,002          | 0.360016     |
| 5   | 19  | 1,000,002          | 0.323097     |
| 7   | 19  | 1,000,002          | 0.370041     |
| 2   | 24  | 10,000,002         | 8.60041      |

The parameter $k$ denotes the index of the last term included in the partial sum $S_k$. The relationship between $k$ and the number of correct decimal digits is super-linear: each additional iteration roughly doubles the number of accurate digits, consistent with the convergence rate established in the companion article.

The dominant computational cost at large $k$ is the GCD computation required to reduce the rational partial sum, which operates on operands whose size grows proportionally to the target precision. GMP's sub-quadratic Lehmer GCD algorithm is used implicitly for large operands.

To reproduce these measurements:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
sudo .shell/bench-marks.sh ./build/tests
```

---

## 6. Dependencies

| Library | Purpose |
|---------|---------|
| [GMP](https://gmplib.org/) | Arbitrary-precision integer arithmetic (`mpz_class`) |
| [MPFR](https://www.mpfr.org/) | Arbitrary-precision floating-point for decimal output |
| [GoogleTest](https://github.com/google/googletest) | Unit and benchmark test framework |

GMP and MPFR are system libraries installed via the package manager. GoogleTest is fetched automatically by CMake at configure time via `FetchContent`.

```bash
# Debian / Ubuntu
sudo apt-get install libgmp-dev libmpfr-dev pkg-config cmake g++
```

---

## 7. Build Instructions

```bash
git clone https://github.com/Abega1642/sqrt-higher-precision.git
cd sqrt-higher-precision

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

---

## 8. Running the Tests

The test suite covers three categories: correctness of rational arithmetic (`FractionCorrectnessTest`), correctness of the square root approximation against NASA reference data (`SquareRootTest`), and computational performance benchmarks (`SquareRootBenchmark`).

```bash
# Run all suites with per-suite output
.shell/run-tests.sh

# Run benchmarks only, with CPU environment locked for reproducibility
sudo .shell/bench-marks.sh ./build/tests
```

The script `.shell/run-tests.sh` runs each GoogleTest suite individually, producing full per-test output. The script `.shell/bench-marks.sh` requires root in order to set the CPU frequency governor and disable turbo boost; it restores all system settings unconditionally on exit.

---

## 9. Relation to the Java Predecessor

An earlier implementation of this algorithm exists in Java:

> [on-the-construction-of-real-numbers](https://github.com/Abega1642/on-the-construction-of-real-numbers)

That version also used GMP via JNI bindings for big-integer arithmetic. The migration to C++ was motivated by the elimination of JVM overhead and JNI boundary costs, which were significant relative to the cost of individual GMP operations at high precision. The C++ implementation gives direct access to `mpz_class` and `mpfr_t`, resulting in substantially lower overhead per arithmetic operation and better cache utilization for large operands.

The mathematical algorithm is identical between both versions.

---

## 10. Author

**Abegà Razafindratelo**
Mathematics student and Computer Science engineering student

- Email: [a.razafindratelo@gmail.com](mailto:a.razafindratelo@gmail.com)
- GitHub: [https://github.com/Abega1642](https://github.com/Abega1642)
- LinkedIn: Abegà Razafindratelo