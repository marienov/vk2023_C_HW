#pragma once

#include <fstream>
#include <string>
#include <set>
#include <vector>


struct Series {
private:
    double rating;
    std::string id;
    std::string title;
public:
    Series(std::string id_, std::string title_, double rating_) : id(id_), title(title_), rating(rating_) {}
    std::string getId() const { return id; }
    std::string getTitle() const { return title; }
    bool operator<(const Series& series) const {
        return (rating < series.rating);
    }
};
void choose_series(std::ifstream& file_bas, std::ifstream& file_rat, std::ifstream& file_title, int max_min, std::set<Series>& ser_set);
void find_title(std::ifstream& file_title, std::string& s_id, std::string& s_title, std::string& last_line);
double find_rating(std::ifstream& file_rat, std::string& s_id, std::string& last_line);
void split_string(std::string const& line, std::vector <std::string>& words);
bool isNumeric(std::string const& str);
int input_params(int argc, char* argv[], std::ifstream& fbas, std::ifstream& faka, std::ifstream& frat );
void print_set(std::set<Series> const& set_obj);