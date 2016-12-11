/*
 * 	The Organizer ALPHA version
 *  
 * 	Copyleft (c) 2014:
 * 	- Quim Martínez <joaquim.martinez.vidal@estudiant.upc.edu>
 * 	- Pol Mesalles <pol.mesalles@estudiant.upc.edu>
 * 	- Arnau Singla <arnau.singla@estudiant.upc.edu>
 * 	
 * 	This program is free software: you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation, either version 3 of the License, or
 * 	(at your option) any later version.
 * 	
 * 	This program is distributed in the hope that it will be useful,
 * 	but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 	GNU General Public License for more details.
 * 	
 * 	You should have received a copy of the GNU General Public License
 * 	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 	
 */

// Universal libraries
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Constants
#define N 10 // ALPHA version

// Global variables
int nTrips = 0;
string user = "";

// Data structure
struct Country {
	string name, comment, cities[N];
	int nCities;
};

struct Album {
	string path, tags[N];
	int nTags;
};

struct Trip {
	int start, end, nCountries, nAlbums; // 'start', 'end': dates; the rest: counters
	Country countries[N];
	Album albums[N];
	short int score; 
};

// OS-specific functions

#ifdef _WIN32

	void clearUp() {
		system("cls");
	}

#elif __APPLE__

	void clearUp() {
		system("clear");
	}

#elif __linux__

	void clearUp() {
		system("clear");
	}

#endif

// HELPER functions

int findByDate(Trip trips[N], const int& date) {
	for (int i = 0; i < nTrips; ++i) {
		if (trips[i].start == date) return i;
	}
	return -1; // error handling
}

int date2int(string str) {
	str.erase(remove(str.begin(), str.end(), '/'), str.end());
	return atoi(str.c_str());
}

string int2date(const int& date) {
	// int2stream
	stringstream ss;
	ss << date;

	// stream2string
	string str = ss.str();
	str.insert(4, "/");
	str.insert(7, "/");

	return str;
}

void errorArray(const string& array) {
	cout << "ERROR. You've reached the maximum number of possible " << array << ".\n";
	cout << "To continue using this software, upgrade to BETA.\n\n";
}

void errorDate() {
	cout << "ERROR. No existing trip matches the input date.\n";
}

void error404() {
	cout << "ERROR 404. Logic NOT found.\n";
	cout << "You could not have been in 2 places at the same time.\n";
	cout << "If you are not " << user << ", copy the *.cpp into a different folder and run it again.\n\n";
}

void errorSorry() {
	cout << "Sorry... what? ";
}

void displayMenu() {
	cout << "What would you like to do?\n\n";
	cout << "1. Add new trips.\n";
	cout << "2. Edit existing trips.\n";
	cout << "3. Remove existing trips.\n";
	cout << "4. Check my trips!\n";
	cout << "X. Exit...\n\n";

	cout << "Your move: ";
}

void displayEditMenu() {
	cout << "1. Departure date.\n";
	cout << "2. Arrival date.\n";
	cout << "3. Countries visited.\n";
	cout << "4. Photo albums.\n";
	cout << "5. General score for the trip.\n";
	cout << "X. Go back...\n\n";

	cout << "Your move: ";
}

void displaySearchMenu() {
	cout << "SEARCH CRITERIA\n\n";
	cout << "1. View all.\n";
	cout << "2. View by country.\n";
	cout << "3. View by score.\n";
	cout << "4. View by year.\n";
	cout << "X. Go back...\n\n";
	
	cout << "Your move: ";
}

// IMPORT / EXPORT functions

