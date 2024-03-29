//Cakes.h

#include<vector>
#include<algorithm>
#include<queue>

using namespace std;

class Cakes {

	struct kakEater {
		vector<int> prefs;
		int id;
		int which_cake;
		int joyPoints;
		vector<int> cake_bites;
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
						bitTemp.ingredient.push_back( cakes[ c*I*S*S + x * I * S + y * I +i ] );
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
////////////////////////////////////////////////////////
	void whichCake (int C, int G, int I, int S, vector<int> &preferences, vector<int> &cakes)
	{
		int guests_per_cake = ( G + C - 1) / C;
		for( int i = 0; i < G; i++) {
			kakMonster[i].which_cake = i / guests_per_cake;
		}//for

	};//whichCake
////////////////////////////////////////////////////////
	void markBite(vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, int guest, int temp_which_cake, int temp_bite )
	{
		allaKakor[temp_which_cake].kB[temp_bite].eaten = kakMonster[guest].id;
	};//markBite

	int calcJoy( vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, int which_cake, int guest, int temp_bite, int I)
	{
		int temp_joy = 0;
		for(int i = 0; i < I; i++) {
			temp_joy += kakMonster[guest].prefs[i] * allaKakor[which_cake].kB[temp_bite].ingredient[i];
		}//for

		return temp_joy;
	};//calcJoy
////////////////////////////////////////////////////////
	void addPoints(vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, int guest, int temp_which_cake, int temp_bite, int I )
	{
		/*for(int i = 0; i < I; i++) {
			kakMonster[guest].joyPoints += kakMonster[guest].prefs[i] * allaKakor[temp_which_cake].kB[temp_bite].ingredient[i];*/
//		}//for
		kakMonster[guest].joyPoints += calcJoy(allaKakor, kakMonster, temp_which_cake, guest, temp_bite, I);
		kakMonster[guest].cake_bites.push_back(temp_bite);

	};//addPoints
////////////////////////////////////////////////////////
	void subPoints(vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, int guest, int temp_which_cake, int temp_bite, int I )
	{
		//for(int i = 0; i < I; i++) {
		//	kakMonster[k].joyPoints -= kakMonster[k].prefs[i] * allaKakor[temp_which_cake].kB[temp_bite].ingredient[i];

		//}//for
		kakMonster[guest].joyPoints -= calcJoy(allaKakor, kakMonster, temp_which_cake, guest, temp_bite, I);
		kakMonster[guest].cake_bites.erase( remove( kakMonster[guest].cake_bites.begin(), kakMonster[guest].cake_bites.end(), temp_bite), kakMonster[guest].cake_bites.end() );

		//vector<int>::iterator it = find( kakMonster[guest].cake_bites.begin(), kakMonster[guest].cake_bites.end(), temp_bite);
		//if( it != kakMonster[guest].cake_bites.end() )
		//kakMonster[guest].cake_bites.erase(it) ;

	};//subPoints
////////////////////////////////////////////////////////
	void addNsubPoints(vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, int winner, int loser,
 int temp_which_cake, int temp_bite, int I )
	{
		addPoints(allaKakor, kakMonster, winner, temp_which_cake, temp_bite, I);
		subPoints(allaKakor, kakMonster, loser, temp_which_cake, temp_bite, I); 
	};//addNsubPoints
////////////////////////////////////////////////////////
	void startCut (int C, int G, int I, int S, vector<int> &preferences, vector<int> &cakes)
	{
		int gpc = ( G + C - 1) / C; //guest_per_cake
		int temp_which_cake;
		float temp_bite;

		for( int i = 0; i < G; i++) { //a bite to everybody
			temp_which_cake = kakMonster[i].which_cake;
			temp_bite = static_cast<float>( ((float)i - ( (float)temp_which_cake * (float)gpc ) ) / ( (float)gpc - 1) * ( ((float)S * (float)S ) - 1 ) );
			
			markBite( allaKakor, kakMonster, i, temp_which_cake, temp_bite ); //b�ttre att skicka �ver en kaka och en kak�tare, ist�llet f�r alla?
			addPoints( allaKakor, kakMonster, i, temp_which_cake, temp_bite, I);
		}//for

	};//startCut
////////////////////////////////////////////////////////
	int findJoyfullestBite(vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, int which_cake, int guest, int S, int I )//returning which bite
	{
		//vector<int>::iterator it = kakMonster[guest].cake_bites.begin();
		int temp_bite;
		int max_bite = -2;
		int temp_joy = 0;
		int max_joy = 0;
		int ss = S * S;

		//for( ; it != kakMonster[guest].cake_bites.end(); it++) {
		//	temp_bite = *it;

		for(int i = 0; i < kakMonster[guest].cake_bites.size(); i++) {
			temp_bite = kakMonster[guest].cake_bites[i]; 
			//cout << "temp_bite " << allaKakor[which_cake].kB[temp_bite].eaten << endl;
			//cout << "ss " << ss << endl;
			
			//up
			if( temp_bite - S >= 0 && allaKakor[which_cake].kB[temp_bite - S].eaten == -1 )
			{
				temp_joy = calcJoy( allaKakor, kakMonster, which_cake, guest, (temp_bite - S), I);
			//	cout << "temp_joy 1 " << temp_joy << endl;
				if( temp_joy > max_joy )
				{
					max_joy = temp_joy;
					max_bite = temp_bite - S;
				}//if
			}//if

			//down
			if( temp_bite + S < ss  && allaKakor[which_cake].kB[temp_bite + S].eaten == -1 )
			{
				temp_joy = calcJoy( allaKakor, kakMonster, which_cake, guest, (temp_bite + S), I);
				//cout << "temp_joy 2 " << temp_joy << endl;
				if( temp_joy > max_joy )
				{
					max_joy = temp_joy;
					max_bite = temp_bite + S;
				}//if
			}//if

			//left
			//if( temp_bite - 1 >= 0 && allaKakor[which_cake].kB[temp_bite - 1].eaten == -1 )
			if( temp_bite % S != 0 && allaKakor[which_cake].kB[temp_bite - 1].eaten == -1 )
			{
				temp_joy = calcJoy( allaKakor, kakMonster, which_cake, guest, (temp_bite - 1), I);
			//	cout << "temp_joy 3 " << temp_joy << endl;
				if( temp_joy > max_joy )
				{
					max_joy = temp_joy;
					max_bite = temp_bite - 1;
				}//if
			}//if

			//right
			//if( temp_bite + 1 < ss && allaKakor[which_cake].kB[temp_bite + 1].eaten == -1 )
			if( temp_bite % S != ( S - 1 ) && allaKakor[which_cake].kB[temp_bite + 1].eaten == -1 )
			{
				temp_joy = calcJoy( allaKakor, kakMonster, which_cake, guest, (temp_bite + 1), I);
				//cout << "temp_joy 4 " << temp_joy << endl;
				if( temp_joy > max_joy )
				{
					max_joy = temp_joy;
					max_bite = temp_bite + 1;
				}//if
			}//if

		}//for, test for all bites

		return max_bite;
	};//findJoyfullestBite
////////////////////////////////////////////////////////
	bool greaterPointsComp(kakEater lk, kakEater rk) 
	{
		if(lk.joyPoints < rk.joyPoints)
		return !(lk.joyPoints < rk.joyPoints); // kolla att r�tt
	}//greaterPointsComp
////////////////////////////////////////////////////////
	class CgreaterPointsComp : public binary_function<kakEater, kakEater, bool>
	{
	public:
		inline bool operator()( const kakEater lhs,const kakEater rhs) const//skall det vara &?
		{
		//	return !(lhs.joyPoints < rhs.joyPoints); // kolla att r�tt
			if( lhs.joyPoints < rhs.joyPoints)
				return false;
			if( lhs.joyPoints > rhs.joyPoints)
				return true;
			return false;
		}

	};//class CgreaterPointsComp

