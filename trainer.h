#include "pokeClasses.h"


class Trainer {
	private:
		bool human = false;
		int pokeDollars = 0;
		Route* currRoute = routes["Pallet Town"];
		Route* lastCenter = routes["Pallet Town"];
		vector<string> inventory;
		UserPoke* currPoke = &voidPoke;
		string name;
		vector<UserPoke> party;
		vector<UserPoke> box;
	public:
		Trainer() {name = "Nope";}
		Trainer(string newName) {name = newName;}
		Trainer(string newName, UserPoke newPoke, Route* startRoute) {
			name = newName;
			party.push_back(newPoke);
			currRoute = startRoute;
			lastCenter = startRoute;
		}
		Trainer(string newName, vector<UserPoke> newParty) {
			name = newName;
			party = newParty;
		}
		Trainer(string newName, UserPoke newPoke) {
			name = newName;
			party.push_back(newPoke);
		}
		Trainer(string newName, vector<UserPoke> newParty, vector<UserPoke> newBox) {
			name = newName;
			party = newParty;
			box = newBox;
		}
		void reset() {for(UserPoke& poke: party) poke.reset();}
		int getExp() {
			if(name != WILD) return currPoke->getExp() * 1.5;
			return currPoke->getExp();
		}
		void gainExp(int expGain) {
			currPoke->gainExp(expGain);
		}
		bool isHuman() {return human;}
		bool inCity(); //moved to bottom of map.h
		void setHuman() {human = true;}
		UserPoke *getCurrPoke() {
			if(currPoke == &voidPoke) {
				currPoke = getPoke();
			}
			return currPoke;
		}
		void printLocation();//moved to bottom of map.h
		void printAll() {
			cout << name << endl;
			printLocation();
			cout << "Party Pokemon:\n";
			for(UserPoke poke : party)
				poke.printAll();
			cout << "Box Pokemon:\n";
			for(UserPoke poke : box)
				poke.printAll();
		}
		void printPoke() {
			for(int i = 0; i < (int)party.size(); i++) {
				cout << i+1 << " ";
				party.at(i).print();
			}
		}
		void print() {
			cout << name << endl;
			printLocation();
			printPoke();
		}
		void catchPoke(UserPoke newPoke) {
			if(party.size() < 6) 
				party.push_back(newPoke);
			else 
				box.push_back(newPoke);
		}
		UserPoke* getPoke() {
			for(UserPoke &poke:  party) {
				if(poke.getHp() > 0) return &poke;
			}
			//cout << "Something went really wrong" << endl;
			whiteout();
			return &voidPoke;
		}
		void switchPoke() {
			if(getPoke() == &voidPoke) whiteout();
			cout << "Which pokemon would you want to switch to? " << endl;
			printPoke();
			string sel;
			cin >> sel;
			if(!cin) die();
			if(isdigit(sel.at(0))) {
				int selNum = stoi(sel);
				if(selNum > (int)party.size()) {
					cout << "Not a Pokemon in your party\n";
					switchPoke();
					return;
				}
				currPoke = &party.at(selNum-1);
				if(currPoke->getHp() <= 0) {
					cout << "That Pokemon has no will left to fight\n";
					switchPoke();
				}
				return;
			} else {
				for(UserPoke &poke: party) {
					if(poke.getName() == sel) {
						currPoke = &poke;
						if(currPoke->getHp() <= 0) {
							cout << "That Pokemon has no will left to fight\n";
							switchPoke();
						}
						return;
					}
				}
				cout << "Not a Pokemon in your party\n";
				switchPoke();
				return;
			}
			//cout << "Something went really wrong" << endl;
		}
		void move(string dir);//moved to bottom of map.h
		void visitCenter();
		//need to make try catch changes and throw all of the things!!!!!
		void whiteout() {
			if(isHuman()) {
				cout << "You Whitted Out\n";
				reset();
			}
			currRoute = lastCenter;
		}
};