void readData(Trip trips[N], int dates[N], int scores[N]) {
	ifstream reader("data.dat");
	string str;

	getline(reader, user, '\n'); // recover user name
	
	// Build trips[] array
	getline(reader, str, '\n'); nTrips = atoi(str.c_str());
	for (int i = 0; i < nTrips && !reader.eof(); ++i) {
		getline(reader, str, '\t'); trips[i].start = atoi(str.c_str());
		getline(reader, str, '\t'); trips[i].end = atoi(str.c_str());
		getline(reader, str, '\t'); trips[i].nCountries = atoi(str.c_str());
		for (int j = 0; j < trips[i].nCountries; ++j) {
			getline(reader, trips[i].countries[j].name, '\t');
			getline(reader, str, '\t'); trips[i].countries[j].nCities = atoi(str.c_str());
			for (int k = 0; k < trips[i].countries[j].nCities; ++k) {
				getline(reader, trips[i].countries[j].cities[k], '\t');
			}
			getline(reader, trips[i].countries[j].comment, '\t');
		}
		getline(reader, str, '\t'); trips[i].nAlbums = atoi(str.c_str());
		for (int j = 0; j < trips[i].nAlbums; ++j) {
			getline(reader, trips[i].albums[j].path, '\t');
			getline(reader, str, '\t'); trips[i].albums[j].nTags = atoi(str.c_str());
			for (int k = 0; k < trips[i].albums[j].nTags; ++k) {
				getline(reader, trips[i].albums[j].tags[k], '\t');
			}
		}
		getline(reader, str, '\n'); trips[i].score = atoi(str.c_str());

		// Build hash
		dates[i] = i;
		scores[i] = i;
	}
}

void writeData(Trip trips[N], int dates[N]) {
	ofstream writer;
	writer.open("data.dat");

	writer << user << '\n'; // save user name

	// Save trips[]
	writer << nTrips << '\n';
	for (int i = 0; i < nTrips; ++i) {
		writer << trips[dates[i]].start << '\t';
		writer << trips[dates[i]].end << '\t';
		writer << trips[dates[i]].nCountries << '\t';
		for (int j = 0; j < trips[dates[i]].nCountries; ++j) {
			writer << trips[dates[i]].countries[j].name << '\t';
			writer << trips[dates[i]].countries[j].nCities << '\t';
			for (int k = 0; k < trips[dates[i]].countries[j].nCities; ++k) {
				writer << trips[dates[i]].countries[j].cities[k] << '\t';
			}
			writer << trips[dates[i]].countries[j].comment << '\t';
		}
		writer << trips[dates[i]].nAlbums << '\t';
		for (int j = 0; j < trips[dates[i]].nAlbums; ++j) {
			writer << trips[dates[i]].albums[j].path << '\t';
			writer << trips[dates[i]].albums[j].nTags << '\t';
			for (int k = 0; k < trips[dates[i]].albums[j].nTags; ++k) {
				writer << trips[dates[i]].albums[j].tags[k] << '\t';
			}
		}
		writer << trips[dates[i]].score << '\n';
	}
}

// ADD / EDIT functions

int addDate() {
	string str;
	cin >> str;
	while (str.size() != 10 || str[4] != '/' || str[7] != '/') {
		cout << "   ERROR. Wrong date format (yyyy/mm/dd): ";
		cin >> str;
	}
	return date2int(str);
}

void addCountries(Trip& trip) {
	trip.nCountries = 0;
	int i = 0, j;

	cout << "3. Countries visited ('done' to end):\n";
	cout << " 3.1. Name: ";
	cin.ignore();
	getline(cin, trip.countries[i].name);

	while (i < N && trip.countries[i].name != "done") {
		trip.countries[i].nCities = 0;
		++trip.nCountries;
		j = 0;
		cout << " 3.2. And its cities ('done' to end): ";
		getline(cin, trip.countries[i].cities[j]);
		while (j < N && trip.countries[i].cities[j] != "done") {
			++trip.countries[i].nCities;
			++j;
			cout << "                                      ";
			getline(cin, trip.countries[i].cities[j]);
		}
		if (j == N) errorArray("cities");

		cout << " 3.3. Any comments about the country? ";
		getline(cin, trip.countries[i].comment);
		
		++i;
		cout << "3. More countries visited ('done' to end):\n";
		cout << " 3.1. Name: ";
		getline(cin, trip.countries[i].name);
	}
	if (i == N) errorArray("countries");
}

