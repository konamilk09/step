#include <sys/time.h>
#include <vector>
#include <stdio.h>

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

int main() {

    double begin;
    double end;
    int n = 1000;
    int i, j, k;

    std::vector<std::vector<int>> a(n, std::vector<int>(n, 1));
    std::vector<std::vector<int>> b(n, std::vector<int>(n, 1));
    std::vector<std::vector<int>> c(n, std::vector<int>(n));

    begin = get_time();
    for (k = 0; k < n; k++)
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++)
                c[i][j] += a[i][k] * b[k][j];
    end = get_time();
    printf("k-i-j: %.3f\n", end-begin);

}
