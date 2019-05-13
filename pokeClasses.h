#include "headers.h"

//vector locations for stats
const int HP = 1;
const int ATTACK = 2;
const int DEFENSE = 3;
const int SPEC_ATTACK = 4;
const int SPEC_DEFENSE = 5;
const int SPEED = 6;
const int ACCURACY = 7;
const int EVASION = 8;
const int LANGUAGE = 9; //for selecting english is the stats csv

class Move {
	private:
		string name, type;
		int id, power, pp, accuracy;
		bool priority; //for things like quick attack
	public:
		bool operator==(Move rhs) {return this->getName() == rhs.getName();}
		Move(string line);
		int getId() {return id;}
		string getName() {return name;}
		string getType() {return type;}
		int getPower() {return power;}
		int getPP() {return pp;}
		int getAccuracy() {return accuracy;}
		void printAll() {
			cout << name << " " << type << endl;
			cout << " PP: " << pp << " Power: " << power << " Accuracy " << accuracy;
			cout << endl;
		}
		void print() {
			cout << name << " " << type << " PP: " << pp << endl;
		}
		void damage(UserPoke *att, UserPoke *def);
};

class Pokemon {
	private:
		multimap<int,Move*> canLearn; //int level, the move
		string name, type1, type2;
		int id = 0, hp = 0, attack = 0, defense = 0, specAttack = 0, specDef = 0, speed = 0, accuracy = 0, evasion = 0, baseExp = 0;
		vector<int> evs;
	public:
		bool operator==(Pokemon rhs) {
			return (this->getName() == rhs.getName()) && (this->getType1() == rhs.getType1()) && (this->getType2() == rhs.getType2());
		}
		Pokemon(){name = "MissingNo";}
		Pokemon(string line);
		int getId() {return id;}
		void setStat(int statId, int value);
		int getHp() {return hp;}
		int getAttack() {return attack;}
		int getDefense() {return defense;}
		int getSpecAttack() {return specAttack;}
		int getSpecDef() {return specDef;}
		int getSpeed() {return speed;}
		int getAccuracy() {return accuracy;}
		int getEvasion() {return evasion;}
		string getName() {return name;}
		int getBaseExp() {return baseExp;}
		vector<Move> getMoves(int lvl) {
			vector<Move> retMoves;
			int moveNum = 4;
			if(lvl > STARTERLVL) {
				moveNum = rand() % (MAX_MOVES/2) + 2;
			} else moveNum = 2;
			for(int i = lvl; i > 0 && moveNum; i--) {
				auto iterpair = canLearn.equal_range(i);
				auto it = iterpair.first;
				for (; it != iterpair.second; ++it) {
					retMoves.push_back(*it->second);
					moveNum--;
				}
			}
			return retMoves;
		}
		void takeDamage(int damage) {
			hp -= damage;
			//if(hp < 0) hp = 0;
		}
		void setType(string newType) {
			if(!type1.size()) type1 = newType;
			else type2 = newType;
		}
		void addMove(int lvl, Move* move);
		void addExp(int newExp, vector<int> newEvs) {
			baseExp = newExp;
			evs = newEvs;
		}
		string getType1() {return type1;}
		string getType2() {return type2;}
		void print() {
			cout << name << " " << type1 << " " << type2 << endl;
		}
};

Pokemon::Pokemon(string line) {
	stringstream ss(line);
	string word;
	getline(ss, word, ',');
	if(word.size() && isdigit(word.at(0)))
		id = stoi(word);
	getline(ss, word, ',');
	uppercasify(word);
	name = word;
}

void Pokemon::setStat(int statId, int value) {
	if(statId == HP) hp = value;
	else if(statId == ATTACK) attack = value;
	else if(statId == DEFENSE) defense = value;
	else if(statId == SPEC_ATTACK) specAttack = value;
	else if(statId == SPEC_DEFENSE) specDef = value;
	else if(statId == SPEED) speed = value;
	else if(statId == ACCURACY) accuracy = value;
	else if(statId == EVASION) evasion = value;
}

void Pokemon::addMove(int lvl, Move* move) {
	for(auto &mv : canLearn) if(mv.second == move) {
		if(lvl <= mv.first) return; //make the hardest to get moves
		//find and delete lower lvl duplicate then insert new value
		auto iterpair = canLearn.equal_range(mv.first);
		auto it = iterpair.first;
		for (; it != iterpair.second; ++it) {
			if (it->second == move) {
				//cout << lvl << " Erased " << mv.second->getName() << endl;
				canLearn.erase(it);
				canLearn.insert(make_pair(lvl,move));
				return;
			}
		}
		return;
	}
	//cout << name << " can learn " << move->getName() << " at " << lvl << endl;
	canLearn.insert(make_pair(lvl,move));
}

