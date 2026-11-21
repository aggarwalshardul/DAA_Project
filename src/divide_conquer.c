/**
 * @file divide_conquer.c
 * @brief Implementation of Divide and Conquer algorithms.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#include "divide_conquer.h"

/* -------------------------------------------------------------
 * 1. MAXIMUM SUBARRAY (DIVIDE & CONQUER)
 * ------------------------------------------------------------- */

/**
 * Finds the maximum crossing subarray that crosses the midpoint.
 * Time: Theta(n), Space: O(1).
 */
static MaxSubarrayResult max_crossing_subarray(const int arr[], int low, int mid, int high) {
    MaxSubarrayResult res;
    int left_sum = -INF_DIST;
    int sum = 0;
    res.low = mid;

    for (int i = mid; i >= low; i--) {
        ops_increment(1);
        sum += arr[i];
        if (sum > left_sum) {
            left_sum = sum;
            res.low = i;
        }
    }

    int right_sum = -INF_DIST;
    sum = 0;
    res.high = mid + 1;
    for (int j = mid + 1; j <= high; j++) {
        ops_increment(1);
        sum += arr[j];
        if (sum > right_sum) {
            right_sum = sum;
            res.high = j;
        }
    }

    res.sum = left_sum + right_sum;
    return res;
}

/**
 * Finds the contiguous subarray with maximum sum using Divide & Conquer.
 * Recurrence: T(n) = 2T(n/2) + Theta(n) -> Theta(n log n).
 * Space: O(log n) call stack.
 */
MaxSubarrayResult max_subarray_dc(const int arr[], int low, int high) {
    ops_increment(1);
    if (low == high) {
        MaxSubarrayResult base;
        base.low = low;
        base.high = high;
        base.sum = arr[low];
        return base;
    }

    int mid = low + (high - low) / 2;
    MaxSubarrayResult left_res  = max_subarray_dc(arr, low, mid);
    MaxSubarrayResult right_res = max_subarray_dc(arr, mid + 1, high);
    MaxSubarrayResult cross_res = max_crossing_subarray(arr, low, mid, high);

    if (left_res.sum >= right_res.sum && left_res.sum >= cross_res.sum) {
        return left_res;
    } else if (right_res.sum >= left_res.sum && right_res.sum >= cross_res.sum) {
        return right_res;
    } else {
        return cross_res;
    }
}

/**
 * Brute force maximum subarray for verification.
 * Time: Theta(n^2), Space: O(1).
 */
MaxSubarrayResult max_subarray_brute(const int arr[], int n) {
    MaxSubarrayResult best;
    best.low = 0; best.high = 0; best.sum = -INF_DIST;
    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = i; j < n; j++) {
            ops_increment(1);
            sum += arr[j];
            if (sum > best.sum) {
                best.sum = sum;
                best.low = i;
                best.high = j;
            }
        }
    }
    return best;
}

/* -------------------------------------------------------------
 * 2. FAST EXPONENTIATION VS NAIVE LOOP
 * ------------------------------------------------------------- */

/**
 * Computes base^exp in Theta(log exp) time using binary exponentiation.
 * Recurrence: T(n) = T(n/2) + Theta(1) -> Theta(log n).
 * Space: O(1) iterative.
 */
double fast_power(double base, int exp) {
    if (exp == 0) return 1.0;
    if (exp < 0) {
        base = 1.0 / base;
        exp = -exp;
    }

    double result = 1.0;
    double current = base;

    while (exp > 0) {
        ops_increment(1);
        if (exp % 2 == 1) {
            result *= current;
            ops_increment(1);
        }
        current *= current;
        ops_increment(1);
        exp /= 2;
    }
    return result;
}

/**
 * Computes base^exp using a naive linear loop in Theta(exp) time.
 * Time: Theta(n), Space: O(1).
 */
double naive_power(double base, int exp) {
    if (exp == 0) return 1.0;
    if (exp < 0) {
        base = 1.0 / base;
        exp = -exp;
    }
    double result = 1.0;
    for (int i = 0; i < exp; i++) {
        ops_increment(1);
        result *= base;
    }
    return result;
}

/**
 * Modular fast exponentiation (base^exp mod m) for cryptographic/CRT checks.
 * Time: Theta(log exp), Space: O(1).
 */
long long modular_power(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        ops_increment(1);
        if (exp % 2 == 1) {
            res = (res * base) % mod;
            ops_increment(1);
        }
        base = (base * base) % mod;
        ops_increment(1);
        exp /= 2;
    }
    return res;
}

/* -------------------------------------------------------------
 * 3. CELEBRITY / HUB IDENTIFICATION
 * ------------------------------------------------------------- */

/**
 * Finds a Hub Station in Theta(n) time using elimination.
 * A hub is known/reachable by all (in-degree = n-1) and reaches none (out-degree = 0).
 * Time: Theta(n), Space: O(1).
 */
int celebrity_hub_linear(int n, int reach[MAX_NODES][MAX_NODES]) {
    int candidate = 0;
    for (int i = 1; i < n; i++) {
        ops_increment(1);
        /* If candidate can reach i, candidate cannot be the hub; i might be */
        if (reach[candidate][i] == 1) {
            candidate = i;
        }
    }

    /* Verification step: candidate must be reached by all, and reach none */
    for (int i = 0; i < n; i++) {
        if (i != candidate) {
            ops_increment(2);
            if (reach[candidate][i] == 1 || reach[i][candidate] == 0) {
                return -1; /* No hub exists */
            }
        }
    }
    return candidate;
}