	void cutUp(vector<helKaka> &allaKakor,vector<kakEater> &kakMonster, priority_queue<kakEater,vector<kakEater>, CgreaterPointsComp>  &kakEaterQueue, int S, int I)
	{
		kakEater temp_guest = kakEaterQueue.top();
		kakEaterQueue.pop();
		int temp_id = temp_guest.id;
		//cout << "temp_id " << temp_id << endl;
		int temp_cake = temp_guest.which_cake;
		//cout << "temp_cake " << temp_cake << endl;
		int temp_bite= findJoyfullestBite(allaKakor, kakMonster, temp_cake, temp_id, S, I);
		//cout << "temp_bite " << temp_bite << endl;
		markBite(allaKakor, kakMonster, temp_id, temp_cake, temp_bite);
		addPoints(allaKakor, kakMonster, temp_id, temp_cake, temp_bite, I);
		kakEaterQueue.push(kakMonster[temp_id]);
		//cout << "Points: " << kakMonster[0].joyPoints << endl;
		
		while( !kakEaterQueue.empty() )
	//	for(int i = 0; i < 641; i++)
		{
			//if( i == 640 ) cout << "kakE " << kakEaterQueue.top().cake_bites.size() << endl;
			temp_guest = kakEaterQueue.top();
			kakEaterQueue.pop();
			temp_id = temp_guest.id;
			
			temp_cake = temp_guest.which_cake;
			//cout << "temp_cake " << temp_cake << endl;
			temp_bite= findJoyfullestBite(allaKakor, kakMonster, temp_cake, temp_id, S, I);
			//cout << "temp_bite " << temp_bite << endl;
			if( temp_bite != -2 )
			{
				markBite(allaKakor, kakMonster, temp_id, temp_cake, temp_bite);
				addPoints(allaKakor, kakMonster, temp_id, temp_cake, temp_bite, I);
				kakEaterQueue.push(kakMonster[temp_id]);
			}//if
		}//while

	};//cutUp
////////////////////////////////////////////////////////
	vector<int> split(int C, int G, int I, int S, vector<int> preferences, vector<int> cakes)
	{
		init(C, G, I, S, preferences, cakes);
		whichCake(C, G, I, S, preferences, cakes);
		startCut(C, G, I, S, preferences, cakes);
		//make a priority-queue
//		bool (*ptr_greaterPointsComp) (kakEater, kakEater);
//		ptr_greaterPointsComp = &Cakes::greaterPointsComp;
//		priority_queue<kakEater, vector<kakEater>, ptr_greaterPointsComp > kakEaterQueue;
		//cout << "Points: " << kakMonster[2].joyPoints << endl;

		priority_queue<kakEater,vector<kakEater>, CgreaterPointsComp> kakEaterQueue;
		for(int i = 0; i < G; i++) {
			kakEaterQueue.push(kakMonster[i]);

		}//for

		cutUp(allaKakor, kakMonster, kakEaterQueue, S, I);
				
		vector<int> ret;
		ret.reserve( C * S * S );

		int ss = S * S;
		for(int i = 0; i < C; i++)
		{
			for(int j = 0; j < ss; j++)
			{
				ret.push_back( allaKakor[i].kB[j].eaten);
			}//for j 
		}//for i

		return ret;
	}//split


};//Cakes