void addAlbums(Trip& trip) {
	trip.nAlbums = 0;
	int i = 0, j;

	cout << "4. Photo albums ('done' to end):\n";
	cout << " 4.1. Folder path: ";
	getline(cin, trip.albums[i].path);

	while (i < N && trip.albums[i].path != "done") {
		trip.albums[i].nTags = 0;
		++trip.nAlbums;
		j = 0;
		cout << "  4.1.1. 1-word tags (spaced; 'done' to end): ";
		cin >> trip.albums[i].tags[j];
		while (j < N && trip.albums[i].tags[j] != "done") {
			++trip.albums[i].nTags;
			++j;
			cin >> trip.albums[i].tags[j];
		}
		if (j == N) errorArray("tags");

		++i;
		cout << " 4.1. Another folder path ('done' to end): "; // IDEA: que pregunti "Another folder path?" i responguis folderName OR "no"
		cin.ignore();
		getline(cin, trip.albums[i].path);
	}
	if (i == N) errorArray("photo albums");
}

void addScore(Trip& trip) {
	cout << "5. General score for the trip [0, 5]: ";
	cin >> trip.score; // cin.ignore(); // if necessary
	while (trip.score < 0 || trip.score > 5) {
		cout << "   ERROR. Score must be between [0, 5]: ";
		cin >> trip.score;
	}
}

void add(Trip trips[N], Trip &trip, int& index) {
	clearUp();
	cout << "ADD A NEW TRIP\n\n";

	cout << "1. Departure date (yyyy/mm/dd): ";
	index = addDate();
	
	if (findByDate(trips, index) == -1) {
		trip.start = index;

		cout << "2. Arrival date (yyyy/mm/dd): ";
		trip.end = addDate();

		addCountries(trip);
		addAlbums(trip);
		addScore(trip);
	}
}

// REMOVE functions

int findHash(int hash[N], const int& index) {
	for (int i = 0; i < nTrips; ++i) {
		if (hash[i] == index) return i;
	}
	// return -1; // cannot fail
}

void moveHash(int hash[N], const int& hashIndex) {
	for (int i = hashIndex; i < nTrips-1; ++i) {
		hash[i] = hash[i+1];
	}
}

void modHash(int hash[N], const int& index) {
	for (int i = 0; i < nTrips; ++i) {
		if (hash[i] > index) --hash[i];
	}
}

void erase(Trip trips[N], int dates[N], int scores[N], const int& index) {
	// Remove trip from trips[]
	for (int i = index; i < nTrips-1; ++i) {
		trips[i] = trips[i+1];
	}

	// Update hash(s)
	int dateIndex = findHash(dates, index), scoreIndex = findHash(scores, index);
	moveHash(dates, dateIndex); moveHash(scores, scoreIndex);
	modHash(dates, index); modHash(scores, index);

	--nTrips; // update global counter

	clearUp();
	cout << "Your trip details have been removed.\n\n";
}

// SORTING functions

void quickSortDates(Trip trips[N], int dates[N], const int& first, const int& last) {
	int i = first, j = last, mid = trips[dates[(first + last) / 2]].start; // quickSort indices
	int copy; // auxiliary variable for swapping

	// Build the "partition"
	while (i <= j) {
		while (trips[dates[i]].start < mid) ++i;
		while (trips[dates[j]].start > mid) --j;
		
		if (i <= j) { // no need to change order when 2 departure dates are equal, because you cannot be in 2 places at the same time! (personal application)
			copy = dates[i];
			dates[i] = dates[j];
			dates[j] = copy;
			++i; --j;
		}
	}

	// Recursion
	if (first < j) quickSortDates(trips, dates, first, j);
	if (last > i) quickSortDates(trips, dates, i, last);
}

