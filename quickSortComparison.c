#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long comparison_count = 0;

void quick_sort_first(int arr[], int low, int high);
int partition_first(int arr[], int low, int high);

void quick_sort_random(int arr[], int low, int high);
int partition_random(int arr[], int low, int high);

void quick_sort_median3(int arr[], int low, int high);
int partition_median3(int arr[], int low, int high);

void iterative_quick_sort(int arr[], int low, int high);
int is_sorted(int arr[], int n);

void quick_sort_first(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition_first(arr, low, high);
        quick_sort_first(arr, low, pi - 1);
        quick_sort_first(arr, pi + 1, high);
    }
}

int partition_first(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low + 1;
    int j = high;
    int temp;

    while (1) {
        while (i <= j) {
            comparison_count++;
            if (arr[i] <= pivot) {
                i++;
            }
            else {
                break;
            }
        }

        while (i <= j) {
            comparison_count++;
            if (arr[j] >= pivot) {
                j--;
            }
            else {
                break;
            }
        }

        if (i <= j) {
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
            j--;
        }
        else {
            break;
        }
    }

    temp = arr[low];
    arr[low] = arr[j];
    arr[j] = temp;
    return j;
}

void quick_sort_random(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition_random(arr, low, high);
        quick_sort_random(arr, low, pi - 1);
        quick_sort_random(arr, pi + 1, high);
    }
}

int partition_random(int arr[], int low, int high) {
    int rand_idx = low + rand() % (high - low + 1);
    int temp = arr[low];
    arr[low] = arr[rand_idx];
    arr[rand_idx] = temp;

    return partition_first(arr, low, high);
}

void quick_sort_median3(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition_median3(arr, low, high);
        quick_sort_median3(arr, low, pi - 1);
        quick_sort_median3(arr, pi + 1, high);
    }
}

int partition_median3(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;
    int idx1, idx2, med_idx;
    int temp;

    comparison_count++;
    if (arr[low] > arr[mid]) {
        idx1 = mid;
        idx2 = low;
    }
    else {
        idx1 = low;
        idx2 = mid;
    }

    comparison_count++;
    if (arr[idx2] > arr[high]) {
        comparison_count++;
        if (arr[idx1] > arr[high]) {
            med_idx = idx1;
        }
        else {
            med_idx = high;
        }
    }
    else {
        med_idx = idx2;
    }

    temp = arr[low];
    arr[low] = arr[med_idx];
    arr[med_idx] = temp;

    return partition_first(arr, low, high);
}

void iterative_quick_sort(int arr[], int low, int high) {
    if (low >= high) return;

    int size = high - low + 1;
    int* stack = (int*)malloc(size * sizeof(int));
    int top = -1;

    stack[++top] = low;
    stack[++top] = high;

    while (top >= 0) {
        high = stack[top--];
        low = stack[top--];

        if (low < high) {
            int pi = partition_median3(arr, low, high);

            if (pi - 1 > low) {
                stack[++top] = low;
                stack[++top] = pi - 1;
            }

            if (pi + 1 < high) {
                stack[++top] = pi + 1;
                stack[++top] = high;
            }
        }
    }
    free(stack);
}

int is_sorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) return 0;
    }
    return 1;
}

int main() {
    srand((unsigned int)time(NULL));
    int sizes[] = { 1000, 2000, 4000, 8000, 16000, 20000 };
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    printf("=== 퀵 정렬 피벗 선택 방법별 비교 횟수 측정 ===\n\n");

    for (int s = 0; s < num_sizes; s++) {
        int n = sizes[s];
        int* original = (int*)malloc(n * sizeof(int));
        int* data1 = (int*)malloc(n * sizeof(int));
        int* data2 = (int*)malloc(n * sizeof(int));
        int* data3 = (int*)malloc(n * sizeof(int));

        for (int i = 0; i < n; i++) {
            original[i] = rand() % 100000 + 1;
        }

        for (int i = 0; i < n; i++) data1[i] = original[i];
        comparison_count = 0;
        quick_sort_first(data1, 0, n - 1);
        long long count_first = comparison_count;
        if (!is_sorted(data1, n)) { printf("Error: data1 not sorted!\n"); }

        for (int i = 0; i < n; i++) data2[i] = original[i];
        comparison_count = 0;
        quick_sort_random(data2, 0, n - 1);
        long long count_random = comparison_count;
        if (!is_sorted(data2, n)) { printf("Error: data2 not sorted!\n"); }

        for (int i = 0; i < n; i++) data3[i] = original[i];
        comparison_count = 0;
        quick_sort_median3(data3, 0, n - 1);
        long long count_median3 = comparison_count;
        if (!is_sorted(data3, n)) { printf("Error: data3 not sorted!\n"); }

        printf("Data Size : %d\n", n);
        printf("Quick Sort (First)    : %lld comparisons\n", data1 ? count_first : 0);
        printf("Quick Sort (Random)   : %lld comparisons\n", count_random);
        printf("Quick Sort (Median-3) : %lld comparisons\n\n", count_median3);

        free(original);
        free(data1);
        free(data2);
        free(data3);
    }

    printf("=== [보너스] 재귀적 vs 반복적 퀵 정렬 실행 시간 비교 (Size: 10000) ===\n");
    int bonus_size = 10000;
    int repeats = 50;

    int* bonus_orig = (int*)malloc(bonus_size * sizeof(int));
    int* temp_arr = (int*)malloc(bonus_size * sizeof(int));

    for (int i = 0; i < bonus_size; i++) {
        bonus_orig[i] = rand() % 100000 + 1;
    }

    clock_t start, end;
    double rec_total_ms = 0;
    double iter_total_ms = 0;

    for (int r = 0; r < repeats; r++) {
        for (int i = 0; i < bonus_size; i++) temp_arr[i] = bonus_orig[i];
        start = clock();
        quick_sort_median3(temp_arr, 0, bonus_size - 1);
        end = clock();
        rec_total_ms += (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;

        for (int i = 0; i < bonus_size; i++) temp_arr[i] = bonus_orig[i];
        start = clock();
        iterative_quick_sort(temp_arr, 0, bonus_size - 1);
        end = clock();
        iter_total_ms += (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    }

    printf("Data Size : %d\n", bonus_size);
    printf("Pivot     : Median-of-Three\n\n");
    printf("Recursive Quick Sort : %.4f ms\n", rec_total_ms / repeats);
    printf("Iterative Quick Sort : %.4f ms\n", iter_total_ms / repeats);

    free(bonus_orig);
    free(temp_arr);

    return 0;
}