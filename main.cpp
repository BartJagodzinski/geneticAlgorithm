#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <random>
#include <functional>
#include <math.h>
#include <tuple>
#include <map>
#include <limits>
#include <fstream>

// population
#define N 4

class Point {
	std::tuple<int, int, double> _xy;
public:
	Point() {}
	Point(int x, int y) { _xy = std::make_tuple(x, y, func(x,y)); }

	std::tuple<int, int, double> get() { return _xy; }

	void setX(int x) { std::get<0>(_xy) += x; }
	void setY(int y) { std::get<1>(_xy) += y; }
	void setValue() { std::get<2>(_xy) = func(std::get<0>(_xy), std::get<1>(_xy)); }

	double func(double x, double y) {	return  x / ((x * x) + 2 * (y * y) + 1); }

	std::string print() { return "(" + std::to_string(std::get<0>(_xy)) + ", " + std::to_string(std::get<1>(_xy)) + ") = " + std::to_string(std::get<2>(_xy)); }
	bool operator != (const Point& p) { return _xy != p._xy; }
	bool operator == (const Point& p) { return _xy == p._xy; }
};

class Genetic {
    std::vector<Point> _population;
public:
	Genetic() {}

	int rand(int from, int to) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(from, to);
		return dis(gen);
	}

	Point best() {
		double f = -999999999;
		Point bestValue;
		size_t id = 0;
		for (size_t i = 0; i < _population.size(); ++i) {
			if (std::get<2>(_population[i].get()) > f) { bestValue = _population[i];  f = std::get<2>(_population[i].get()); id = i; }
		}

		_population.erase(_population.begin() + id);

		return bestValue;
	}

	void delWorst() {
		double f = 999999999;
		size_t id = 0;
		for (size_t i = 0; i < _population.size(); ++i)
			if (std::get<2>(_population[i].get()) < f) { f = std::get<2>(_population[i].get()); id = i; }



		_population.erase(_population.begin() + id);
	}

	void mutate(Point &p, int axis, int newValue) {
		if (axis == 0) { p.setX(newValue); }
		else { p.setY(newValue); }
		p.setValue();
	}

	void mix(int range) {
		delWorst();
		Point bestUnit = best();

		Point p1(std::get<0>(bestUnit.get()), std::get<1>( _population[0].get()));
		Point p2(std::get<0>(bestUnit.get()), std::get<1>(_population[1].get()));
		Point p3(std::get<0>( _population[0].get()), std::get<1>(bestUnit.get()));
		Point p4 (std::get<0>(_population[1].get()), std::get<1>(bestUnit.get()));

		Point arr[N] = { p1, p2, p3, p4 };

		for (size_t i = 0; i < N; ++i) {
			for (size_t j = i + 1; j < N; ++j)
				if (arr[i] == arr[j]) mutate(arr[i], rand(0, 1), rand(-1, 1));
		}

		_population.clear();
		for (size_t i = 0; i < N; i++)
			_population.push_back(arr[i]);


	}

	void run(size_t iterLimit, int range) {
		for (size_t i = 0; i < N; ++i)
			_population.push_back(Point(rand(-range, range), rand(-range, range)));
        std::ofstream resultsFile;

        resultsFile.open("results.txt", std::ios_base::app);

		size_t it = 0;
		while (it < iterLimit) {
			mix(range);
			resultsFile << "== generation " << it << " ==" << std::endl;
            for (auto el : _population)
                resultsFile << el.print() << std::endl;
			++it;
		}
        resultsFile.close();
	}
};

int main() {

	Genetic g;
	g.run(1500, 500);

	return EXIT_SUCCESS;
}
