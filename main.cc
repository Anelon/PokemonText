#include "constants.h"

void showPokedex() {
	cout << endl << "Pokedex" << endl;
	for(auto &pokemon : pokedex)
		cout << pokemon.second->getName() << " " << pokemon.second->getId() << endl;
}
void showMovedex() {
	cout << endl << "Movedex" << endl;
	for(auto &move : movedex)
		cout << move.second->getId() << ":" << move.second->getName() << " " << move.second->getType() << endl;
}
void showTypeTable() {
	cout << endl << "Type Table" << endl;
	for(auto &table : typeTable) 
		cout << table.first << ":" << table.second << endl;
}
void showEff() {
	cout << endl << "Not effective:" << endl;
	for(auto &types : notEff) 
		cout << types.first << ":" << types.second << endl;

	cout << endl << "Half effective:" << endl;
	for(auto &types : halfEff) 
		cout << types.first << ":" << types.second << endl;

	cout << endl << "Super effective:" << endl;
	for(auto &types : superEff) 
		cout << types.first << ":" << types.second << endl;
}
void die(string fail) {
	cout << "Something Went Wrong\n";
	cout << fail << endl;
	// possibly try and save state something?
	exit(1);
}

// would put into class but requires the constants so i guess here is fine
//id,identifier,generation_id,type_id,power,pp,accuracy,priority,target_id,damage_class_id,effect_id,effect_chance,contest_type_id,contest_effect_id,super_contest_effect_id
Move::Move(string line) {
	stringstream ss(line);
	string word;
	getline(ss, word, ',');
	if(word.size() && isdigit(word.at(0)))
		id = stoi(word);
	getline(ss, word, ',');
	uppercasify(word);//make the name look nice
	name = word;
	getline(ss, word, ',');
	//do nothing with the generation id
	getline(ss, word, ',');
	//get the typeId and set the type
	int typeNum;
	if(word.size() && isdigit(word.at(0)))
		typeNum = stoi(word);
	type = typeTable[typeNum];
	getline(ss, word, ',');
	if(word.size() && isdigit(word.at(0)))
		power = stoi(word);
	getline(ss, word, ',');
	if(word.size() && isdigit(word.at(0)))
		pp = stoi(word);
	getline(ss, word, ',');
	if(word.size() && isdigit(word.at(0)))
		accuracy = stoi(word);
	getline(ss, word, ',');
	if(word.size() && isdigit(word.at(0)))
		priority = stoi(word);
}

Pokemon choosePokemon() {
	cout << "Choose a Pokemon: ";
	while(true) {
		string in;
		cin >> in;
		if(isdigit(in.at(0))) {
			int inNum = stoi(in);
			if(pokedex.count(inNum) > 0)  {
				cout << "You chose " << pokedex[inNum]->getName() << endl;
				return *pokedex[inNum];
			}
		} else {
			if(pokedexNames.count(in) > 0)  {
				cout << "You chose " << pokedexNames[in]->getName() << endl;
				return *pokedexNames[in];
			} else if(in == "print") {
				showPokedex();
			}
		}
		cout << "Not found try again, (to print pokemon again enter 'print')\n";
	}
}

vector<Move> chooseMove() {
	vector<Move> selected;
	cout << "Choose a Move: ";
	bool failed = true;
	while(selected.size() < MAX_MOVES) {
		if(!failed) cout << "Choose another Move: ";
		string in;
		cin >> in;
		if(isdigit(in.at(0))) {
			int inNum = stoi(in);
			if(movedex.count(inNum) > 0)  {
				cout << "You chose " << movedex[inNum]->getName() << endl;
				selected.push_back(*movedex[inNum]);
				failed = false;
				continue;
			}
		} else {
			if(movedexNames.count(in) > 0)  {
				cout << "You chose " << movedexNames[in]->getName() << endl;
				selected.push_back(*movedexNames[in]);
				failed = false;
				continue;
			} else if(in == "print") {
				showMovedex();
				failed = false;
				continue;
			}
		}
		cout << "Not found try again, (to print moves again enter 'print')\n";
		failed = true;
	}
	return selected;
}

