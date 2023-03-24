#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include "utils.hpp"
using namespace std;




int main(int argc, char* argv[]) {
    string bas_col = "tconst	titleType	primaryTitle	originalTitle	isAdult	startYear	endYear	runtimeMinutes	genres";
    string title_col = "titleId	ordering	title	region	language	types	attributes	isOriginalTitle";
    string rat_col = "tconst	averageRating	numVotes";

    ifstream fbas;
    ifstream faka;
    ifstream frat;
    int minutes;

    if (argc != 5 ) {
        cerr << "Wrong amount of arguments" << endl;
        return 0;
    }

    for (int i = 1; i < 5; i++ ) {
        if (isNumeric(argv[i])) {
            minutes = stoi(argv[i]);
            continue;
        }
        ifstream fin(argv[i]);
        if (fin.is_open()) {
            string columns;
            getline(fin, columns);
            if (columns == bas_col) {
                fbas.open(argv[i]);
            }
                
            if (columns == title_col){
                faka.open(argv[i]);
            }
                
            if (columns == rat_col) {
                frat.open(argv[i]);
            }
            fin.close();
        }
        else
            cerr << "couldn't open file: " << argv[i] << endl;
    }
    set<Series> chosen;
    choose_series(fbas, frat, faka, minutes, &chosen);
    print_set(chosen);
    
    frat.close();
    fbas.close();
    faka.close();
}

