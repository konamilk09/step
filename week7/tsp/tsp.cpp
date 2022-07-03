#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <sys/time.h>
#include "tsp.hpp"

City::City(double X, double Y)
    : pointx(std::move(X)), pointy(std::move(Y)) {}

double City::x() const {
    return pointx;
}

double City::y() const {
    return pointy;
}

/* 時刻を返す */
double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

/* 入力ファイル読み込み：番号の文字列を受け取ったらその番号のファイルを読み込み、
   std::vector<City> 型の点の座標の配列を返す */
std::vector<City> read_input(const std::string& filenum) {
    std::vector<City> cities;

    std::string filename = "./data/input_" + filenum + ".csv";
    std::ifstream fin(filename);
    if (fin.fail()) {
		printf("Couldn't open the file: %s\n", filename.c_str());
		exit(EXIT_FAILURE);
	}

    std::string data;
    /* 1行目が x,y となっていることを確認する。 */
    assert(std::getline(fin, data) && data=="x,y");
    while(std::getline(fin, data)) {
        auto index = data.find(',');
        auto x = data.substr(0, index);
        auto y = data.substr(index + 1, data.size() - x.size() - 1);

        try{
            cities.push_back(City(std::stod(x), std::stod(y)));
        }
        catch(const std::invalid_argument& e){
            std::cout << "invalid argument" << std::endl;
            exit(EXIT_FAILURE);
        }
        catch(const std::out_of_range& e){
            std::cout << "Out of range" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    fin.close();

    return cities;
}