//TODO make look better later
//legacy not needed anymore
void battle1v1(UserPoke &poke1, UserPoke &poke2) {
	UserPoke yourPoke = poke1;
	UserPoke *firstPoke = &poke1;
	UserPoke *nextPoke = &poke2;
	if(poke1.getSpeed() < poke2.getSpeed()) {
		swap(firstPoke, nextPoke);
	}
	int action = 0;
	int moveSel = 0;
	while(poke1.getHp() > 0 && poke2.getHp() > 0) {
		nextPoke->print();
		cout << "\n\n\n\n";
		//check if it is the users pokemon
		if(!action) { //select what to do
			firstPoke->print();
			cout << "What would you like to do?\n";
			cout << "	" << FIGHT << ") Fight 	"<< POKEMON << ") Pokemon\n";
			cout << "	" << BAG << ") Bag 		" << RUN << ") Run\n";
			cin >> action;
			system("clear");
		} else { //action selected
			if(action == FIGHT) { //select move
				firstPoke->printMoves();//add show moves
				cout << "Choose move for " << firstPoke->getName() << " to use: ";
				cin >> moveSel;
				if(!cin) die();
				if(moveSel < 1 || moveSel > MAX_MOVES) {
					cout << "Not a valid move try again\n";
					continue;
				}
				//need to move you, or wait until both moves are selected?
				firstPoke->damage(nextPoke, moveSel);
			} else if(action == POKEMON) {
				cout << "Can not switch pokemon at this moment\n";
				action = 0;
				continue;
			} else if(action == BAG) {
				cout << "You look for your bag but can't seem to find it\n";
				action = 0;
				continue;
			} else if(action == RUN) {
				cout << "You don't see anywhere to run\n";
				action = 0;
				continue;
				//posibly set leave battle flag somewhere
			} else {
				cout << "Bad action try again!" << endl;
				action = 0;
				continue;
			}
			action = 0;
			swap(firstPoke,nextPoke);
		}
	}
}

void trainerBattle(Trainer &you, Trainer opp) {
	system("clear");
	if (you.getPoke() == &voidPoke || opp.getPoke() == &voidPoke)
		die("Stated a Battle With no Pokemon");
	//loop until one trainer is out of pokemon
	while(true) {
		if (you.getPoke() == &voidPoke || opp.getPoke() == &voidPoke)
			break;
		//UserPoke yourPoke = poke1;
		Trainer *first = &you;
		Trainer *next = &opp;
		if(you.getCurrPoke()->getSpeed() < opp.getCurrPoke()->getSpeed()) {
			swap(first, next);
		}
		int action = 0;
		int moveSel = 0;
		//int moves = 0;
		while(true) {
			UserPoke* firstPoke = first->getCurrPoke();
			UserPoke* nextPoke = next->getCurrPoke();
			nextPoke->print();
			cout << "\n\n\n\n";
			//check if it is the users pokemon
			if(!first->isHuman()) {
				//add actual ai or something?
				action = 1;
				moveSel = 1;
			}
			if(!action) { //select what to do
				firstPoke->print();
				cout << "What would you like to do?\n";
				cout << "	" << FIGHT << ") Fight 	"<< POKEMON << ") Pokemon\n";
				cout << "	" << BAG << ") Bag 		" << RUN << ") Run\n";
				cin >> action;
				system("clear");
			} else { //action selected
				if(action == FIGHT) { //select move
					if(!moveSel) {
						firstPoke->printMoves();//add show moves
						cout << "Choose move for "<< firstPoke->getName() << " to use: ";
						cin >> moveSel;
						if(!cin) die();
						if(moveSel < 1 || moveSel > MAX_MOVES) {
							cout << "Not a valid move try again\n";
							continue;
						}
					}
					//need to move you, or wait until both moves are selected?
					firstPoke->damage(nextPoke, moveSel-1);
					moveSel = 0;
				} else if(action == POKEMON) {
					first->switchPoke();
					//cout << "Can not switch pokemon at this moment\n";
					//action = 0;
					//continue;
				} else if(action == BAG) {
					cout << "You look for your bag but can't seem to find it\n";
					action = 0;
					continue;
				} else if(action == RUN) {
					cout << "You don't see anywhere to run\n";
					action = 0;
					continue;
					//posibly set leave battle flag somewhere
				} else {
					cout << "Bad action try again!" << endl;
					action = 0;
					continue;
				}
				action = 0;
				swap(first,next);
			}
			//check if pokemon dead?
			if(you.getCurrPoke()->getHp() <= 0 || opp.getCurrPoke()->getHp() <= 0){
				if(opp.getCurrPoke()->getHp() <= 0) {
					int expGain = opp.getExp();
					you.gainExp(expGain);
					cout << you.getCurrPoke()->getName() << " gained " << expGain << " exp.\n";
				}
				break;
			}
		}
		//battle1v1(you.getPoke(), opp.getPoke());
	}
	cout << "You: " << you.getPoke()->getName() << endl;
	if (you.getPoke() == &voidPoke) {
		cout << "You Lose!\n";
	}
	cout << "Opp: " << opp.getPoke()->getName() << endl;
	if (opp.getPoke() == &voidPoke) {
		cout << "You Win!\n";
	}
}

