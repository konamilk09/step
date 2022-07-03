#pragma once
#include <string>
#include <vector>

/* 1つの点：x座標とy座標 */
class City {
private:
    double pointx;
    double pointy;
public:
    City(double X, double Y);
    double x() const;
    double y() const;
};

std::vector<std::vector<double>> init_dist(
    const std::vector<City>& cities, 
    const int& N);

double get_time();

std::vector<City> read_input(const std::string& filenum);
