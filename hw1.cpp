#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
using namespace std;


class Series {
private:
	double rating;
	string id;
	string title;
public:
	Series(string id_, string title_, double rating_ ) : id(id_), title(title_), rating(rating_) {}
	string getId() const { return id; }
	string getTitle() const { return title; }
	bool operator<(const Series& series) const {
		if (rating >= series.rating)
			return false;
		else
			return true;
	}
};


void split_string(string& line, vector <string>* words) {
	stringstream ss;
	ss << line;
	string word;
	while (getline(ss, word, '\t'))
		words->push_back(word);
}

void choose_series(ifstream& file_bas, ifstream& file_rat, ifstream& file_title, int max_min, set <Series>* ser_set) {
	string line, line_r, line_t;
	vector<string> ser_attr;

	getline(file_bas, line);
	getline(file_rat, line);

	while (getline(file_bas, line)) {
		getline(file_bas, line);
		ser_attr.clear();
		split_string(line, &ser_attr);
		if (ser_attr[1] == "tvSeries" && ser_attr[7] != "\\N") {
			string s_title = ser_attr[2];
			if (stoi(ser_attr[7]) <= max_min) {
				string s_id = ser_attr[0];
				double s_rat = 0.0;
				//поиск рейтинга
				vector<string> rat_attr; 
				getline(file_rat, line_r);
				split_string(line_r, &rat_attr);
				while (rat_attr[0] <= s_id && getline(file_rat, line_r)) {
					if (rat_attr[0] == s_id) {
						s_rat = stod(rat_attr[1]);
					}
					rat_attr.clear();
					split_string(line_r, &rat_attr);
				}
				//поиск заголовка
				vector<string> title_attr;
				getline(file_title, line_t);
				split_string(line_t, &title_attr);
				while (title_attr[0] <= s_id && getline(file_title, line_t)) {
					if (title_attr[0] == s_id && title_attr[3] == "RU") {
						s_title = title_attr[2];
					}
					title_attr.clear();
					split_string(line_t, &title_attr);
				}
				//добавление сериала в множество выбранных сериалов
				Series ser(s_id, s_title, s_rat);
				ser_set->insert(ser);
				if (ser_set->size() > 10)
					ser_set->erase(ser_set->begin());
			}
		}		
	}
}


void print_set(std::set<Series> const& set_obj){
	set<Series>::reverse_iterator rit;
	for (rit = set_obj.rbegin(); rit != set_obj.rend(); rit++)
		cout << (*rit).getTitle() << endl;
}


bool isNumeric(string const& str){
	return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}


int main(int argc, char* argv[]) {
	string bas_col = "tconst	titleType	primaryTitle	originalTitle	isAdult	startYear	endYear	runtimeMinutes	genres";
	string title_col = "titleId	ordering	title	region	language	types	attributes	isOriginalTitle";
	string rat_col = "tconst	averageRating	numVotes";

	ifstream fbas;
	ifstream faka;
	ifstream frat;
	int minutes = 10;
	if (argc != 5 ) {
		cerr << "Wrong amount of arguments" << endl;
		return 0;
	}

	for (int i = 1; i < 5; i++ ) {
		if (isNumeric(argv[i])) {
			minutes = stoi(argv[i]);
		}
		else {
			ifstream fin(argv[i]);
			string columns;
			getline(fin, columns);
			if (columns == bas_col){
				fbas.open(argv[i]);
			}
			if (columns == title_col)
				faka.open(argv[i]);
			if (columns == rat_col)
				frat.open(argv[i]);
		}
	}

	set<Series> chosen;
	choose_series(fbas, frat, faka, minutes, &chosen);
	print_set(chosen);
}

