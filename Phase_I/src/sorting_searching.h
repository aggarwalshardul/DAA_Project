/**
 * @file sorting_searching.h
 * @brief Hand-crafted sorting and searching routines with operation tracking.
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * Plain C17 implementation without any standard library shortcuts.
 */

#ifndef SORTING_SEARCHING_H
#define SORTING_SEARCHING_H

#include "model.h"
#include "analysis.h"

/* Sorting criterion for stations */
typedef enum {
    SORT_BY_ID = 0,
    SORT_BY_COST,
    SORT_BY_BENEFIT,
    SORT_BY_RATIO
} StationSortCriterion;

/* Integer array sorting functions */
void merge_sort_int(int arr[], int left, int right);
void quick_sort_int(int arr[], int low, int high);

/* Station array sorting functions */
void merge_sort_stations(Station arr[], int left, int right, StationSortCriterion criterion);
void quick_sort_stations(Station arr[], int low, int high, StationSortCriterion criterion);

/* Binary search algorithms */
int binary_search_iterative_int(const int arr[], int size, int target);
int binary_search_recursive_int(const int arr[], int low, int high, int target);

/* Domain-specific search: search station in a sorted array by ID */
int binary_search_station_by_id(const Station arr[], int size, int target_id);

/* Index builder: builds an index array where index[i] points to station in sorted order */
void build_station_id_index(const Station stations[], int size, int index[]);

#endif /* SORTING_SEARCHING_H */
