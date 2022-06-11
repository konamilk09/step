#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <set>
#include <cfloat>
#include <string>

class City {
private:
    std::pair<double, double> point;
public:
    City(double X, double Y);
    double x() const;
    double y() const;
};

City::City(double X, double Y) {
    point = std::move(std::make_pair(X, Y));
}

double City::x() const {
    return point.first;
}

double City::y() const {
    return point.second;
}

std::vector<City> read_input(const std::string& filenum) {
    std::vector<City> cities;

    std::string filename = "./data/input_" + filenum + ".csv";
    std::ifstream fin(filename);
    if (fin.fail()) {
		printf("Couldn't open the file: %s\n", filename.c_str());
		exit(EXIT_FAILURE);
	}

    std::string data;
    // 1行目の x,y を受け取っておく
    std::getline(fin, data);
    while(std::getline(fin, data)) {
        auto index = data.find(',');
        auto x = data.substr(0, index);
        auto y = data.substr(index + 1, data.size() - x.size() - 1);
        
        try{
            cities.push_back(City(std::stod(x), std::stod(y)));
        }
        catch(const std::invalid_argument& e){
            std::cout << "invalid argument" << std::endl;
        }
        catch(const std::out_of_range& e){
            std::cout << "Out of range" << std::endl;
        }
    }

    fin.close();

        return cities;
}

// 2点間の距離を計算する
double distance(const City& city1, const City& city2) {
    return std::sqrt(std::pow(city1.x()-city2.x(), 2.0)+std::pow(city1.y()-city2.y(), 2.0));
}

std::vector<std::vector<double>> calc_dist(
    const std::vector<City>& cities,
    const int& N) 
{
    std::vector<std::vector<double>> dist(N, std::vector<double>(N));
    // 2点間の距離を配列に代入する
    for(int i=0; i<N; i++) {
        for(int j=0; j<N; j++) {
            dist[i][j] = dist[j][i] = distance(cities[i], cities[j]);
        }
    }
    return dist;
}

std::set<int> init_set(const int& start, const int& end) {
    std::set<int> s;
    for(int i=start; i<end; i++) {
        s.insert(i);
    }
    return s;
}

// 1点と、2点間の距離の配列と、まだ辿っていない点のセットを受け取って、
// 1点と一番近い点を返す
int min_city(const int& city,
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

std::vector<int> greedy(const std::vector<City>& cities, 
    const std::vector<std::vector<double>>& dist, 
    const int &N) {

    std::vector<int> tour;

    int current_city = 0;
    std::set<int> unvisited_cities;
    unvisited_cities = init_set(1, N);

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

void write_tour(const std::vector<int>& tour, const std::string& filenum) {
    auto filename = "./data/output_" + filenum + ".csv";
    std::ofstream fout(filename);
    fout << "index" << std::endl;
    for(int i=0; i<(int)tour.size(); i++) {
        fout << tour[i] << std::endl;
    }
    fout.close();
    return;
}

void print_path_len(
    const std::vector<int>& tour,
    const std::vector<std::vector<double>>& dist) 
{
    double ans=0;
    for(int i=0; i<tour.size()-1; i++) {
        ans += dist[i][i+1];
    }
    std::cout << ans << std::endl;
    return;
}

int main(int argc, char* argv[]){
    if(argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        return EXIT_FAILURE;
    }

    std::vector<City> cities;
    cities = read_input(argv[1]);

    // すべての2点間の距離を保持する配列
    int N = cities.size();
    std::vector<std::vector<double>> dist(N, std::vector<double>(N));
    dist = calc_dist(cities, N);

    std::vector<int> tour;
    tour = greedy(cities, dist, N);

    write_tour(tour, argv[1]);

    print_path_len(tour, dist);

    return 0;
}
