#include "map.h"

void loadConsts() {
	int lines =0;
	cout << "Starting Loading Your Game\n";
	//load type id and name
	ifstream typeText;
	typeText.open("data/types.csv");
	if(typeText) {
		string line;
		//throw away first line
		getline(typeText, line);
		cout << endl << "Load Type ID and Name" << endl;
		cout << line << endl;
		while(typeText) {
			lines++; //see how many lines of data were used
			getline(typeText, line);
			stringstream ss(line);
			string temp;
			int id;
			getline(ss, temp, ',');
			if(temp.size() && isdigit(temp.at(0))) {
				id = stoi(temp);
			}
			getline(ss, temp, ',');
			uppercasify(temp);//make the name look nicer
			if(typeText) typeTable.insert(make_pair(id,temp));
		}
	}
	typeText.close();
	//load pokemon name and ID
	ifstream pokeText;
	pokeText.open("data/pokemon.csv");
	if(pokeText) {
		string line;
		//throw away first line
		getline(pokeText, line);
		cout << endl <<  "Load Pokemon ID and Name" << endl;
		cout << line << endl;
		while(pokeText) {
			lines++; //see how many lines of data were used
			getline(pokeText, line);
			Pokemon *temp = new Pokemon(line);
			if(pokeText) {
				//map the name and id to the pokemon pointer
				pokedex.insert(make_pair(temp->getId(),temp));
				pokedexNames.insert(make_pair(temp->getName(),temp));
			}
		}
	}
	pokeText.close();
	//get pokemon types and link to pokemon
	ifstream pokeTypeText;
	pokeTypeText.open("data/pokemon_types.csv");
	if(pokeTypeText) {
		string line;
		//throw away first line
		getline(pokeTypeText, line);
		cout << endl << "Load Pokemon Types" << endl;
		cout << line << endl;
		while(pokeTypeText) {
			lines++; //see how many lines of data were used
			getline(pokeTypeText, line);
			stringstream ss(line);
			string temp;
			getline(ss, temp, ',');
			int pokeID = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				pokeID = stoi(temp);
			getline(ss, temp, ',');
			int typeID = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				typeID = stoi(temp);
			if(pokeTypeText) pokedex[pokeID]->setType(typeTable[typeID]);
		}
	}
	pokeTypeText.close();
	//get pokemon types and link to pokemon
	ifstream pokeStatText;
	pokeStatText.open("data/pokemon_stats.csv");
	if(pokeStatText) {
		string line;
		//throw away first line
		getline(pokeStatText, line);
		cout << endl << "Load Pokemon Stats" << endl;
		cout << line << endl;
		while(pokeStatText) {
			lines++; //see how many lines of data were used
			getline(pokeStatText, line);
			stringstream ss(line);
			string temp;
			getline(ss, temp, ',');
			int pokeID = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				pokeID = stoi(temp);
			getline(ss, temp, ',');
			int statID = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				statID = stoi(temp);
			getline(ss, temp, ',');
			int stat = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				stat = stoi(temp);
			//cout << stat << endl;
			if(pokeStatText) pokedex[pokeID]->setStat(statID,stat);
		}
	}
	pokeStatText.close();
	//load moves
	ifstream moveText;
	moveText.open("data/moves.csv");
	if(moveText) {
		string line;
		//throw away first line
		getline(moveText, line);
		cout << endl << "Load all of the moves" << endl;
		cout << line << endl;
		while(moveText) {
			lines++; //see how many lines of data were used
			getline(moveText, line);
			Move *temp = new Move(line);
			if(moveText) {
				movedex.insert(make_pair(temp->getId(),temp));
				movedexNames.insert(make_pair(temp->getName(),temp));
			}
		}
	}
	moveText.close();
	//load map between regions and their id
	ifstream regionMapText;
	regionMapText.open("data/regions.csv");
	if(regionMapText) {
		string line;
		//throw away first line
		getline(regionMapText, line);
		cout << endl << "Load Region Id Map" << endl;
		cout << line << endl;
		while(regionMapText) {
			lines++; //see how many lines of data were used
			getline(regionMapText, line);
			stringstream ss(line);
			string temp;
			getline(ss, temp, ',');
			int id = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				id = stoi(temp);
			else break;
			ss >> temp;
			uppercasify(temp);
			//cout << temp << endl;
			if(regionMapText) {
				regionIdMap.insert(make_pair(id,temp));
			}
		}
	}
	regionMapText.close();
	//load location id map and locations
	ifstream locationsText;
	locationsText.open("data/locations.csv");
	if(locationsText) {
		string line;
		//throw away first line
		getline(locationsText, line);
		cout << endl << "Load locations" << endl;
		cout << line << endl;
		while(locationsText) {
			lines++; //see how many lines of data were used
			getline(locationsText, line);
			stringstream ss(line);
			string temp;
			getline(ss, temp, ',');
			int id = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				id = stoi(temp);
			else continue;
			getline(ss, temp, ',');
			int regionid = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				regionid = stoi(temp);
			else continue;
			ss >> temp;
			uppercasify(temp);
			Route* tempRoute = new Route(temp);
			//cout << temp << endl;
			if(locationsText) {
				routeIdMap.insert(make_pair(id,temp));
				routeToRegion.insert(make_pair(regionIdMap[regionid],temp));
				routes.insert(make_pair(tempRoute->getName(),tempRoute));
			}
		}
	}
	locationsText.close();
	//load routes
	ifstream routeText;
	routeText.open("routes.csv");
	if(routeText) {
		string line;
		//throw away first line
		getline(routeText, line);
		cout << endl << "Load all of the Routes" << endl;
		cout << line << endl;
		//getline(routeText, line);
		while(routeText) {
			lines++; //see how many lines of data were used
			//hmm read 6 things every time
			vector<string> reading;
			for(int i = 0; i < 6; i++) {
				string read;
				getline(routeText,read,',');
				if(i > 0) fixRoute(read);
				//cout << read << endl << endl;
				uppercasify(read);
				reading.push_back(read);
			}
			string read;
			getline(routeText, read);
			if(reading.at(1).size() == 0) {
				origin = reading.at(0);
				//cerr << "-----------" << origin << "----------" << endl;
				continue;
			}
			//cout << read << endl;
			reading.push_back(read);
			if(!routeText) break;
			if(routes.count(reading.at(0)) > 0) {
				//cout << routes[reading.at(0)]->getName() << endl;
				routes[reading.at(0)]->setRoute(reading);
			} else {
				//cerr << "\t\tMISSING: " << reading.at(0) << endl;
				Route* temp = new Route(reading);
				//cerr << "\t\tAdded: " << temp->getName() << endl;
				routes.insert(make_pair(temp->getName(),temp));
			}
		}
	}
	routeText.close();
	//load location areas map
	map<int,int> areaIdMap;
	ifstream locationAreasText("data/location_areas.csv");
	if(locationAreasText) {
		string line;
		//throw away first line
		getline(locationAreasText, line);
		cout << endl << "Load location area maps" << endl;
		cout << line << endl;
		while(locationAreasText) {
			lines++; //see how many lines of data were used
			getline(locationAreasText, line);
			stringstream ss(line);
			string temp;
			getline(ss, temp, ',');
			int id = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				id = stoi(temp);
			else continue;
			getline(ss, temp, ',');
			int areaId = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				areaId = stoi(temp);
			else continue;
			//cout << id << ":" << areaId << endl;
			if(locationAreasText) {
				areaIdMap.insert(make_pair(id,areaId));
			}
		}
	}
	//load encounters ("encouters.csv")
	ifstream encouterText;
	encouterText.open("data/encounters.csv");
	if(encouterText) {
		string line;
		//throw away first line
		getline(encouterText, line);
		cout << endl << "Load all Encouters" << endl;
		cout << line << endl;
		while(encouterText) {
			lines++; //see how many lines of data were used
			getline(encouterText, line);
			stringstream ss(line);
			//cout << line << endl;
			string temp;
			//throw away first two
			getline(ss, temp, ',');
			getline(ss, temp, ',');
			//get location id
			getline(ss, temp, ',');
			int area = 0;
			//aproved area is working!!!!!
			if(temp.size() && isdigit(temp.at(0))) 
				area = areaIdMap[stoi(temp)];//location area id to actual location?
			else continue;
			//possibly use encouter id (probably too much effort)
			getline(ss, temp, ',');
			//get pokemon id
			getline(ss, temp, ',');
			int pokeid = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				pokeid = stoi(temp);
			else continue;
			//cout << " Pokemon: " << pokeid;
			//get min level
			getline(ss, temp, ',');
			int min = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				min = stoi(temp);
			else continue;
			//cout << " min: " << min;
			//get max level
			getline(ss, temp);
			int max = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				max = stoi(temp);
			else continue;
			//cout << " max: " << max;
			//cout << endl;
			if(encouterText && routeIdMap.count(area)) {
				//check me so see if pokeid is right just gastlys...
				//everything fixed and happy
				Encounter enc(pokedex[pokeid],min,max);
				routes[routeIdMap[area]]->addPoke(enc);
			}
		}
	}
	encouterText.close();
	//load pokemon move learn lvls
	ifstream pokemonMoveText("data/pokemon_moves.csv");
	if(pokemonMoveText) {
		string line;
		//throw away first line
		getline(pokemonMoveText, line);
		cout << endl << "Load Pokemon Moves per lvl" << endl;
		cout << line << endl;
		while(pokemonMoveText) {
			lines++; //see how many lines of data were used
			getline(pokemonMoveText, line);
			stringstream ss(line);
			string temp;
			getline(ss, temp, ',');
			int pokeid = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				pokeid = stoi(temp);
			else continue;
			//toss version group id
			getline(ss, temp, ',');
			//get move id
			getline(ss, temp, ',');
			int moveid = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				moveid = stoi(temp);
			else continue;
			//toss pokemon move method (maybe use later)
			getline(ss, temp, ',');
			//level
			getline(ss, temp, ',');
			int lvl = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				lvl = stoi(temp);
			else continue;
			//cout << id << ":" << areaId << endl;
			if(pokemonMoveText && lvl) {
				pokedex[pokeid]->addMove(lvl,movedex[moveid]);
			}
		}
	}
	//load pokemon move learn lvls
	ifstream pokemonExpText("exp-evs.csv");
	if(pokemonExpText) {
		string line;
		//throw away first line
		getline(pokemonExpText, line);
		cout << endl << "Load Exp and Evs per Pokemon" << endl;
		cout << line << endl;
		while(pokemonExpText) {
			lines++; //see how many lines of data were used
			getline(pokemonExpText, line);
			//cout << line << endl;
			stringstream ss(line);
			string temp;
			getline(ss, temp, ',');
			int pokeid = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				pokeid = stoi(temp);
			else continue;
			//cout << pokeid << " ";
			//toss pokemon name
			getline(ss, temp, ',');
			//get base exp
			getline(ss, temp, ',');
			int exp = 0;
			if(temp.size() && isdigit(temp.at(0))) 
				exp = stoi(temp);
			else continue;
			//cout << exp << " ";
			vector<int> evs;
			while(ss) {
				getline(ss, temp, ',');
				int ev = 0;
				if(temp.size() && isdigit(temp.at(0))) 
					ev = stoi(temp);
				else continue;
				//if(ss) cout << ev << " ";
				if(ss) evs.push_back(ev);
			}
			//cout << endl;
			//cout << id << ":" << areaId << endl;
			if(pokemonExpText && pokedex.count(pokeid)) {// getting rid of mega pokemon for now
				pokedex[pokeid]->addExp(exp, evs);
			}
		}
	}

	//load hometowns?
	cout << endl << "Loaded " << lines << " lines of Data!\n";
	cout << "Finished Loading\n\n";
}
