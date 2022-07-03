#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cfloat>
#include <cassert>
#include <stdio.h>
#include <string>
#include <cmath>
#include <set>
#include <algorithm>
#include <fstream>
#include "tsp.hpp"
#define POPULATION 50
#define TOP_POPULATION 15
#define GENERATIONS 2000

/* 1つの経路 */
class Tour {
    double pathlength; // 経路の長さ
    std::vector<int> tour; // 経路の順番
public:
    Tour() : pathlength(0) {}

    bool contain(int x) const{
        for(int i=0; i<tour.size(); i++) {
            if(tour[i]==x) return true;
        }
        return false;
    }
    int size() const{
        return tour.size();
    }
    void push_back(int x) {
        tour.push_back(x);
    }
    int at(int i) const{
        return tour.at(i);
    }
    auto begin() {
        return tour.begin();
    }
    double get_pathlength() const{
        return pathlength;
    }
    void set_pathlength(double path_len) {
        pathlength = path_len;
    }
    void set_tour(std::vector<int> vec) {
        tour = vec;
    }
};

/* 1世代を pathlength で並べ替えるときに使う */
class Cmp {
public:
    bool operator()(const Tour& a, const Tour& b) noexcept {
        return a.get_pathlength() < b.get_pathlength();
    }
};

/* 2点間の距離を計算する */
double distance(const City& city1, const City& city2) {
    double x = city1.x()-city2.x();
    double y = city1.y()-city2.y();
    return std::sqrt(x*x + y*y);
}

/* 2点間の距離を配列に代入する */
std::vector<std::vector<double>> init_dist(
    const std::vector<City>& cities,
    const int& N) 
{
    std::vector<std::vector<double>> dist(N, std::vector<double>(N));
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            dist[i][j] = dist[j][i] = distance(cities[i], cities[j]);
        }
    }
    return dist;
}

/* 二つの整数 begin,end (a<end) を受け取り、要素が a から end-1 までの整数の set を返す */
std::set<int> init_set(const int& begin, const int& end) {
    std::set<int> s;
    for(int i=begin; i<end; i++) {
        s.insert(i);
    }
    return s;
}

/* 1点と、2点間の距離の配列と、まだ辿っていない点の set を受け取って、
   1点と一番近い点を返す */
int min_city(
    const int& city,
    const std::vector<std::vector<double>>& dist,
    const std::set<int>& unvisited_cities) 
{
    double min_dist = DBL_MAX;
    int next_city;
    for(const int& unvisited_city : unvisited_cities) {
        if(dist[city][unvisited_city] < min_dist) {
            min_dist = dist[city][unvisited_city];
            next_city = unvisited_city;
        }
    }
    return next_city;
}

/* 貪欲法：0をスタート地点として、まだ辿っていない点のうち一番今の点に近い点を次行く点とする
   辿る順番の配列を返す */
Tour greedy(
    const std::vector<City>& cities, 
    const std::vector<std::vector<double>>& dist, 
    const int &N,
    const int &init) 
{
    Tour tour;

    int current_city = init;
    std::set<int> unvisited_cities;
    unvisited_cities = init_set(0, N);
    unvisited_cities.erase(init);

    tour.push_back(current_city);

    // まだ見てない点のうち current_city 一番近い点を次に行く点とする
    while(!unvisited_cities.empty()) {
        int next_city = min_city(current_city, dist, unvisited_cities);
        unvisited_cities.erase(next_city);
        tour.push_back(next_city);
        current_city = next_city;
    }

    return tour;
}

/* 2組の辺を選び、交換した方が経路が短くなったら経路を更新する。 */
void two_opt(
    Tour& tour, 
    const std::vector<City>& cities, 
    const std::vector<std::vector<double>>& dist, 
    const int& N) 
{
    bool is_cross=true;
    while(is_cross) {
        is_cross = false;
        for(int i=0; i<N-2; i++) {
            for(int j=i+2; j<N; j++) {
                int a1 = tour.at(i);
                int a2 = tour.at(i+1);
                int b1 = tour.at(j);
                int b2 = tour.at((j+1)%N);

                if(dist[a1][b1] + dist[a2][b2] < dist[a1][a2] + dist[b1][b2]) {
                    std::reverse(tour.begin()+(i+1), tour.begin()+(j+1));
                    is_cross=true;
                }
            }
        }
    }
    return;
}

/* 受け取った経路の長さを返す */
double calc_path_len(
    Tour& tour,
    const std::vector<std::vector<double>>& dist) 
{
    double ans=0;
    for(int i=0; i<tour.size()-1; i++) {
        ans += dist[tour.at(i)][tour.at(i+1)];
    }
    return ans;
}

