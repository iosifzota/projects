#include <stdlib.h>
#include <stdio.h>

typedef struct {
    unsigned begin, end;
    int sum;
} Interval;

Interval biggest_contiguous_sum(const int arr[], unsigned size);

int main()
{
    int arr[] = { 13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7 };

    Interval res = biggest_contiguous_sum(arr, sizeof(arr)/sizeof(arr[0]));

    printf("The max sum: %d\n", res.sum);

    for (unsigned itr = res.begin; itr < res.end; ++itr) {
        printf("%d ", arr[itr]);
    }
    puts("");

    return 0;
}


Interval biggest_contiguous_sum(const int arr[], unsigned size)
{
    Interval max_so_far = {};
    unsigned current_begin;
    int current_sum;

    if (max_so_far.end != 0)
        exit(-1);
    if (max_so_far.begin != 0)
        exit(-1);
    if (max_so_far.sum != 0)
        exit(-1);

    current_sum = 0;

    for (unsigned i = 0; i < size; ++i) {
        current_sum += arr[i];

        if (current_sum < 0) {
            current_sum = 0;
            current_begin = i + 1; // reset current sequence
        }
        else if (current_sum > max_so_far.sum) {
            if (max_so_far.begin != current_begin)
                max_so_far.begin = current_begin;

            max_so_far.end = i + 1;
            max_so_far.sum = current_sum;
        }
    }

    return max_so_far;
}
