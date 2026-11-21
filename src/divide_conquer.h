/**
 * @file divide_conquer.h
 * @brief Divide and Conquer algorithms for EV Analytics.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation for B.Tech student viva defense.
 */

#ifndef DIVIDE_CONQUER_H
#define DIVIDE_CONQUER_H

#include "model.h"
#include "analysis.h"

/* Result struct for Maximum Subarray */
typedef struct {
    int low;
    int high;
    int sum;
} MaxSubarrayResult;

/* Maximum Subarray Problem (best consecutive operational profit/energy days) */
MaxSubarrayResult max_subarray_dc(const int arr[], int low, int high);
MaxSubarrayResult max_subarray_brute(const int arr[], int n);

/* Fast Exponentiation vs Naive Loop (battery degradation or modular power) */
double fast_power(double base, int exp);
double naive_power(double base, int exp);
long long modular_power(long long base, long long exp, long long mod);

/* Celebrity / Hub Station Identification */
/* Returns junction ID of hub station if exists, or -1 if none */
int celebrity_hub_linear(int n, int reach[MAX_NODES][MAX_NODES]);
int celebrity_hub_brute(int n, int reach[MAX_NODES][MAX_NODES]);

/* Matrix Multiplication: Classical O(n^3) vs Strassen O(n^2.81) */
#define MATRIX_MAX 64
void matrix_multiply_classical(int n, const int A[MATRIX_MAX][MATRIX_MAX],
                               const int B[MATRIX_MAX][MATRIX_MAX],
                               int C[MATRIX_MAX][MATRIX_MAX]);

void strassen_multiply(int n, const int A[MATRIX_MAX][MATRIX_MAX],
                       const int B[MATRIX_MAX][MATRIX_MAX],
                       int C[MATRIX_MAX][MATRIX_MAX]);

#endif /* DIVIDE_CONQUER_H */