/* 最初の世代を貪欲法と two_opt で作る */
std::vector<Tour> init_generation(
    const std::vector<City>& cities, 
    const std::vector<std::vector<double>>& dist, 
    const int& N) 
{
    std::vector<Tour> generation;
    for(int j=0; j<POPULATION; j++) {
        Tour tour = greedy(cities, dist, N, j%N);
        two_opt(tour, cities, dist, N);
        tour.set_pathlength(calc_path_len(tour, dist));
        generation.push_back(tour);
    }
    return generation;
}

/* 世代の中で上位30％とランダムに抽出された個体を返す */
std::vector<Tour> selection(std::vector<Tour>& generation) {
    std::vector<Tour> ret(POPULATION);
    // 世代の中で経路が短い順にソートする
    std::vector<Tour> sorted = generation;
    std::sort(sorted.begin(), sorted.end(), Cmp{});
    // 上位30％は2倍残す
    for(int i=0; i<TOP_POPULATION; i++) {
        ret[i*2] = sorted[i];
        ret[i*2+1] = sorted[i];
    }
    // それ以外からランダムで選ぶ
    for(int i=TOP_POPULATION; i<POPULATION; i++) {
        ret[i] = sorted[rand()%(POPULATION-TOP_POPULATION)+TOP_POPULATION];
    }
    return ret;
}

/* 交差 parent1 の一部をそのまま子供に引継ぎ、 parent2 の前から順で子供の残りの部分を埋める */
Tour cross_over(Tour& parent1, Tour& parent2, const int& N) {
    Tour child;
    int begin = std::rand()%parent1.size();
    int end = std::rand()%parent1.size();
    if(end < begin) std::swap(end, begin);
    for(int i=begin; i<end; i++) {
        child.push_back(parent1.at(i));
    }
    for(int i=0; i<parent2.size(); i++) {
        if(!child.contain(parent2.at(i))) {
            child.push_back(parent2.at(i));
        }
    }
    assert(child.size() == N);
    return child;
}

/* 今の世代を受け取り次の世代を返す */
std::vector<Tour> next_generation(
    std::vector<Tour> generation, 
    const std::vector<City>& cities, 
    const std::vector<std::vector<double>>& dist, 
    const int& N) 
{
    std::vector<Tour> children;
    generation = selection(generation);
    for(int j=0; j<POPULATION; j++) {
        Tour parent1 = generation[std::rand()%POPULATION];
        Tour parent2 = generation[std::rand()%POPULATION];
        Tour child = cross_over(parent1, parent2, N);
        two_opt(child, cities, dist, N);
        child.set_pathlength(calc_path_len(child, dist));
        children.push_back(child);
    }
    return children;
}

/* 世代のうち最短経路を返す */
Tour best_score(
    std::vector<Tour>& generation, 
    const std::vector<std::vector<double>>& dist) 
{
    double min_path = DBL_MAX;
    Tour* min_tour;
    for(int i=0; i<generation.size(); i++) {
        double path = calc_path_len(generation[i], dist);
        if(path < min_path) {
            min_path = path;
            min_tour = &generation[i];
        }
    }
    return *min_tour;
}

Tour genetic_algorithm(
    std::vector<Tour> &tours, 
    const std::vector<City>& cities, 
    const std::vector<std::vector<double>>& dist, 
    const int& N) 
{
    std::vector<Tour> generation;
    generation = init_generation(cities, dist, N);

    // 進化させる
    for(int i=0; i<GENERATIONS; i++) {
        generation = next_generation(generation, cities, dist, N);
    }
    return best_score(generation, dist);
}

/* 出力ファイルに実行結果を書き込む */
void write_tour(
    Tour& tour, 
    const std::string& filenum) 
{
    std::string filename = "./data/output_" + filenum + ".csv";
    std::ofstream fout(filename);
    fout << "index" << std::endl;
    for(int i=0; i<(int)tour.size(); i++) {
        fout << tour.at(i) << std::endl;
    }
    fout.close();
    return;
}

int main(int argc, char* argv[]){
    if(argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    std::srand(1);

    std::vector<City> cities;
    cities = read_input(argv[1]);
    int N = cities.size();

    /* すべての2点間の距離を保持する配列 */
    std::vector<std::vector<double>> dist(N, std::vector<double>(N));
    dist = init_dist(cities, N);

    /* 1世代の経路 */
    std::vector<Tour> tours;

    /* 実行時間の計測用 */
    double begin;
    double end;

    begin = get_time();

    Tour tour = genetic_algorithm(tours, cities, dist, N);
    write_tour(tour, argv[1]);

    end = get_time();

    printf("time: %.6lf sec\n", end - begin);
    return 0;
}