void quickSortScores(Trip trips[N], int scores[N], const int& first, const int& last) {
	int i = first, j = last; // quickSort indices
	short int mid = trips[scores[(first + last) / 2]].score;
	int copy; // auxiliary variable for swapping

	// Build the "partition"
	while (i <= j) {
		while (trips[scores[i]].score < mid) ++i;
		while (trips[scores[j]].score > mid) --j;
		
		if (i <= j) {
			copy = scores[i];
			scores[i] = scores[j];
			scores[j] = copy;
			++i; --j;
		}
	}

	// Recursion
	if (first < j) quickSortScores(trips, scores, first, j);
	if (last > i) quickSortScores(trips, scores, i, last);
}

void sortScores(Trip trips[N], int scores[N]) {
	quickSortScores(trips, scores, 0, nTrips-1);

	// To preserve date order inside each score...
	int j, k = 0;
	for (int i = trips[scores[0]].score; i < trips[scores[nTrips-1]].score; ++i) {
		j = k;
		while (trips[scores[j]].score == trips[scores[j+1]].score && j < nTrips-1) {
			++j;
		}
		quickSortDates(trips, scores, k, j);
		k = j+1;
	}
}

// SHOW functions

void printBasic(const Trip& trip) {
	cout << int2date(trip.start) << " - " << int2date(trip.end) << ": ";
	for (int i = 0; i < trip.nCountries; ++i) {
		if (i != 0) cout << "                         ";
		cout << trip.countries[i].name << ": ";
		for(int j = 0; j < trip.countries[i].nCities; ++j) {
			cout << trip.countries[i].cities[j];
			if (j != trip.countries[i].nCities-1) cout << ", ";
		}
		cout << endl;
	}
}

void printAll(const Trip& trip){
	cout << int2date(trip.start) << " - " << int2date(trip.end) << "\n\n";

	cout << "Countries visited: ";
	for (int i = 0; i < trip.nCountries; ++i) {
		if (i != 0) cout << "                   ";
		cout << trip.countries[i].name << ": ";
		for(int j = 0; j < trip.countries[i].nCities; ++j) {
			cout << trip.countries[i].cities[j];
			if (j != trip.countries[i].nCities-1) cout << ", ";
			else cout << endl;
		}
		cout << "                   \u21b3 " << "\"" << trip.countries[i].comment << "\"";
		cout << endl;
	}

	cout << "Photo albums: ";
	for (int i = 0; i < trip.nAlbums; ++i) {
		if (i != 0) cout << "              ";
		cout << trip.albums[i].path << ": ";
		for(int j = 0; j < trip.albums[i].nTags; ++j) {
			cout << trip.albums[i].tags[j];
			if (j != trip.albums[i].nTags-1) cout << ", ";
		}
		cout << endl;
	}

	cout << "General score: ";
	for (int i = 0; i < trip.score; ++i) {
		cout << "*";
	}
	cout << endl;
}

// MAIN

