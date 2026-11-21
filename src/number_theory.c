/**
 * @file number_theory.c
 * @brief Implementation of Chinese Remainder Theorem with Extended Euclidean Algorithm.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#include "number_theory.h"

/**
 * Extended Euclidean Algorithm: solves a*x + b*y = gcd(a, b).
 * Time: O(log(min(a, b))), Space: O(log(min(a, b))) call stack.
 */
long long extended_gcd(long long a, long long b, long long *x, long long *y) {
    ops_increment(1);
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    long long x1, y1;
    long long gcd = extended_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return gcd;
}

/**
 * Computes modular multiplicative inverse of a modulo m using Extended GCD.
 * Returns inverse in range [0, m-1], or -1 if gcd(a, m) != 1.
 * Time: O(log m), Space: O(log m).
 */
long long modular_inverse(long long a, long long m) {
    long long x, y;
    long long g = extended_gcd(a, m, &x, &y);
    if (g != 1) return -1;
    return (x % m + m) % m;
}

/**
 * Chinese Remainder Theorem:
 * Reconstructs total counter x such that x = rem[i] (mod mod[i]) for all i.
 * Requires moduli to be pairwise coprime.
 * Formula: x = (sum_{i=1}^k rem[i] * M_i * inv(M_i, mod[i])) mod M, where M = prod(mod[i]).
 * Time: Theta(k * log(M)), Space: O(1).
 */
CRTResult chinese_remainder_theorem(int k, const long long rem[], const long long mod[]) {
    CRTResult res;
    res.total_counter = 0;
    res.composite_modulus = 1;
    res.is_valid = 1;

    if (k <= 0 || k > CRT_MAX_PARTITIONS) {
        res.is_valid = 0;
        return res;
    }

    /* Compute total product M */
    for (int i = 0; i < k; i++) {
        ops_increment(1);
        if (mod[i] <= 0) { res.is_valid = 0; return res; }
        res.composite_modulus *= mod[i];
    }

    long long result = 0;
    for (int i = 0; i < k; i++) {
        long long Mi = res.composite_modulus / mod[i];
        long long inv = modular_inverse(Mi, mod[i]);
        if (inv == -1) {
            res.is_valid = 0; /* Moduli are not pairwise coprime */
            return res;
        }
        long long term = (rem[i] * Mi) % res.composite_modulus;
        term = (term * inv) % res.composite_modulus;
        result = (result + term) % res.composite_modulus;
        ops_increment(3);
    }

    res.total_counter = (result + res.composite_modulus) % res.composite_modulus;
    return res;
}
