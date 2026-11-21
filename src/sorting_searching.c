/**
 * @file sorting_searching.c
 * @brief Implementations of Merge Sort, Quick Sort, and Binary Search (iterative & recursive).
 * 
 * Course: CSEG3053 - Design and Analysis of Algorithms
 * All algorithms are hand-written from first principles without qsort or bsearch.
 */

#include "sorting_searching.h"

/* -------------------------------------------------------------
 * INTEGER MERGE SORT
 * ------------------------------------------------------------- */

/**
 * Merges two sorted subarrays arr[left..mid] and arr[mid+1..right].
 * Time: O(n), Space: O(n) auxiliary.
 */
static void merge_int(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int *L = (int *)malloc((size_t)n1 * sizeof(int));
    int *R = (int *)malloc((size_t)n2 * sizeof(int));

    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
        ops_increment(1);
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
        ops_increment(1);
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        ops_increment(1); /* comparison */
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
        ops_increment(1); /* assignment */
    }

    while (i < n1) {
        arr[k++] = L[i++];
        ops_increment(1);
    }
    while (j < n2) {
        arr[k++] = R[j++];
        ops_increment(1);
    }

    free(L);
    free(R);
}

/**
 * Recursive Merge Sort for integer array.
 * Divide: split array into halves.
 * Conquer: recursively sort each half.
 * Combine: merge sorted halves.
 * Time: Theta(n log n), Space: O(n) auxiliary.
 */
void merge_sort_int(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_int(arr, left, mid);
        merge_sort_int(arr, mid + 1, right);
        merge_int(arr, left, mid, right);
    }
}

/* -------------------------------------------------------------
 * INTEGER QUICK SORT
 * ------------------------------------------------------------- */

/**
 * Swaps two integers and logs the operation.
 * Time: O(1), Space: O(1).
 */
static void swap_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    ops_increment(3);
}

/**
 * Lomuto partition scheme for integer array.
 * Pivot is chosen as the last element arr[high].
 * Time: O(n), Space: O(1).
 */
static int partition_int(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        ops_increment(1); /* comparison */
        if (arr[j] <= pivot) {
            i++;
            swap_int(&arr[i], &arr[j]);
        }
    }
    swap_int(&arr[i + 1], &arr[high]);
    return i + 1;
}

/**
 * Recursive Quick Sort for integer array.
 * Time: Best/Avg Theta(n log n), Worst O(n^2) on sorted/reversed array.
 * Space: O(log n) recursion stack.
 */
void quick_sort_int(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition_int(arr, low, high);
        quick_sort_int(arr, low, pi - 1);
        quick_sort_int(arr, pi + 1, high);
    }
}

/* -------------------------------------------------------------
 * BINARY SEARCH (INTEGER)
 * ------------------------------------------------------------- */

/**
 * Iterative Binary Search on a sorted integer array.
 * Returns index of target if found, or -1 if not found.
 * Time: Best O(1), Avg/Worst O(log n), Space: O(1).
 */
int binary_search_iterative_int(const int arr[], int size, int target) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        ops_increment(1); /* loop check / iteration */
        int mid = low + (high - low) / 2;

        ops_increment(1); /* equality check */
        if (arr[mid] == target) {
            return mid;
        }

        ops_increment(1); /* inequality check */
        if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

/**
 * Recursive Binary Search on a sorted integer array.
 * Returns index of target if found, or -1 if not found.
 * Time: Best O(1), Avg/Worst O(log n), Space: O(log n) call stack.
 */
int binary_search_recursive_int(const int arr[], int low, int high, int target) {
    ops_increment(1);
    if (low > high) {
        return -1;
    }

    int mid = low + (high - low) / 2;

    ops_increment(1); /* equality check */
    if (arr[mid] == target) {
        return mid;
    }

    ops_increment(1); /* inequality check */
    if (arr[mid] < target) {
        return binary_search_recursive_int(arr, mid + 1, high, target);
    } else {
        return binary_search_recursive_int(arr, low, mid - 1, target);
    }
}

/* -------------------------------------------------------------
 * STATION COMPARISON & SORTING
 * ------------------------------------------------------------- */