int main() {
	// Arrays
	Trip trips[N];
	int dates[N], scores[N]; // hash(s)

	// Menu variables
	string option;
	int index;
	bool found, error;

	readData(trips, dates, scores); // import data (if available)

	clearUp();

	// Willkommen
	if (user == "") {
		cout << "What's your name? ";
		getline(cin, user);

		clearUp();
		cout << "Welcome, " << user << "!\n\n";
	} else cout << "Welcome back, " << user << "!\n\n";

	// MAIN menu
	displayMenu();
	cin >> option;

	error = false;

	while (option != "X" && option != "x") {
		if (option == "1") {
			option = "yes";
			while (option != "no" && option != "n") {
				error = false;
				if (option == "yes" || option == "y") {
					if (nTrips < N) {
						add(trips, trips[nTrips], index);
						if (findByDate(trips, index) == -1) {
							dates[nTrips] = nTrips; // update dates[] hash
							scores[nTrips] = nTrips; // update scores[] hash
							++nTrips; // update global counter

							clearUp();
							cout << "Your trip details have been saved.\n\n";
							cout << "Add another trip? (yes / no)? ";
							cin >> option;
						} else {
							clearUp();
							error404();
							cout << "Try again (yes / no)? ";
							cin >> option;
						}
					} else {
						clearUp();
						errorArray("trips");
						option = "no";
						error = true;
					}
				} else {
					errorSorry();
					cin >> option;
				}
			}
		} else if (option == "2") {
			option = "yes";
			while (option != "no" && option != "n") {
				if (option == "yes" || option == "y") {
					clearUp();
					cout << "EDIT AN EXISTING TRIP\n\n";
					cout << "What trip would you like to edit?\n";
					cout << "Enter its (current) departure date (yyyy/mm/dd): ";
					cin >> option;
					index = findByDate(trips, date2int(option));
					if (index >= 0) {
						clearUp();
						cout << "EDIT AN EXISTING TRIP\n\n";
						cout << "What would you like to edit?\n\n";
						displayEditMenu();
						cin >> option;
						while (option != "X" && option != "x") {
							clearUp();
							error = false;

							if (option == "1") {
								cout << "1. New departure date (yyyy/mm/dd): ";
								int date = addDate();
								if (findByDate(trips, date) == -1) {
									trips[index].start = date;
								} else {
									clearUp();
									error404();
									error = true;
								}
							} else if (option == "2") {
								cout << "2. New arrival date (yyyy/mm/dd): ";
								trips[index].end = addDate();
							} else if (option == "3") {
								addCountries(trips[index]);
							} else if (option == "4") {
								cin.ignore();
								addAlbums(trips[index]);
							} else if (option == "5") {
								addScore(trips[index]);
							} else {
								errorSorry();
							}

							if (!error) {
								clearUp();
								cout << "Your trip details have been updated.\n\n";
							}

							cout << "What else would you like to edit?\n\n";
							displayEditMenu();
							cin >> option;
						}
						clearUp();
						cout << "Edit another trip (yes / no)? ";
					} else {
						clearUp();
						errorDate();
						cout << "Try again (yes / no)? ";
					}
				} else {
					errorSorry();
				}
				cin >> option;
			}
		} else if (option == "3") {
			option = "yes";
			while (option != "no" && option != "n") {
				if (option == "yes" || option == "y") {
					clearUp();
					cout << "REMOVE AN EXISTING TRIP\n\n";
					cout << "What trip would you like to get rid of?\n";
					cout << "Enter its departure date (yyyy/mm/dd): ";
					cin >> option;
					index = findByDate(trips, date2int(option));
					if (index >= 0) {
						erase(trips, dates, scores, index);
						cout << "Remove another trip (yes / no)? ";
					} else {
						clearUp();
						errorDate();
						cout << "Try again (yes / no)? ";
					}
				} else {
					errorSorry();
				}
				cin >> option;
			}
		} else if (option == "4") {
			if (nTrips > 0) {
				// Sort trips
				quickSortDates(trips, dates, 0, nTrips-1);
				sortScores(trips, scores);

				// SEARCH menu
				clearUp();
				displaySearchMenu();
				cin >> option;
				clearUp();

				while (option != "X" && option != "x") {
					error = true;
					if (option == "1") {
						error = false;
						clearUp();
						cout << "SEARCH CRITERIA\n\n";
						cout << "1. Sort by date (old-new).\n";
						cout << "2. Sort by date (new-old).\n";
						cout << "3. Sort by score (worse-better).\n";
						cout << "4. Sort by score (better-worse).\n";
						cout << "X. Go back...\n\n";

						cout << "Your move: ";
						cin >> option;

						if (option == "1") {
							clearUp();
							cout << "CHECK MY TRIPS\n\n";
							for (int i = 0; i < nTrips; ++i) {
								printBasic(trips[dates[i]]);
							}
						} else if (option == "2") {
							clearUp();
							cout << "CHECK MY TRIPS\n\n";
							for (int i = nTrips-1; i >= 0; --i) {
								printBasic(trips[dates[i]]);
							}
						} else if (option == "3") {
							clearUp();
							cout << "CHECK MY TRIPS\n\n";
							for (int i = 0; i < nTrips; ++i) {
								printBasic(trips[scores[i]]);
							}	
						} else if (option == "4") {
							clearUp();
							cout << "CHECK MY TRIPS\n\n";
							for (int i = nTrips-1; i >= 0; --i) {
								printBasic(trips[scores[i]]);
							}
						} else {
							clearUp();
							error = true;
						}
					} else if (option == "2") {
						clearUp();
						cout << "CHECK MY TRIPS\n\n";
						cout << "Country name: ";
						cin.ignore();
						getline(cin, option);
						cout << endl;

						for (int i = 0; i < nTrips; ++i) {
							found = false;
							for (int j = 0; j < trips[dates[i]].nCountries && !found; ++j) {
								if (trips[dates[i]].countries[j].name == option){
									printBasic(trips[dates[i]]);
									found = true;
									error = false;
								}
							}
						}

						if (error) {
							clearUp();
							cout << "ERROR. No existing trip matches the input country.\n\n";
						}
					} else if (option == "3") {
						clearUp();
						cout << "CHECK MY TRIPS\n\n";
						cout << "Score value [0, 5]: ";
						cin >> option;

						if (atoi(option.c_str()) >= 0 && atoi(option.c_str()) <= 5) {
							cout << endl;
							for (int i=0; i < nTrips; ++i) {
								if (trips[dates[i]].score == atoi(option.c_str())){
									printBasic(trips[dates[i]]);
									error = false;
								}
							}
						}

						if (error) {
							clearUp();
							cout << "ERROR. Score must be a valid value between [0, 5].\n\n";
						}
					} else if (option == "4") {
						clearUp();
						cout << "CHECK MY TRIPS\n\n";
						cout << "Year (i.e., 2010) or interval (i.e., 2010-2014): ";
						cin >> option;

						if (option.size() == 9) {
							cout << endl;
							for (int i = 0; i < nTrips; ++i) {
								found = false;
								for (int j = trips[dates[i]].start/10000; j <= trips[dates[i]].end/10000 && !found; ++j) {
									for (int k = atoi((option.substr(0, 4)).c_str()); k <= atoi((option.substr(5)).c_str()) && !found; ++k) {
										if (k == j) {
											printBasic(trips[dates[i]]);
											found = true;
											error = false;
										}
									}
								}
							}
						} else if (option.size() == 4) {
							cout << endl;
							for (int i = 0; i < nTrips; ++i) {
								found = false;
								for (int j = trips[dates[i]].start/10000; j <= trips[dates[i]].end/10000 && !found; ++j) {
									if (j == atoi(option.c_str())) {
										printBasic(trips[dates[i]]);
										found = true;
										error = false;
									}
								}
							}
						}

						if (error) {
							clearUp();
							cout << "ERROR. You must enter either a valid year (i.e., 2010) or a valid interval of years (i.e., 2010-2014).\n\n";
						}
					} else clearUp();

					// More details
					if (!error) {
						cout << "\nWould you like to see more details of a trip (yes / no)? ";
						cin >> option;

						while (option != "no" && option != "n") {
							if (option == "yes" || option == "y") {
								cout << "Enter its departure date (yyyy/mm/dd): ";
								cin >> option;
								index = findByDate(trips, date2int(option));
								if (index >= 0) {
									clearUp();
									printAll(trips[index]);
									cout << "\nWould you like to see more details of another trip (yes / no)? ";
								} else {
									clearUp();
									errorDate();
									cout << "Try again (yes / no)? ";
								}
							} else {
								errorSorry();
							}
							cin >> option;
						}
					}

					if (!error) clearUp();
					displaySearchMenu();
					cin >> option;
				}
				error = false;
			} else {
				clearUp();
				cout << "ERROR. No trips have been indexed yet.\n\n";
				error = true;
			}
		}

		if (!error) clearUp();
		displayMenu();
		cin >> option;
	}

	clearUp();
	cout << "Keep changes (yes / no)? ";
	cin >> option;

	while (option != "yes" && option != "y" && option != "no" && option != "n") {
		errorSorry();
		cin >> option;
	}
	
	if (option == "yes" || option == "y") {
		writeData(trips, dates);
	}

	clearUp();
	return 0;
}
