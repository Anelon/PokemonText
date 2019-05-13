#include "trainer.h"

class Encounter {
	private:
		int min, max;
		Pokemon* pokemon;
	public:
		bool operator==(Encounter rhs) {
			return (this->getPokemon() == rhs.getPokemon());
		}
		Pokemon *getPokemon() {return pokemon;}
		UserPoke getUserPoke() {
			int range = max-min;
			int lvl;
			cout << "LVL range: " << range << endl;
			if(range) {
				lvl = (rand() % range) + min;
			} else {
				lvl = min;
			}
			return UserPoke(*pokemon, lvl);
		}
		int getMin() {return min;}
		int getMax() {return max;}
		Encounter(Pokemon* poke, int newMin, int newMax) {
			pokemon = poke;
			min = newMin;
			max = newMax;
		}
		void update(int newMax, int newMin) {
			if(min > newMin) min = newMin;
			if(max < newMax) max = newMax;
		}
		void print() {
			pokemon->print();
			cout << " Min LVL: " << min << " Max LVL: " << max << endl;
		}
};

vector<string> cityTypes = {"City", "Town"};

class Route {
	private:
		vector<Encounter> pokemons;//what can be found
		Route* north; //what the route connects to
		Route* south; //what the route connects to
		Route* east; //what the route connects to
		Route* west; //what the route connects to
		string name, notes;
		bool city = false;
	public:
		void cityCheck(string name) {
			stringstream ss(name);
			while(ss) {
				string temp;
				ss >> temp;
				for(string &s: cityTypes) {
					if(s == temp) city = true;
				}
			}
		}
		Route(string newName) {
			name = newName;
			cityCheck(name);
		}
		Route(vector<string> line);
		string getName() {return name;}
		bool isCity() {return city;}
		void setRoute(vector<string> line);
		//for those cities to veryfy that they have right right connections
		void connectNorth(Route* newSouth) {if(!south) south = newSouth;}
		void connectEast(Route* newWest) {if(!west) west = newWest;}
		void connectWest(Route* newEast) {if(!east) east = newEast;}
		void connectSouth(Route* newNorth) {if(!north) north = newNorth;}
		void addPoke(Encounter enc) {
			//if pokemon is already in update min and max levels
			for(Encounter &e : pokemons) {
				if(e == enc) {
					e.update(enc.getMax(), enc.getMin());
					return;
				}
			}
			pokemons.push_back(enc);
		}
		void printAll() {
			print();
			printEncounters(); //might get moved later
		}
		void print() {
			cout << name << ": " << notes << endl;
			if(north) cout << "North: " << north->getName() << endl;
			if(south) cout << "South: " << south->getName() << endl;
			if(east)  cout << "East: " << east->getName() << endl;
			if(west)  cout << "West: " << west->getName() << endl;
		}
		Trainer getEncounter() {
			UserPoke temp = pokemons.at(rand() % pokemons.size()).getUserPoke();
			return Trainer(WILD, temp);
		}
		void printEncounters() {
			cout << "Pokemon Spawns: \n";
			for(Encounter e : pokemons) e.print();
		}
		Route* move(string dir) {
			if(dir == NORTH) if(north) return north->enter();
			if(dir == EAST) if(east) return east->enter();
			if(dir == WEST) if(west) return west->enter();
			if(dir == SOUTH) if(south)return south->enter();
			return this;
		}
		Route* enter() {
			return this;
		}
};

Route::Route(vector<string> line) {
	name = line.at(0);
	setRoute(line);
	cityCheck(name);
}

string origin = "Kanto";
const string ROUTE = "Route";
void fixRoute(string &words) {
	stringstream ss(words);
	string temp;
	ss >> temp;
	if(temp == ROUTE) {
		words = origin + " " + words;
		//cout << "\n\t\t---->" << words << endl;
	}
}
void Route::setRoute(vector<string> line) {
	if(routes.count(line.at(2))) {
		north = routes[line.at(2)];
		north->connectNorth(this);
		//cout << "Connected North" << endl;
	}
	if(routes.count(line.at(3))) {
		east = routes[line.at(3)];
		east->connectEast(this);
		//cout << "Connected North" << endl;
	}
	if(routes.count(line.at(4))) {
		west = routes[line.at(4)];
		west->connectWest(this);
		//cout << "Connected North" << endl;
	}
	if(routes.count(line.at(5))) {
		south = routes[line.at(5)];
		south->connectSouth(this);
		//cout << "Connected North" << endl;
	}
	notes = line.at(6);
}


void Trainer::printLocation() {
	cout << "In ";
	currRoute->print();
}
void Trainer::move(string dir) {
	cout << "Moved from " << currRoute->getName() << endl;
	currRoute = currRoute->move(dir);
	cout << "before Encounter\n";
	Trainer temp = currRoute->getEncounter();
	temp.printAll();
	trainerBattle(*this, temp);
	cout << "To " << currRoute->getName() << endl;
}
void Trainer::visitCenter() {
	lastCenter = currRoute;
	reset();
}
bool Trainer::inCity() {return currRoute->isCity();}

class Region {
};