/**
 * Brute force celebrity check for testing and comparison.
 * Time: Theta(n^2), Space: O(1).
 */
int celebrity_hub_brute(int n, int reach[MAX_NODES][MAX_NODES]) {
    for (int i = 0; i < n; i++) {
        int in_deg = 0;
        int out_deg = 0;
        for (int j = 0; j < n; j++) {
            ops_increment(2);
            if (i != j) {
                if (reach[j][i] == 1) in_deg++;
                if (reach[i][j] == 1) out_deg++;
            }
        }
        if (in_deg == n - 1 && out_deg == 0) {
            return i;
        }
    }
    return -1;
}

/* -------------------------------------------------------------
 * 4. MATRIX MULTIPLICATION: CLASSICAL VS STRASSEN
 * ------------------------------------------------------------- */

/**
 * Classical matrix multiplication: C = A * B.
 * Time: Theta(n^3), Space: O(1) auxiliary.
 */
void matrix_multiply_classical(int n, const int A[MATRIX_MAX][MATRIX_MAX],
                               const int B[MATRIX_MAX][MATRIX_MAX],
                               int C[MATRIX_MAX][MATRIX_MAX]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                ops_increment(2);
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/**
 * Strassen matrix multiplication sub-matrices helper.
 * Computes 7 recursive multiplications for 2x2 blocks or switches to classical.
 * Recurrence: T(n) = 7 T(n/2) + Theta(n^2) -> Theta(n^log2(7)) ~= Theta(n^2.81).
 * Time: O(n^2.81), Space: O(n^2) temporary matrices.
 */
void strassen_multiply(int n, const int A[MATRIX_MAX][MATRIX_MAX],
                       const int B[MATRIX_MAX][MATRIX_MAX],
                       int C[MATRIX_MAX][MATRIX_MAX]) {
    /* Base case threshold: for small sizes (<= 4), classical is faster */
    if (n <= 4) {
        matrix_multiply_classical(n, A, B, C);
        return;
    }

    int half = n / 2;
    int A11[MATRIX_MAX][MATRIX_MAX], A12[MATRIX_MAX][MATRIX_MAX];
    int A21[MATRIX_MAX][MATRIX_MAX], A22[MATRIX_MAX][MATRIX_MAX];
    int B11[MATRIX_MAX][MATRIX_MAX], B12[MATRIX_MAX][MATRIX_MAX];
    int B21[MATRIX_MAX][MATRIX_MAX], B22[MATRIX_MAX][MATRIX_MAX];

    /* Divide A and B into 4 submatrices */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + half];
            A21[i][j] = A[i + half][j];
            A22[i][j] = A[i + half][j + half];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + half];
            B21[i][j] = B[i + half][j];
            B22[i][j] = B[i + half][j + half];
            ops_increment(8);
        }
    }

    /* Strassen 7 products matrices */
    int M1[MATRIX_MAX][MATRIX_MAX], M2[MATRIX_MAX][MATRIX_MAX];
    int M3[MATRIX_MAX][MATRIX_MAX], M4[MATRIX_MAX][MATRIX_MAX];
    int M5[MATRIX_MAX][MATRIX_MAX], M6[MATRIX_MAX][MATRIX_MAX], M7[MATRIX_MAX][MATRIX_MAX];
    int T1[MATRIX_MAX][MATRIX_MAX], T2[MATRIX_MAX][MATRIX_MAX];

    /* M1 = (A11 + A22) * (B11 + B22) */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            T1[i][j] = A11[i][j] + A22[i][j];
            T2[i][j] = B11[i][j] + B22[i][j];
            ops_increment(2);
        }
    }
    strassen_multiply(half, T1, T2, M1);

    /* M2 = (A21 + A22) * B11 */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            T1[i][j] = A21[i][j] + A22[i][j];
            ops_increment(1);
        }
    }
    strassen_multiply(half, T1, B11, M2);

    /* M3 = A11 * (B12 - B22) */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            T2[i][j] = B12[i][j] - B22[i][j];
            ops_increment(1);
        }
    }
    strassen_multiply(half, A11, T2, M3);

    /* M4 = A22 * (B21 - B11) */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            T2[i][j] = B21[i][j] - B11[i][j];
            ops_increment(1);
        }
    }
    strassen_multiply(half, A22, T2, M4);

    /* M5 = (A11 + A12) * B22 */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            T1[i][j] = A11[i][j] + A12[i][j];
            ops_increment(1);
        }
    }
    strassen_multiply(half, T1, B22, M5);

    /* M6 = (A21 - A11) * (B11 + B12) */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            T1[i][j] = A21[i][j] - A11[i][j];
            T2[i][j] = B11[i][j] + B12[i][j];
            ops_increment(2);
        }
    }
    strassen_multiply(half, T1, T2, M6);

    /* M7 = (A12 - A22) * (B21 + B22) */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            T1[i][j] = A12[i][j] - A22[i][j];
            T2[i][j] = B21[i][j] + B22[i][j];
            ops_increment(2);
        }
    }
    strassen_multiply(half, T1, T2, M7);

    /* Reconstruct C quadrants:
     * C11 = M1 + M4 - M5 + M7
     * C12 = M3 + M5
     * C21 = M2 + M4
     * C22 = M1 - M2 + M3 + M6
     */
    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            C[i][j]               = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C[i][j + half]        = M3[i][j] + M5[i][j];
            C[i + half][j]        = M2[i][j] + M4[i][j];
            C[i + half][j + half] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
            ops_increment(8);
        }
    }
}
