#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include "utils.hpp"


void find_title(std::ifstream& file_title, std::string& s_id, std::string& s_title, std::string& last_line){
    std::vector<std::string> title_attr;
    std::string line_t;
    //check last line
    if (!last_line.empty()){
        split_string(last_line, title_attr);
        if (title_attr[0]  > s_id){
            title_attr.clear();
        }
        if (title_attr[0] == s_id) {
                s_title = title_attr[2];
            }
    }
    while ( getline(file_title, line_t)) {
        last_line = line_t;
        split_string(line_t, title_attr);
        if (title_attr[0]  > s_id){
            title_attr.clear();
            break;
        }
        else{
            std::streampos pos = file_title.tellg();
        }
        if (title_attr[0] == s_id && title_attr[3] == "RU") {
            s_title = title_attr[2];
        }
        title_attr.clear();
    }
}

double find_rating(std::ifstream& file_rat, std::string& s_id, std::string& last_line){
    std::vector<std::string> rat_attr;
    std::string line_r;
    double s_rat = 0.0;
    //check last line
    if (!last_line.empty()){
        split_string(last_line, rat_attr);
        if (rat_attr[0]  > s_id){
            rat_attr.clear();
            return s_rat;
        }
        if (rat_attr[0] == s_id) {
                s_rat = std::stod(rat_attr[1]);
            }
    }
    while ( getline(file_rat, line_r)) {
        last_line = line_r;
        split_string(line_r, rat_attr);
        if (rat_attr[0]  > s_id){
            rat_attr.clear();
            break;
        }
        if (rat_attr[0] == s_id) {
            s_rat = std::stod(rat_attr[1]);
        }
        rat_attr.clear();
        }
    return s_rat;
}

void choose_series(std::ifstream& file_bas, std::ifstream& file_rat, std::ifstream& file_title, int max_min, std::set<Series>& ser_set) {
    std::string line, line_t, line_r, last_r, last_t;
    std::vector<std::string> ser_attr;
    //pass first lines with headers
    getline(file_title, line_t);
    getline(file_rat, line_r);
    getline(file_bas, line);
    std::vector<std::string> title_attr;
    std::vector<std::string> rat_attr;
    const int n = 10;

    while (getline(file_bas, line)) {
        ser_attr.clear();
        split_string(line, ser_attr);
        if (ser_attr[4] != "0") //check if adult
            continue;
        if (!(ser_attr[1] == "tvSeries" && ser_attr[7] != "\\N")) //check if not a serial 
            continue;
        if (std::stoi(ser_attr[7]) > max_min)
            continue;
        std::string s_title = ser_attr[2];
        std::string s_id = ser_attr[0];
        double s_rat = 0.0;
        find_title(file_title,  s_id, s_title, last_t);
        s_rat = find_rating(file_rat, s_id, last_r);
        //add chosen series
        Series ser(s_id, s_title, s_rat);
        ser_set.insert(ser);
        if (ser_set.size() > n){
            ser_set.erase(ser_set.begin());
        }
    }
}


void split_string( std::string const& line, std::vector <std::string>& words) {
    std::stringstream ss;
    ss << line;
    std::string word;
    while (getline(ss, word, '\t')){
        words.push_back(word);
    }
}


void print_set(std::set<Series> const& set_obj) {
    for (auto rit = set_obj.rbegin(); rit != set_obj.rend(); ++rit)
        std::cout << (*rit).getTitle() << std::endl;
}


bool isNumeric(std::string const& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}


int input_params(int argc, char* argv[], std::ifstream& fbas, std::ifstream& frat, std::ifstream& faka ){
    std::string bas_col = "tconst	titleType	primaryTitle	originalTitle	isAdult	startYear	endYear	runtimeMinutes	genres";
    std::string title_col = "titleId	ordering	title	region	language	types	attributes	isOriginalTitle";
    std::string rat_col = "tconst	averageRating	numVotes";
    int minutes;
    int r = 0, t = 0, b = 0;

    if (argc != 5 ) {
        std::cerr << "Wrong amount of arguments" << std::endl;
        return -1;
    }
    for (int i = 1; i < 5; i++ ) {
        if (isNumeric(argv[i])) {
            minutes = std::stoi(argv[i]);
            continue;
        }
        std::ifstream fin(argv[i]);
        if (fin.is_open()) {
            std::string columns;
            getline(fin, columns);
            if (columns == bas_col) {
                fbas.open(argv[i]);
                b = 1;
                continue;
            }  
            if (columns == title_col){
                faka.open(argv[i]);
                t = 1;
                continue;
            }
            if (columns == rat_col) {
                frat.open(argv[i]);
                r = 1;
                continue;
            }
            fin.close();
        }
        else{
            std::cerr << "couldn't open file: " << argv[i] << std::endl;
            return -1;
        }
    }
    if (!(r && t && b)){
        std::cerr<< "Wrong files were passed"<< std::endl;
        return -1;
    }
    return minutes;
}