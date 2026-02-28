// Winter'24
// Instructor: Diba Mirza
// Student name: Satvik Balakrishnan
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <sstream>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    // Create an object of a STL data-structure to store all the movies
    vector<Movie> movies;

    string line, movieName;
    double movieRating;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure

            movies.push_back({movieName, movieRating});
    }

    movieFile.close();
    sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b){return a.name < b.name;});

    if (argc == 2) {

        sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b){ 
            return a.name < b.name; 
        });

        for (const auto& mov : movies){ 
            cout << mov.name << ", " << fixed << setprecision(1) << mov.rating << endl; 
        }

        return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    vector<Movie> bestMovies;
    vector<string> bestPrefixes;

    for(const string& prefix : prefixes){

        auto low = lower_bound(movies.begin(), movies.end(), prefix, [](const Movie& m, const string& value){return m.name < value;});
        string upper = prefix;
        upper.push_back(char(127));

        auto high = lower_bound(movies.begin(), movies.end(), upper, [](const Movie& m, const string& value){return m.name < value;});


        vector<Movie> matches(low, high);

        if (matches.empty()) {
            cout << "No movies found with prefix " << prefix << endl;
            continue;
        }

        sort(matches.begin(), matches.end(), [](const Movie& a, const Movie& b){

            if (a.rating != b.rating) return a.rating > b.rating;
            return a.name < b.name;

        });

        for (const auto& mov : matches){
            cout << mov.name << ", " << fixed << setprecision(1) << mov.rating << endl;
        }
        cout << endl; 

        
        bestMovies.push_back(matches[0]);
        bestPrefixes.push_back(prefix);

    }

    for (int i = 0; i < (int)bestMovies.size(); i++) {
        cout << "Best movie with prefix " << bestPrefixes[i] << " is: " << bestMovies[i].name << " with rating " << fixed << setprecision(1) << bestMovies[i].rating << endl;
    }


    return 0;
}

/* 

PART 3A:

Let:
n = number of movies
m = number of prefixes
k = maximum number of movies matching a prefix
l = maximum length of a movie name

For each prefix:

1) We use lower_bound twice to find the matching range.
   Each lower_bound is O(log n) comparisons.
   Each comparison is O(l) for string comparison.
   Thus this step is O(l log n).

2) We copy the matching range of size k into a vector.
   This is O(k).

3) We sort the k matches by rating (and name if tied).
   Sorting consists O(k log k).
   Each comparison could involve O(l) string comparison.
   Therefore this is O(l k log k).

Therefore, for one prefix the total time is:

O(l log n + l k log k)

For m prefixes, the total worst-case running time is:

O(m (l log n + l k log k))


PART 3B:

We store:

- n movies in a vector → O(n)
- m prefixes → O(m)
- k matches per prefix (temporary vector) → O(k)
- best movie per prefix → O(m)

Therefore, the total space complexity is:

O(n + m + k)

PART 3C:

This algorithm was designed primarily for low time complexity.

Instead of scanning all n movies for every prefix (which would cost O(mn)),
we sort the movies once and use binary search (lower_bound) to find
only the relevant range of matches.

The space complexity remains reasonable (O(n + m + k)),
since we only store the dataset and temporary matches.

Achieving low time complexity was more important for this assignment,
especially since grading emphasizes efficiency.

Low time complexity was harder to achieve than low space complexity,
because naive solutions easily lead to O(mn) behavior.

*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}