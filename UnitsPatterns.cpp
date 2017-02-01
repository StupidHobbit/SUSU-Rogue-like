#include <iostream>
#include "UnitsPatterns.h"

UnitPattern getPattern(std::string name){
	static bool is_init = false;
	static std::unordered_map<std::string, UnitPattern> patterns;
	
	if (!is_init){
		std::ifstream fin("data/Units.txt");
		std::string temp, curUnit, tempChar;
		UnitPattern pattern;
		while(fin >> temp){
			//std::cout << temp;
			if (temp[temp.size() - 1] == ':'){
				temp.pop_back();
				curUnit = temp;
				//std::cout << curUnit;
				patterns[temp] = pattern;
			}
			else{	
				fin >> tempChar;
				//std::cout << temp[temp.size() - 1] << std::endl;
				if (temp == "hp")
					fin >> patterns[curUnit].hp;
				else if (temp == "lvl")
					fin >> patterns[curUnit].lvl;
				else if (temp == "dmg")
					fin >> patterns[curUnit].dmg;
				else if (temp == "spriteNum")
					fin >> patterns[curUnit].spriteNum;
			}
		}
		is_init = true;
	}
	
	return patterns[name];
}