/**
 * Compares two stations based on the given criterion.
 * Returns < 0 if a should precede b, > 0 if b should precede a, 0 if equal.
 * Time: O(1), Space: O(1).
 */
static int compare_stations(const Station *a, const Station *b, StationSortCriterion criterion) {
    ops_increment(1);
    switch (criterion) {
        case SORT_BY_ID:
            return a->id - b->id;
        case SORT_BY_COST:
            return a->install_cost - b->install_cost;
        case SORT_BY_BENEFIT:
            return b->benefit_score - a->benefit_score; /* descending benefit */
        case SORT_BY_RATIO: {
            double rA = (a->install_cost > 0) ? ((double)a->benefit_score / a->install_cost) : 0;
            double rB = (b->install_cost > 0) ? ((double)b->benefit_score / b->install_cost) : 0;
            if (rA > rB) return -1; /* descending ratio */
            if (rA < rB) return 1;
            return 0;
        }
        default:
            return 0;
    }
}

/**
 * Merges two subarrays of stations according to criterion.
 * Time: O(n), Space: O(n).
 */
static void merge_stations(Station arr[], int left, int mid, int right, StationSortCriterion criterion) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Station *L = (Station *)malloc((size_t)n1 * sizeof(Station));
    Station *R = (Station *)malloc((size_t)n2 * sizeof(Station));

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compare_stations(&L[i], &R[j], criterion) <= 0) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
        ops_increment(1);
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

/**
 * Merge sort on station array.
 * Time: Theta(n log n), Space: O(n).
 */
void merge_sort_stations(Station arr[], int left, int right, StationSortCriterion criterion) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort_stations(arr, left, mid, criterion);
        merge_sort_stations(arr, mid + 1, right, criterion);
        merge_stations(arr, left, mid, right, criterion);
    }
}

/**
 * Swaps two Station structures.
 * Time: O(1), Space: O(1).
 */
static void swap_stations(Station *a, Station *b) {
    Station temp = *a;
    *a = *b;
    *b = temp;
    ops_increment(3);
}

/**
 * Lomuto partition for station array.
 * Time: O(n), Space: O(1).
 */
static int partition_stations(Station arr[], int low, int high, StationSortCriterion criterion) {
    Station pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (compare_stations(&arr[j], &pivot, criterion) <= 0) {
            i++;
            swap_stations(&arr[i], &arr[j]);
        }
    }
    swap_stations(&arr[i + 1], &arr[high]);
    return i + 1;
}

/**
 * Quick sort on station array.
 * Time: Avg Theta(n log n), Worst O(n^2), Space: O(log n).
 */
void quick_sort_stations(Station arr[], int low, int high, StationSortCriterion criterion) {
    if (low < high) {
        int pi = partition_stations(arr, low, high, criterion);
        quick_sort_stations(arr, low, pi - 1, criterion);
        quick_sort_stations(arr, pi + 1, high, criterion);
    }
}

/**
 * Binary search for a station by its unique ID in a sorted station array.
 * Returns array index if found, or -1 if not found.
 * Time: O(log n), Space: O(1).
 */
int binary_search_station_by_id(const Station arr[], int size, int target_id) {
    int low = 0;
    int high = size - 1;

    while (low <= high) {
        ops_increment(1);
        int mid = low + (high - low) / 2;
        if (arr[mid].id == target_id) {
            return mid;
        }
        if (arr[mid].id < target_id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

/**
 * Builds an index mapping where index[i] points to the i-th smallest station ID.
 * Uses selection sort on the index array to keep stations original array untouched.
 * Time: O(n^2), Space: O(1) auxiliary.
 */
void build_station_id_index(const Station stations[], int size, int index[]) {
    for (int i = 0; i < size; i++) {
        index[i] = i;
    }
    for (int i = 0; i < size - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < size; j++) {
            ops_increment(1);
            if (stations[index[j]].id < stations[index[min_idx]].id) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int tmp = index[i];
            index[i] = index[min_idx];
            index[min_idx] = tmp;
            ops_increment(3);
        }
    }
}
