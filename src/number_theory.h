/**
 * @file number_theory.h
 * @brief Chinese Remainder Theorem (CRT) and Modular Arithmetic for Distributed Counters.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#ifndef NUMBER_THEORY_H
#define NUMBER_THEORY_H

#include <stdio.h>
#include <stdlib.h>
#include "analysis.h"

#define CRT_MAX_PARTITIONS 8

typedef struct {
    long long total_counter;
    long long composite_modulus;
    int is_valid;
} CRTResult;

/* Extended Euclidean Algorithm: computes gcd(a, b) and coefficients x, y such that a*x + b*y = gcd */
long long extended_gcd(long long a, long long b, long long *x, long long *y);

/* Modular multiplicative inverse: computes a^(-1) mod m */
long long modular_inverse(long long a, long long m);

/* Chinese Remainder Theorem: solves x = rem[i] (mod mod[i]) */
CRTResult chinese_remainder_theorem(int k, const long long rem[], const long long mod[]);

#endif /* NUMBER_THEORY_H */
