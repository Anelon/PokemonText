#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

int main() {
	map<int,string> test;
	test.insert({1,"test"});
	if(test[2]) cout << "not in map\n";
	else cout << "Something wrong\n";
}
