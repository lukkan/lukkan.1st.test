//Cakes.h

#include<vector>

using namespace std;



class Cakes {

	struct kakEater {
		vector<int> prefs;
		int id;
		int which_cake;
		int joyPoints;
};//kakEater

	struct kakBit {
		vector<int> ingredient;
		int eaten;
		int row;
		int col;
};//kakbit

	struct helKaka {
		vector<kakBit> kB;
};//helKaka


public:
	vector<kakEater> kakMonster;
	vector<helKaka> allaKakor;

	void init (int C, int G, int I, int S, vector<int> &preferences, vector<int> &cakes)
	{
		kakEater kakTemp;
		kakTemp.which_cake = -1;
		kakTemp.joyPoints = 0;
		for(int i = 0; i < G; i++) {
			kakTemp.id = i;
			for( int j = 0; j < I; j++ ) {
				kakTemp.prefs.push_back(preferences[ i * I + j ]);
			}//for j
			kakMonster.push_back(kakTemp);
			kakTemp.prefs.clear();
		}//for i

		helKaka helTemp;
		kakBit bitTemp;
//		vector<int> ingTemp;

		for( int c = 0; c < C; c++) {

			for( int x = 0; x < S; x++ ) {
				bitTemp.row = x;

				for( int y = 0; y < S; ++y ) {
					bitTemp.col = y;

					for( int i = 0; i < I; i++ ) {
						bitTemp.ingredient.push_back( cakes[ c*S*S + x * I * S + y * I +i ] );
					}//for i
					bitTemp.eaten = -1;
					helTemp.kB.push_back(bitTemp);
					bitTemp.ingredient.clear();

				}//for y

			}//for x
			allaKakor.push_back(helTemp);
			helTemp.kB.clear();

		}//for c

	};//init

	void whichCake (int C, int G, int I, int S, vector<int> &preferences, vector<int> &cakes)
	{
		int guests_per_cake = ( G + C - 1) / C;
		for( int i = 0; i < G; i++) {
			kakMonster[i].which_cake = i / guests_per_cake;
		}//for

	};//whichCake

	void markBite(vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, int temp_which_cake, int temp_bite )
	{
		allaKakor[temp_which_cake].kB[temp_bite].eaten = true;
	};//markBite


	void addPoints(vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, int k, int temp_which_cake, int temp_bite, int I )
	{
		for(int i = 0; i < I; i++) {
			kakMonster[k].joyPoints += kakMonster[k].prefs[i] * allaKakor[temp_which_cake].kB[temp_bite].ingredient[i];

		}//for

	};//addPoints

	void subPoints(vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, int k, int temp_which_cake, int temp_bite, int I )
	{
		for(int i = 0; i < I; i++) {
			kakMonster[k].joyPoints -= kakMonster[k].prefs[i] * allaKakor[temp_which_cake].kB[temp_bite].ingredient[i];

		}//for

	};//subPoints

	void startCut (int C, int G, int I, int S, vector<int> &preferences, vector<int> &cakes)
	{
		int gpc = ( G + C - 1) / C; //guest_per_cake
		int temp_which_cake;
		int temp_bite;

		for( int i = 0; i < G; i++) {
			temp_which_cake = kakMonster[i].which_cake;
			temp_bite = ( i - ( temp_which_cake * gpc ) ) / ( gpc - 1) * ( (S * S ) - 1 ) ;//( s * s) - 1

			if(temp_bite > ( S * S - 1) ) {
				temp_bite = S * S - 1;
			}//if
			
			markBite( allaKakor, kakMonster, temp_which_cake, temp_bite ); //bättre att skicka över en kaka och en kakätare, istället för alla?
			addPoints( allaKakor, kakMonster, i, temp_which_cake, temp_bite, I);
		}//for

	};//startCut


	vector<int> split(int C, int G, int I, int S, vector<int> preferences, vector<int> cakes)
	{
		init(C, G, I, S, preferences, cakes);
		whichCake(C, G, I, S, preferences, cakes);
		startCut(C, G, I, S, preferences, cakes);
		
		vector<int> ret;
		int css = C * S * S;
		for( int i = 0; i < css; i++) {
			ret.push_back(i);
		}//for

		return ret;
	}//split


};//Cakes
