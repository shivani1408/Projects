#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>


using namespace std;

/* 

Program that reads in an input of a grid, or "map", with characters each representing a different "Country". The program
outputs a valid 4-coloring of the grid, such that none of the countries touching each other on the map share the same color. 

In the input example, A - H represent different countries, and their layout is depicted on the grid below.

Sample Input:
8 4 10          // # of countries, # of rows, # of columns
AAAAAAAAAA
AHHHGGFFFA
ABBCCDDEEA
AAAAAAAAAA

Sample Output:  // Valid 4-coloring of input grid
A 1
B 2
C 3
D 4
E 2
F 3
G 2
H 4


*/

typedef map<char, int> ANSWER_MAP_T;
void printSolution(ANSWER_MAP_T& answer, std::ostream& ostr);

// Helper functions
map<char, set<char>> readInput(std::ifstream &in, map<char, set<char>> &countries); 
bool validLoc(int n_index, int m_index, int n, int m);
bool backtrack(map<char, set<char>> countryMap, ANSWER_MAP_T &colors, vector<char> countrylist, int index);
bool find_colors(map<char, set<char>> countryMap, ANSWER_MAP_T &colors);
bool isValid(map<char, set<char>> &countryMap, ANSWER_MAP_T &colors);

int main(int argc, char* argv[])
{
    if(argc < 3) {
        cout << "Please supply a map input file and output filename" << endl;
        return 1;
    }

    ifstream in(argv[1]);

    map<char, set<char>> countries;
    countries = readInput(in, countries);

    ofstream out(argv[2]);

    ANSWER_MAP_T colors;
    bool final = find_colors(countries, colors);
   
    // Call printSolution to output the answers to the output file
    if (final) printSolution(colors, out);

    return 0;
}


void printSolution(ANSWER_MAP_T& answer, std::ostream& os)
{
    for(ANSWER_MAP_T::iterator it = answer.begin();
            it!= answer.end();
            ++it) {
        os << it->first << " " << it->second << endl;
    }
}

/* 
Calls the backtracking recursive function "backtrack".
Returns a map with keys of countries and values of the corresponding color value (a number from 1 - 4)
*/
bool find_colors(map<char, set<char>> countryMap, ANSWER_MAP_T &colors)
{
    map<char, set<char>>::iterator it;

    vector<char> countrylist;

    for (it = countryMap.begin(); it != countryMap.end(); ++it)
        countrylist.push_back(it->first);
    
    int index = 0;
    bool check = backtrack(countryMap, colors, countrylist, index);
    return check;
}

/* 
Performs the backtracking algorithm to correctly assign colors to each of the countries,
while checking that none of the bordering countries have the same color.
Returns after finding one valid coloring.
*/
bool backtrack(map<char, set<char>> countryMap, ANSWER_MAP_T &colors, vector<char> countrylist, int index)
{
    if (colors.size() == countryMap.size()) 
    {
        return true;
    }

    for (int i = 1; i < 5; i++)
    {
        colors.insert({countrylist[index], i});
    
        if (isValid(countryMap, colors))
        {
            bool check = backtrack(countryMap, colors, countrylist, index + 1);
            if (check == true) return true;
        }

        colors.erase(countrylist[index]);
    }   
    return false;

}

/* Reads the input grid and outputs a map with keys of countries 
and values to a corresponding set of bordering countries */
map<char, set<char>> readInput(std::ifstream &in,  map<char, set<char>> &countries)
{
    int num, n , m;
    in >> num >> n >> m;

    char** countryMap = new char*[n];
    for (int i = 0; i < n; i++)
    {
        countryMap[i] = new char[m];
    }

    string line;
    int index = 0;
    getline(in, line);

    while(getline(in, line))
    {
        for (int k = 0; k < m; k++)
        {
            countryMap[index][k] = line[k];
        }
        index++;
    }

    in.close();

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            map<char, set<char>>::iterator it = countries.find(countryMap[i][j]);
            if (it == countries.end())
            {
                set<char> borders;
                countries.insert({countryMap[i][j], borders});
            }

            it = countries.find(countryMap[i][j]);
            set<char>::iterator cit;
            if (validLoc(i+1, j, n, m))
            {
                if (countryMap[i][j] != countryMap[i+1][j])
                {
                    cit = (it->second).find(countryMap[i+1][j]);
                    if (cit == (it->second).end())
                        (it->second).insert(countryMap[i+1][j]);
                }
            }
            if (validLoc(i-1, j, n, m))
            {
                if (countryMap[i][j] != countryMap[i-1][j])
                {
                    cit = (it->second).find(countryMap[i-1][j]);
                    if (cit == (it->second).end())
                        (it->second).insert(countryMap[i-1][j]);
                }
            }
            if (validLoc(i, j+1, n, m))
            {
                if (countryMap[i][j] != countryMap[i][j+1])
                {
                    cit = (it->second).find(countryMap[i][j+1]);
                    if (cit == (it->second).end())
                        (it->second).insert(countryMap[i][j+1]);
                }
            }
            if (validLoc(i, j-1, n, m))
            {
                if (countryMap[i][j] != countryMap[i][j-1])
                {
                    cit = (it->second).find(countryMap[i][j-1]);
                    if (cit == (it->second).end())
                        (it->second).insert(countryMap[i][j-1]);
                }
            }
        }

    }

    return countries;
}

/* Determines the indices correspond to a valid location on the grid */
bool validLoc(int n_index, int m_index, int n, int m)
{
    if (n_index < 0 || n_index > (n - 1)) return false;
    if (m_index < 0 || m_index > (m - 1)) return false;
    return true;
}

/* Determines whether the current coloring is valid, 
meaning no bordering countries in our answer map have the same color */
bool isValid(map<char, set<char>> &countryMap, ANSWER_MAP_T &colors)
{
    for (ANSWER_MAP_T::iterator it = colors.begin(); it != colors.end(); ++it)
    {
        map<char, set<char>>::iterator check = countryMap.find(it->first);
        for (set<char>::iterator sit = (check->second).begin(); sit != (check->second).end(); ++sit)
        {
            ANSWER_MAP_T::iterator verify = colors.find(*sit);
            if (verify != colors.end())
            {
               if (it->second == verify->second) return false; 
            }
        }
    }

    return true;
}