int main() {
	srand(time(NULL));
	loadConsts();
	//testing delete later
	/*
	cout << typeTable[11] << endl;
	cout << "Kanto: " << endl;
	routes["Kanto Route 9"]->printAll();
	cout << endl;
	routes["Pallet Town"]->print();
	routes["Kanto Route 1"]->print();
	*/
	const int NUMSTARTERS = 3;
	vector<vector<Pokemon*>> starters = {
		{pokedexNames["Bulbasaur"],pokedexNames["Charmander"],pokedexNames["Squirtle"]},
		{pokedexNames["Chikorita"],pokedexNames["Cyndaquil"],pokedexNames["Totodile"]},
		{pokedexNames["Treecko"],pokedexNames["Torchic"],pokedexNames["Mudkip"]},
		{pokedexNames["Turtwig"],pokedexNames["Chimchar"],pokedexNames["Piplup"]},
		{pokedexNames["Snivy"],pokedexNames["Tepig"],pokedexNames["Oshawott"]},
		{pokedexNames["Chespin"],pokedexNames["Fennekin"],pokedexNames["Froakie"]},
		{pokedexNames["Rowlet"],pokedexNames["Litten"],pokedexNames["Popplio"]},
		{pokedexNames["Eevee"],pokedexNames["Pikachu"]},
	};

	cout << "\nWelcome to Pokemon, and your name is? ";
	string username;
	getline(cin, username);

	cout << "Pick a Starter:" << endl;
	//-1 to account for pikachu and eevee being a pair rather than 3
	for(int i = 0; i < (int)starters.size()*NUMSTARTERS-1; i++) {
		cout << "\t" << i+1 << ": ";
		starters.at(i/NUMSTARTERS).at(i%NUMSTARTERS)->print();
	}
	Pokemon tempPoke;
	Pokemon rivalPoke;
	while(true) {
		string in;
		cin >> in;
		if(isdigit(in.at(0))) {
			int inNum = stoi(in) -1;
			if(inNum < (int)starters.size()*NUMSTARTERS-1)  {
				tempPoke = *starters.at(inNum/NUMSTARTERS).at(inNum%NUMSTARTERS);
				cout << "You chose " << tempPoke.getName() << endl;
				//oh gosh math!!!
				rivalPoke = *starters.at(inNum/NUMSTARTERS).at((inNum+1)%starters.at(inNum/NUMSTARTERS).size());
				break;
			}
		} else {
			//shhhh easteregg you can pick any poke to start
			if(pokedexNames.count(in) > 0)  {
				//its not a bug its me being lazy
				cout << "You chose " << pokedexNames[in]->getName() << endl;
				tempPoke = *pokedexNames[in];
			}
		}
	}
	string nick = giveNickName();
	UserPoke myPoke(tempPoke, STARTERLVL, nick);
	UserPoke riPoke(rivalPoke, STARTERLVL);

	//vector<Move> tempMoves = chooseMove();
	//UserPoke myPoke(tempPoke, tempMoves);
	myPoke.printAll();
	Trainer player(username, myPoke, routes["Pallet Town"]);
	player.setHuman();
	Trainer rival("Rival", riPoke, routes["Pallet Town"]);
	//cout << endl;
	//rival.printAll();
	string in;
	cout << "System: Are you ready to begin your journey as a Pokemon Trainer?" << endl;
	cin >> in;
	if(!cin) die();
	cout << "Person: Hey I just saw that you started, would you like to battle? (Y/N)\n";
	cin >> in;
	if(!cin) die();
	if(toupper(in.at(0)) == 'Y') {
		trainerBattle(player, rival);
	}
	next();

	while(true) {
		system("clear");
		player.printLocation();
		cout << "What would you like to do?\n";
		cout << "\t('Move') and a direction to change locations.\n";
		if(player.inCity()) {
			cout << "\t('Center') Visit pokemon center to heal your pokemon.\n";
			cout << "\t('Mart') To see what the pokemart in city has to sell.\n"; 
		}
		cin >> in;
		if(!cin || in == "q") die();
		uppercasify(in);
		if(in == "Move") {
			cin >> in;
			if(!cin || in == "q") die();
			uppercasify(in);
			player.move(in);
		} else if(in == "Center") {
			player.reset();
		} else if(in == "Mart") {
			cout << "You walk to the Pokemart and see that it is closed temporarily\n";
		} else if(in == "Die") {
			player.whiteout();
			continue;
		} else continue;
		next();
	}

	//get second pokemon
	/*
	string name = "Wild";
	tempPoke = choosePokemon();
	tempMoves = chooseMove();
	UserPoke theirPoke(tempPoke, tempMoves);
	theirPoke.print();
	vector<UserPoke> vecPoke;
	Trainer wild(name, vecPoke);

	trainerBattle(player, wild);
	*/
}
