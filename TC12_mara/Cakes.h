//Cakes.h

#include<vector>

using namespace std;

class Cakes {

public:
	vector<int> split(int C, int G, int I, int S, vector<int> preferences, vector<int> cakes)
	{
		vector<int> ret;
		int css = C * S * S;
		for( int i = 0; i < css; i++) {
			ret.push_back(i);
		}//for

		return ret;
	}//split


};//Cakes
