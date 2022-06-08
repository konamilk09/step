/**
 * STEP2022 week2
 * homework1 
 * 行列のサイズＮが与えられて、行列積を求め、実行時間を出力する。
 **/
#include <iostream>
#include <string>
#include <sys/time.h>
#include <vector>

// 時刻を受け取る
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

int main(int argc, char* argv[]) {
    
    if(argc != 2) {
        printf("Usage: %s N\n", argv[0]);
        return -1;
    }

    int n = atoi(argv[1]);

    std::vector<std::vector<double>> a(n, std::vector<double>(n)); // 行列A
    std::vector<std::vector<double>> b(n, std::vector<double>(n)); // 行列B
    std::vector<std::vector<double>> c(n, std::vector<double>(n)); // 行列C

    // 行列を適当な数で初期化
    int i, j, k;
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            a[i][j] = i * n + j; // A[i][j]
            b[i][j] = j * n + i; // B[i][j]
            c[i][j] = 0; // C[i][j]
        }
    }

    double begin = get_time();

    // 行列積の計算
    for(i = 0; i < n; i++) {
        for(j = 0; j < n; j++) {
            for(k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    double end = get_time();
    printf("time: %.6lf sec\n", end - begin);

    // // Print C for debugging. Comment out the print before measuring the execution time.
    // double sum = 0;
    // for(i = 0; i < n; i++) {
    //     for(j = 0; j < n; j++) {
    //         sum += c[i][j];
    //         // printf("c[%d][%d]=%lf\n", i, j, c[i * n + j]);
    //     }
    // }
    // // Print out the sum of all values in C.
    // // This should be 450 for N=3, 3680 for N=4, and 18250 for N=5.
    // printf("sum: %.6lf\n", sum);

    return 0;
}
