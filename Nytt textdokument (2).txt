//
#include<vector>
#include<string>
#include<iostream>

#include "Cakes.h"

using namespace std;

int main( int argc, char* argv[])
{
	int C, G, I, S;
	cin>>C; //cakes
	cin>>G; //guests
	cin>>I; //ingredients
	cin>>S; // size, one side

	int gi = G * I;
	vector<int> preferences;
	preferences.reserve(gi);
	int temp;
	for( int i = 0; i < gi; i++) {
		cin>>temp;
		preferences.push_back(temp);
	}//for

	vector<int> cakes;
	int cssi = C * S * S * I;
	cakes.reserve(cssi);
	for( int i = 0; i < cssi; i++) {
		cin>>temp;
		cakes.push_back(temp);
	}//for

	Cakes kaka;
	vector<int> ret = kaka.split( C, G, I, S, preferences, cakes);
//	cout << "!" << endl;
	
	for(int i = 0; i < ret.size(); i++) {
		cout << ret[i] << endl;	
	}//for

	cout.flush();
	return 0;
};//main

	//vector<string> contourMap;
	//int H;
	//cin>>H;

	//for(int i = 0; i < H; i++) {
	//	string t;
	//	cin>>t;
	//	contourMap.push_back(t);
	//}//for

	//ReliefMap rm;
	//vector<double> ret = rm.getMap(contourMap);
	//cout << "!" << endl;

	//for(int i = 0; i < ret.size(); i++)
	//{
	//	cout << ret[i] << endl;
	//}//for

	//cout.flush();

	//return 0;