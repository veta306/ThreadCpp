#include<iostream>
#include<omp.h>

using namespace std;

const int rows = 20000;
const int cols = 20000;

int arr[rows][cols];

void init_arr();
long long sum_elements(int);
pair<int, long long> row_with_min_sum(int);

int main() {
    init_arr();
    omp_set_nested(1);
    double t1 = omp_get_wtime();

#pragma omp parallel sections
    {
#pragma omp section
        {
            for (int i = 1; i <= 64; i *= 2) {
                cout << "Total sum of elements: " << sum_elements(i) << endl;
            }
        }
#pragma omp section
        {
            for (int i = 1; i <= 64; i *= 2) {
                pair<int, long long> min_row_sum = row_with_min_sum(i);
                cout << "Row with minimum sum: " << min_row_sum.first << ", Minimum sum: " << min_row_sum.second << endl;
            }
        }
    }

    double t2 = omp_get_wtime();

    cout << "Total time: " << t2 - t1 << " seconds" << endl;

    return 0;
}

void init_arr() {
    srand((unsigned)time(NULL));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            //arr[i][j] = cols * i + j;
            arr[i][j] = (rows - i) * cols - j;
        }
    }
    int min = rand() % rows;
    for (int j = 0; j < cols; j++) {
        arr[min][j] = INT32_MIN;
    }
}

long long sum_elements(int num_threads) {
    long long total_sum = 0;
    double t1 = omp_get_wtime();

#pragma omp parallel for reduction(+:total_sum) num_threads(num_threads)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            total_sum += arr[i][j];
        }
    }
    double t2 = omp_get_wtime();
    cout << "Sum with " << num_threads << " threads worked - " << t2 - t1 << " seconds" << endl;
    return total_sum;
}

pair<int, long long> row_with_min_sum(int num_threads) {
    long long min_sum = LLONG_MAX;
    int min_row = -1;
    double t1 = omp_get_wtime();

#pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < rows; i++) {
        long long row_sum = 0;
        for (int j = 0; j < cols; j++) {
            row_sum += arr[i][j];
        }
        if (row_sum < min_sum) {
#pragma omp critical
            if (row_sum < min_sum) {
                min_sum = row_sum;
                min_row = i;
            }
        }
    }
    double t2 = omp_get_wtime();
    cout << "Min with " << num_threads << " threads worked - " << t2 - t1 << " seconds" << endl;
    return make_pair(min_row, min_sum);
}