class UserPoke {
	private:
		Pokemon poke;
		Pokemon defaultStatPoke;
		vector<Move> moves;
		//maybe change later but use lvl equation
		//exp = level^3
		int level = 0, exp = 0;
		string nickName = "";
	public:
		UserPoke(Pokemon newPoke, vector<Move> newMoves) {
			nickName = giveNickName();
			poke = newPoke;
			defaultStatPoke = newPoke;
			moves = newMoves;
		}
		UserPoke(Pokemon newPoke) {
			nickName = giveNickName();
			defaultStatPoke = newPoke;
			poke = newPoke;
		}
		UserPoke(Pokemon newPoke, int lvl, string newNick = "") {
			nickName = newNick;
			level = lvl;
			defaultStatPoke = newPoke;
			poke = newPoke;
			moves = poke.getMoves(lvl);
		}
		UserPoke() {poke = Pokemon();}
		bool operator==(UserPoke rhs) {
			return (this->getPokemon() == rhs.getPokemon()) && (this->getHp() == rhs.getHp()) && (this->getLevel() == rhs.getLevel());
		}
		void setName(string newName) {nickName = newName;}
		string getName() {
			if(nickName.size()) return nickName;
			return poke.getName();
		}
		Pokemon getPokemon() {return poke;}
		int getExp() {
			if(getHp() > 0) return 0;//sanity check
			//exp = baseExp * level/7 (lazy edition of exp)
			return (poke.getBaseExp() * level)/7;
		}
		void gainExp(int expGain) {
			exp += expGain;
			if(exp > pow(level+1,3)) levelUp();
		}
		string getType1() {return poke.getType1();}
		string getType2() {return poke.getType2();}
		//gonna have to change getLevel with get statboost or something
		int getLevel() {return level;}
		int getHp() {return poke.getHp()+getLevel();}
		int getAttack() {return poke.getAttack()+getLevel();}
		int getDefense() {return poke.getDefense()+getLevel();}
		int getSpecAttack() {return poke.getSpecAttack()+getLevel();}
		int getSpecDef() {return poke.getSpecDef()+getLevel();}
		int getSpeed() {return poke.getSpeed()+getLevel();}
		int getAccuracy() {return poke.getAccuracy()+getLevel();}
		int getEvasion() {return poke.getEvasion();}
		int getMovePower(int moveId) {return moves.at(moveId-1).getPower();}
		int getMoveAccuracy(int moveId) {return moves.at(moveId-1).getAccuracy();}
		int getMovePP(int moveId) {return moves.at(moveId-1).getPP();}
		void printHp() {cout << " HP: " << getHp();}
		void printLevel() {cout << " Level: " << getLevel();}
		void printAttack() {cout << " Attack: " << getAttack();}
		void printDefense() {cout << " Defense: "<< getDefense();}
		void printSpecAttack() {cout << " SpecAttack: "<< getSpecAttack();}
		void printSpecDef() {cout << " SpecDefense: "<< getSpecDef();}
		void printSpeed() {cout << " Speed: " << getSpeed();}
		void printAccuracy() {cout << " Accuracy: "<< getAccuracy();}
		void printEvasion() {cout << " Evasion: "<< getEvasion();}
		void printMovePower(int moveId) {cout << " Power: "<< moves.at(moveId-1).getPower();}
		void printMoveAccuracy(int moveId) {cout << " Accuracy: "<< moves.at(moveId-1).getAccuracy();}
		void printMovePP(int moveId) {cout << " PP: "<<  moves.at(moveId-1).getPP();}
		string getMoveType(int moveId) {return moves.at(moveId-1).getType();}
		string getMoveName(int moveId) {return moves.at(moveId-1).getName();}
		void takeDamage(int damage) {poke.takeDamage(damage);}
		void reset() {poke = defaultStatPoke;}
		void levelUp() {
			if(level < 100) level++;
		}
		void print() {
			cout << getName() << " " << getType1() << " " << getType2() << endl;
			printHp();
			cout << endl;
		}
		void printMoves() {
			print();
			for(int i = 0; i < (int)moves.size(); i++) {
				cout << i + 1 << ": ";//dont start at 0
				moves.at(i).print();
			}
		}
		void printAll() {
			print();

			printLevel();
			printAttack();
			printDefense();
			printSpecAttack();
			printSpecDef();
			printSpeed();
			cout << endl;
			for(int i = 0; i < (int)moves.size(); i++) {
				cout << i+1 << ": ";
				moves.at(i).printAll();
			}
		}
		void damage(UserPoke *def, int use) {
			cout << getName() << " used " << moves.at(use).getName() << endl;
			moves.at(use).damage(this, def);
		}
};
//hidden and lost forever
UserPoke voidPoke = UserPoke();

void Move::damage(UserPoke *att, UserPoke *def) {
	if(!power) return; //if the move doesnt have any power dont do anything
	/*
	   A = attacker's Level
	   B = attacker's Attack or Special
	   C = attack Power
	   D = defender's Defense or Special
	   X = same-Type attack bonus (1 or 1.5)
	   Y = Type modifiers (4, 2, 1, .5, .25, or 0)
	   Z = a random number between 217 and 255
	   */
	//set up variables
	double A = att->getLevel();
	if(A < 10) A = 10;
	double B = att->getAttack();
	double C = power;
	double D = def->getDefense();
	double X = 1;//calcuate stab
	if(att->getType1() == type) X = 1.5;
	else if(att->getType2() == type) X = 1.5;
	double Y = 1;//calculate type advantage
	//hmmm might refactor to 2d array not sure if this 
	//gets itterator to begining and end or type effective range
	auto range = notEff.equal_range(type);
	for(auto it = range.first; it != range.second; it++)
		if(it->second == def->getType1() || it->second == def->getType2())
			Y *= 0;
	if(Y == 0) 
		cout << type << " doesn't Damage " << def->getName();

	range = superEff.equal_range(type);
	for(auto it = range.first; it != range.second; it++)
		if(it->second == def->getType1() || it->second == def->getType2())
			Y *= 2;

	range = halfEff.equal_range(type);
	for(auto it = range.first; it != range.second; it++)
		if(it->second == def->getType1() || it->second == def->getType2())
			Y *= .5;

	double Z = 255;//random number between 217 and 255
	//do some math
	//might adjust actual damage numbers later
	int damage = ((((((((2.0*A/5+2.0)*B*C)/D/5))+2.0)*X)*Y)*Z)/255.0;
	cout << def->getName() << " Took " << damage << " damage.\n";
	if(Y > 1) cout << " it was Super Effective.";
	else if(Y < 1) cout << " it was Not Very Effective.";
	cout << endl;
	def->takeDamage(damage);
}
