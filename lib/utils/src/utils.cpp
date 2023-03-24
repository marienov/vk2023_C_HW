#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include "utils.hpp"
using namespace std;



void choose_series(ifstream& file_bas, ifstream& file_rat, ifstream& file_title, int max_min, set<Series>* ser_set) {
    string line, line_t, line_r;
    vector<string> ser_attr;
    //pass first lines with headers
    getline(file_title, line_t);
    getline(file_rat, line_r);
    getline(file_bas, line);
    vector<string> title_attr;
    vector<string> rat_attr;

    while (getline(file_bas, line)) {
        ser_attr.clear();
        split_string(line, &ser_attr);
        if (ser_attr[4] != "0") //check if adult
            continue;
        if (!(ser_attr[1] == "tvSeries" && ser_attr[7] != "\\N")) //check if not a serial or 
            continue;
        if (stoi(ser_attr[7]) > max_min)
            continue;
        string s_title = ser_attr[2];
        string s_id = ser_attr[0];
        double s_rat = 0.0;
        //find russian title
        if (title_attr.empty()) {
            getline(file_title, line_t);
            split_string(line_t, &title_attr);
        }
        while (title_attr[0] <= s_id && getline(file_title, line_t)) {
            if (title_attr[0] == s_id && title_attr[3] == "RU") {
                s_title = title_attr[2];
                title_attr.clear();
                split_string(line_t, &title_attr);
            }
            title_attr.clear();
            split_string(line_t, &title_attr);
        }
        //find rating
        if (rat_attr.empty()) {
            getline(file_rat, line_r);
            split_string(line_r, &rat_attr);
        }
        while (rat_attr[0] <= s_id && getline(file_rat, line_r)) {
            if (rat_attr[0] == s_id) {
                s_rat = stod(rat_attr[1]);
                rat_attr.clear();
                split_string(line_r, &rat_attr);
            }
            rat_attr.clear();
            split_string(line_r, &rat_attr);
        }
        //add chosen series
        Series ser(s_id, s_title, s_rat);
        ser_set->insert(ser);
        if (ser_set->size() > 10)
            ser_set->erase(ser_set->begin());
    }

}


void split_string(string& line, vector <string>* words) {
    stringstream ss;
    ss << line;
    string word;
    while (getline(ss, word, '\t'))
        words->push_back(word);
}


void print_set(std::set<Series> const& set_obj) {
    set<Series>::reverse_iterator rit;
    for (rit = set_obj.rbegin(); rit != set_obj.rend(); rit++)
        cout << (*rit).getTitle() << endl;
}


bool isNumeric(string const& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}