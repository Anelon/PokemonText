#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
using namespace std;

class UserPoke;
class Pokemon;
class Move;
class Trainer;
class Region;
class Route;

//name of region to Region pointer
map<string,Region*> regions;
//id to name of region
map<int,string> regionIdMap;
//name of route to Route pointer
map<string,Route*> routes;
//id to name of route
map<int,string> routeIdMap;
//route name to region name
map<string,string> routeToRegion;
//Pokedex num to Pokemon
map<int,Pokemon*> pokedex;
//Name to Pokemon
map<string,Pokemon*> pokedexNames;
map<int,Move*> movedex;
map<string,Move*> movedexNames;
map<int,string> typeTable;
multimap<string,string> superEff;
multimap<string,string> halfEff;
multimap<string,string> notEff;

const int FIGHT = 1;
const int POKEMON = 2;;
const int BAG = 3;
const int RUN = 4;
const int MAX_MOVES = 4;
const int STARTERLVL = 5;

const string NORTH = "North";
const string SOUTH = "South";
const string EAST = "East";
const string WEST = "West";

const string WILD = "Wild";

void uppercasify(string &word) {
	bool first = true;
	for(char &c : word) {
		if(first) {
			c = toupper(c);
			first = false;
		} else {
			if(c == '-') {
				c = ' ';
				first = true;
			}
		}
	}
}
string giveNickName() {
	string nickName = "", choice;
	cout << "Would you like to nickname your Pokemon (y/n)?\n";
	cin >> choice;
	if(toupper(choice.at(0)) == 'Y') {
		cout << "What would you like to name your Pokemon?\n";
		cin >> nickName;
	}
	return nickName;
}
void die(string fail="");
void trainerBattle(Trainer &you, Trainer opp);
void next() {
	string in;
	cout << "\nContinue(Y/N)?\n";
	cin >> in;
	if(!cin || toupper(in.at(0)) == 'Q' || toupper(in.at(0)) == 'N') die();
}

/*
// could use for partial search of maps
TStrStrMap::const_iterator FindPrefix(const TStrStrMap& map, const string& search_for) {
    TStrStrMap::const_iterator i = map.lower_bound(search_for);
    if (i != map.end()) {
        const string& key = i->first;
        if (key.compare(0, search_for.size(), search_for) == 0) // Really a prefix?
            return i;
    }
    return map.end();
}

void Test(const TStrStrMap& map, const string& search_for) {
    cout << search_for;
    auto i = FindPrefix(map, search_for);
    if (i != map.end())
        cout << '\t' << i->first << ", " << i->second;
    cout << endl;
}
*/
