/*
 * Newton's Forward Difference Interpolation
 * -------------------------------------------
 * Builds a forward-difference table from n equally-spaced (x, y) data
 * points and uses it to estimate y at an interpolation point via
 * Newton's forward difference formula:
 *
 *   f(a) ~= y0 + u*D y0 + u(u-1)/2! * D^2 y0 + u(u-1)(u-2)/3! * D^3 y0 + ...
 *
 * where u = (a - x0) / h  and  D^k y0  is the k-th forward difference of
 * y0 (the top-left diagonal of the difference table).
 *
 * Supports up to MAX_POINTS data points (equally spaced, step size h).
 *
 * Build:  gcc -Wall -Wextra -std=c11 -o newton_forward newton_forward.c -lm
 * Run:    ./newton_forward
 */

#include <stdio.h>
#include <math.h>

#define MAX_POINTS 10

float forward_product(float u, int i);
int factorial(int k);

int main(void)
{
    float x[MAX_POINTS], y[MAX_POINTS], d[MAX_POINTS][MAX_POINTS];
    float a, u, h, tsum, sum = 0;
    int i, j, n;

    printf("Enter the number of pairs of values (2-%d)\t", MAX_POINTS);
    scanf("%d", &n);

    if (n < 2 || n > MAX_POINTS) {
        printf("Error: number of pairs must be between 2 and %d.\n", MAX_POINTS);
        return 1;
    }

    printf("Enter the initial value\t\t");
    scanf("%f", &x[0]);
    printf("Enter the common difference\t");
    scanf("%f", &h);

    /* Only n data points are needed: x[0] plus (n-1) more steps of h.
     * (The original version looped one iteration too far and wrote an
     * (n+1)-th value into a size-n array -- at n == MAX_POINTS that is
     * a stack buffer overflow. This loop produces exactly n values.) */
    for (i = 1; i < n; i++)
        x[i] = x[i - 1] + h;

    printf("Enter the value of y\n");
    for (i = 0; i < n; i++)
        scanf("%f", &y[i]);

    for (i = 0; i < n; i++)
        d[0][i] = y[i];

    printf("\nThe forward difference table is\n");
    for (i = 1; i < n; i++) {
        for (j = 0; j < n - i; j++) {
            d[i][j] = d[i - 1][j + 1] - d[i - 1][j];
            printf("d[%d][%d] = %f\t", i, j, d[i][j]);
        }
        printf("\n");
    }

    printf("\nEnter the point we want to interpolate:\t");
    scanf("%f", &a);

    u = (a - x[0]) / h;
    sum = 0;
    for (i = 1; i < n; i++)
        sum += (forward_product(u, i) * d[i][0]) / factorial(i);

    tsum = sum + y[0];
    printf("\nThe interpolated value at x = %f is %f\n", a, tsum);

    return 0;
}

/* u * (u-1) * (u-2) * ... * (u-i+1) -- the i-th term's u-product */
float forward_product(float u, int i)
{
    int l;
    float prod = u;
    for (l = 1; l < i; l++)
        prod = prod * (u - l);
    return prod;
}

int factorial(int k)
{
    int prod = 1, i;
    for (i = 1; i <= k; i++)
        prod = prod * i;
    return prod;
}
