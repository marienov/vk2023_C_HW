#pragma once

#include <fstream>
#include <string>
#include <set>
#include <vector>
using namespace std;

struct Series {
private:
    double rating;
    string id;
    string title;
public:
    Series(string id_, string title_, double rating_) : id(id_), title(title_), rating(rating_) {}
    string getId() const { return id; }
    string getTitle() const { return title; }
    bool operator<(const Series& series) const {
        return (rating < series.rating);
    }
};
void choose_series(ifstream& file_bas, ifstream& file_rat, ifstream& file_title, int max_min, set<Series>* ser_set);
void split_string(string& line, vector <string>* words);
void print_set(std::set<Series> const& set_obj);
bool isNumeric(string const& str);