# Newton's Forward Difference Interpolation

A C implementation of Newton's forward difference interpolation formula: builds a forward-difference table from equally-spaced data points and uses it to estimate the value of a function at a point that wasn't directly measured.

## Overview

Given a set of `n` data points `(x0, y0), (x1, y1), ..., (xn-1, yn-1)` with equally spaced `x` values (step size `h`), Newton's forward difference formula estimates `y` at any point `a` near the start of the range by building a forward-difference table and evaluating a polynomial through it. This is a classic numerical analysis technique for interpolation and light extrapolation on tabulated data.

## The Math

With `u = (a - x0) / h`, the interpolated value is:

```
f(a) ~= y0 + u*Dy0 + u(u-1)/2! * D^2y0 + u(u-1)(u-2)/3! * D^3y0 + ...
```

where `D^k y0` (the k-th forward difference of `y0`) is read off the top-left diagonal of the forward-difference table:

```
D^0 y0 = y0
D^k y[i] = D^(k-1) y[i+1] - D^(k-1) y[i]
```

## A bug found and fixed in the original version

The original version had a stack buffer overflow. Arrays were declared for a maximum of 10 points (`x[10]`, `y[10]`, `d[10][10]`), but the loop that filled in the x-values ran one iteration too far:

```c
for (i = 0; i < n; i++)
    x[i + 1] = x[i] + h;   // writes x[1] ... x[n] -- that's n writes, n+1 total values
```

For `n` data points you only need `n` x-values (`x[0]` through `x[n-1]`), but this loop produces `n+1` of them. At `n = 10` — the exact maximum the arrays were sized for — this wrote to `x[10]`, one past the end of a 10-element array. Compiling and running the original code with AddressSanitizer confirms it:

```
==ERROR: AddressSanitizer: stack-buffer-overflow ... in main
    [80, 120) 'x' (line 5) <== Memory access at offset 120 overflows this variable
SUMMARY: AddressSanitizer: stack-buffer-overflow Newton_Forward.c:16 in main
```

The fixed version's loop starts from `i = 1` and fills `x[i] = x[i-1] + h`, producing exactly `n` values:

```c
for (i = 1; i < n; i++)
    x[i] = x[i - 1] + h;
```

Re-running the `n = 10` case under AddressSanitizer after the fix completes cleanly with no overflow. The fix is also paired with explicit input validation (`n` must be between 2 and `MAX_POINTS`) so an out-of-range count is rejected with a clear error instead of corrupting the stack.

Two smaller cleanups: the original `main()` had an implicit `int` return type (a compiler warning under `-Wall`, and rejected outright by some strict/older compilers), and the helper functions were prototyped inside `main()` instead of at file scope. Both are fixed — the program now compiles with zero warnings under `-Wall -Wextra -std=c11`.

## Correctness check

Newton's forward formula is exact when the underlying data comes from a polynomial of degree less than `n`. As a sanity check, interpolating `y = x^3` at 5 points (`x = 0..4`) and evaluating at `x = 2.5` should recover `2.5^3 = 15.625` exactly:

```
Input:  n=5, x0=0, h=1, y = 0 1 8 27 64, interpolate at 2.5
Output: The interpolated value at x = 2.500000 is 15.625000
```

It does, to 6 decimal places — confirming the difference table and the interpolation loop are both implemented correctly.

## Sample run

A classic textbook example (population interpolation): given population figures at 10-year census intervals, estimate the population in a year that falls between two censuses.

```
Enter the number of pairs of values (2-10): 5
Enter the initial value: 1891
Enter the common difference: 10
Enter the value of y: 46 66 81 93 101

The forward difference table is
d[1][0] = 20.000000  d[1][1] = 15.000000  d[1][2] = 12.000000  d[1][3] = 8.000000
d[2][0] = -5.000000  d[2][1] = -3.000000  d[2][2] = -4.000000
d[3][0] = 2.000000   d[3][1] = -1.000000
d[4][0] = -3.000000

Enter the point we want to interpolate: 1895

The interpolated value at x = 1895.000000 is 54.852798
```

i.e. given census populations of 46, 66, 81, 93, and 101 (thousands) in 1891, 1901, 1911, 1921, and 1931, the estimated population in 1895 is about **54,853**.

## Tech Stack

- C (C11)
- `<math.h>` (linked but not required by the current formula — kept for straightforward extension to functions like `sin`/`cos` sample data)

## Project Structure

```
Newton-s-Forward-difference-formula/
├── src/
│   └── newton_forward.c
└── README.md
```

## How to Build and Run

```bash
git clone https://github.com/Akash-Nion/Newton-s-Forward-difference-formula.git
cd Newton-s-Forward-difference-formula/src

gcc -Wall -Wextra -std=c11 -o newton_forward newton_forward.c -lm
./newton_forward
```

You'll be prompted for: the number of data points (2-10), the starting x-value, the common difference `h` between x-values, the `y` value at each point, and finally the point you want to interpolate.

## Author

**Akash Nion Rahaman**
B.Sc. in Mathematics · Postgraduate Diploma in Data Science
