//Cakes.h

#include<vector>
#include<algorithm>
#include<queue>
#include<climits>
#include<numeric>

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

	void init (const int C, const int G, const int I, const int S, const vector<int> &preferences, const vector<int> &cakes)
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
	void whichCake (const vector<helKaka> &allaKakor, vector<kakEater> &kakMonster , const int S, vector<int> &how_many_guests_per_cake )
	{
		int num_cakes = allaKakor.size();
		int num_guests = kakMonster.size();
		int ing = allaKakor[0].kB[0].ingredient.size();
		vector< vector<int> > guest_want_cake;
		vector<int> temp_want;
		int temp_sum = 0;

		int ss = S * S;
	
		for(int g = 0; g < num_guests; g++ ) {
			for(int c = 0; c < num_cakes; c++) {
				for(int s = 0; s < ss; s++) {
					for(int i = 0; i < ing; i++) {
						temp_sum += allaKakor[c].kB[s].ingredient[i] * kakMonster[g].prefs[i];
					}//for i
				}//for s
				temp_want.push_back(temp_sum);
				temp_sum = 0;
			}//for c
			guest_want_cake.push_back(temp_want);
			temp_want.clear();
		}//for g
		
		vector<int> number_per_cake (num_cakes, 1);

		vector<int> guests_max;
		int temp_max;

		//testa 2 olika initial startingar, översta minimala maxet får välja först
		//nästa, den som har minimala totalen får välja först

		int temp_total = 0;
		for(int i = 0; i < num_guests; i++) {
			temp_total = accumulate(guest_want_cake[i].begin(), guest_want_cake[i].end(), 0);
			guests_max.push_back(temp_total);
		}//for i

		//den andra varianten är borta, finns den i acer?
		//for(int i = 0; i < num_guests; i++) {
		//	temp_max = *max_element(guest_want_cake[i].begin(), guest_want_cake[i].end() );
		//	guests_max.push_back(temp_max);
		//}//for i

		for(int g = 0; g < num_guests; g++) {
			int who = min_element(guests_max.begin(), guests_max.end() ) - guests_max.begin();
			int temp_cake_value = 0;
			int max_cake_value = 0;
			int max_cake_nr = 0;
			vector<int> cake_need = guest_want_cake[who];
			for(int c = 0; c < num_cakes; c++) {
				temp_cake_value = cake_need[c] / number_per_cake[c];
				if(temp_cake_value > max_cake_value ) {
					max_cake_nr = c;
					max_cake_value = temp_cake_value;
				}//if
			}//for c

			kakMonster[who].which_cake = max_cake_nr;
			guests_max[who] = INT_MAX;
			number_per_cake[max_cake_nr] += 1;
			max_cake_value = 0;
			
		}//for g
	
		//swappa med alla andra till erhåller lägst poäng
		// fördela enligt lägsta min eller enligt lägsta totpoäng (enligt ovan) 

		bool has_changed = true;
		while (has_changed) {
			has_changed = false;
			//find mini
			int min_point = INT_MAX;
			int min_guest = 0;
			int temp_guest_cake;
			vector<int> temp_guest_need ;
			for(int i = 0; i < num_guests; i++) {
				temp_guest_need = guest_want_cake[i];
				int temp_guest_cake = kakMonster[i].which_cake;
				int temp_points = temp_guest_need[temp_guest_cake] / number_per_cake[temp_guest_cake];
				
				if( min_point > temp_points )
				{
					min_point = temp_points;
					min_guest = i;
				}//if
			}//for

			//nu har jag hittat gästen med lägst chans att få många poäng
			// gå igenom alla gäster, hitta min_

			int min_guest_cake = kakMonster[min_guest].which_cake;
			vector<int> min_guest_need = guest_want_cake[min_guest];

			//int min_guest_points = min_guest_need[
			int swapee_max_points = 0;
			int swapee = -1;
			
			for(int i = 0; i < num_guests; i++) {
				int temp_whick_cake = kakMonster[i].which_cake;
				if( i == min_guest || temp_whick_cake == min_guest_cake )
				{	continue; }

				temp_guest_need= guest_want_cake[i];
				//simulate a swap
				//kolla först at min_guest får mer poäng
				if( min_guest_need[temp_whick_cake] / number_per_cake[temp_whick_cake] > min_point )
				{
					//kolla sedan att temp_guest inte får LÄGRE poäng än min_guest
					int temp_guest_points = temp_guest_need[min_guest_cake] / number_per_cake[min_guest_cake];
					if( temp_guest_points > min_point )
					{
						//kolla sedan att att det blir högsta swapee_points
						if( temp_guest_points > swapee_max_points )
						{
							swapee_max_points = temp_guest_points;
							swapee = i;
						}//if
					}//if
				}//if
			}//for

			if(swapee != -1) {
				has_changed = true;
				int temp = kakMonster[min_guest].which_cake;
				kakMonster[min_guest].which_cake = kakMonster[swapee].which_cake;
				kakMonster[swapee].which_cake = temp;

			}

		}//while

		for(int c = 0; c < num_cakes; c++)
		{
			how_many_guests_per_cake[c] = number_per_cake[c] - 1;
		}// for c

	};//whichCake

////////////////////////////////////////////////////////
	void markBite(vector<helKaka> &allaKakor, vector<kakEater> &kakMonster,const int guest, const int temp_which_cake, const int temp_bite )
	{
		//allaKakor[temp_which_cake].kB[temp_bite].eaten = kakMonster[guest].id;
		allaKakor[temp_which_cake].kB[temp_bite].eaten = guest;
	};//markBite

/////////////////////////////////////////////////////////////////
	int calcJoy( const vector<helKaka> &allaKakor, const vector<kakEater> &kakMonster, const int which_cake, const int guest, const int temp_bite, const int I)
	{
		int temp_joy = 0;
		for(int i = 0; i < I; i++) {
			temp_joy += kakMonster[guest].prefs[i] * allaKakor[which_cake].kB[temp_bite].ingredient[i];
		}//for

		return temp_joy;
	};//calcJoy

////////////////////////////////////////////////////////
	void addPoints( const vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, const int guest, const int temp_which_cake, const int temp_bite, const int I )
	{
		kakMonster[guest].joyPoints += calcJoy(allaKakor, kakMonster, temp_which_cake, guest, temp_bite, I);
		kakMonster[guest].cake_bites.push_back(temp_bite);

	};//addPoints

////////////////////////////////////////////////////////
	void subPoints(const vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, const int guest, const int temp_which_cake, const int temp_bite, const int I )
	{
		kakMonster[guest].joyPoints -= calcJoy(allaKakor, kakMonster, temp_which_cake, guest, temp_bite, I);
		kakMonster[guest].cake_bites.erase( remove( kakMonster[guest].cake_bites.begin(), kakMonster[guest].cake_bites.end(), temp_bite), kakMonster[guest].cake_bites.end() );

	};//subPoints

////////////////////////////////////////////////////////
	void addNsubPoints(const vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, const int winner, const int loser, const int temp_which_cake, const int temp_bite, const int I )
	{
		addPoints(allaKakor, kakMonster, winner, temp_which_cake, temp_bite, I);
		subPoints(allaKakor, kakMonster, loser, temp_which_cake, temp_bite, I); 
	};//addNsubPoints

////////////////////////////////////////////////////////
	void startCut ( vector<helKaka> &allaKakor, vector<kakEater> &kakMonster ,const int S, const int C, const int I, const vector<int> &how_many_guests_per_cake)
	{ 
		//how_many, bestämms i whichCake
		
		//cout << "how many " << how_many_guests_per_cake[0] << endl;

		for(int c = 0; c < C; c++) {
			int gpc = how_many_guests_per_cake[c];

			switch(gpc)
			{
			case 2:
				{
				int num_g = 0;
				vector<kakEater>::iterator it = kakMonster.begin();
				float x [2] = { 0, 1 };
				float y [2] = { 0, 1 };
				}
				break;
			case 3:
				{
				int num_g = 0;
				vector<kakEater>::iterator it = kakMonster.begin();
				float x [3] = { 0, 1, 0.33 };
				float y [3] = { 0, 0.33, 1 };
				}
				break;
				
			case 4:
				{
				int num_g = 0;
				vector<kakEater>::iterator it = kakMonster.begin();
				float x [4] = { 0, 1, 1, 0 };
				float y [4] = { 0, 0, 1, 1 };
				}
				break;
				
			case 5:
				{
				int num_g = 0;
				vector<kakEater>::iterator it = kakMonster.begin();
				float x [5] = { 0, 0.8, 1, 0.6, 0 };
				float y [5] = { 0, 0, 0.6, 1, 0.8 };
				}
				break;
				
			case 6:
				{
				int num_g = 0;
				vector<kakEater>::iterator it = kakMonster.begin();
				float x [6] = { 0, 0.66, 1, 1, 0.33, 0 };
				float y [6] = { 0, 0, 0.33, 1, 1, 0.67 };
				}
				break;
				
			case 7:
				{
				int num_g = 0;
				vector<kakEater>::iterator it = kakMonster.begin();
				float x [7] = { 0, 0.57, 1, 1, 0.71, 0.14, 0 };
				float y [7] = { 0, 0.14, 0.71, 1, 1, 0.57 };
				}
				break;
				
			case 8:
				{
				int num_g = 0;
				vector<kakEater>::iterator it = kakMonster.begin();
				float x [8] = { 0, 0.5, 1, 1, 1, 0.5, 0, 0 };
				float y [8] = { 0, 0, 0, 0.5, 1, 1, 1, 0.5 };
				}
				break;
				
			case 9:
				{
				int num_g = 0;
				vector<kakEater>::iterator it = kakMonster.begin();
				float x [9] = { 0, 0.45, 0.89,	  1,    1, 0.78, 0.34,    0,    0 };
				float y [9] = { 0,	  0,	0, 0.34, 0.78,    1,    1, 0.89, 0.45 };
				//float x [9] = { 0, 4/9 , 8/9 , 1, 1, 16/9 - 1, 3 - 24 / 9, 0, 0 };
				//float y [9] = { 0, 0, 0, 0.34, 0.78, 1, 1, 0.89, 0.45 };
				}
				break;
				
			case 10:
				{
				int num_g = 0;
				vector<kakEater>::iterator it = kakMonster.begin();
				float x [10] = { 0, 0.4, 0.8, 1, 1, 1, 0.6, 0.2, 0, 0 };
				float y [10] = { 0, 0, 0, 0.2, 0.6, 1, 1, 1, 0.8, 0.4 };
				}
				break;
			}//switch

			for( ; it != kakMonster.end(); it++) {
					if( (*it).which_cake == c ) {
						//int temp_bite = (  ( S - 1 ) * ( S * y[num_g] + x[num_g] ) ) ;
						int temp_bite = ( S - 1) * ( x[num_g] + S * y[num_g] ) ;
						num_g++;
			
						markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
						addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
					}//if
				}//for


		}//for

		//for(int c = 0; c < C; c++) {
		//	int gpc = how_many_guests_per_cake[c];

		//	switch(gpc)
		//	{
		//	case 2:
		//		{
		//		int num_g = 0;
		//		vector<kakEater>::iterator it = kakMonster.begin();
		//		float x [2] = { 0, 1 };
		//		float y [2] = { 0, 1 };
		//		for( ; it != kakMonster.end(); it++) {
		//			if( (*it).which_cake == c ) {
		//				int temp_bite = (  ( S - 1 ) * ( S * y[num_g] + x[num_g] ) ) ;
		//				num_g++;
		//	
		//				markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
		//				addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
		//			}//if
		//		}//for

		//		}
		//		break;
		//	case 3:
		//		{
		//		int num_g = 0;
		//		vector<kakEater>::iterator it = kakMonster.begin();
		//		float x [3] = { 0, 1, 0.33 };
		//		float y [3] = { 0, 0.33, 1 };
		//		for( ; it != kakMonster.end(); it++) {
		//			if( (*it).which_cake == c ) {
		//				int temp_bite = (  ( S - 1 ) * ( S * y[num_g] + x[num_g] ) ) ;
		//				num_g++;
		//	
		//				markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
		//				addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
		//			}//if
		//		}//for

		//		}
		//		break;
		//		
		//	case 4:
		//		{
		//		int num_g = 0;
		//		vector<kakEater>::iterator it = kakMonster.begin();
		//		float x [4] = { 0, 1, 1, 0 };
		//		float y [4] = { 0, 0, 1, 1 };
		//		for( ; it != kakMonster.end(); it++) {
		//			if( (*it).which_cake == c ) {
		//				int temp_bite = (  ( S - 1 ) * ( S * y[num_g] + x[num_g] ) ) ;
		//				num_g++;
		//	
		//				markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
		//				addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
		//			}//if
		//		}//for

		//		}
		//		break;
		//		
		//	case 5:
		//		{
		//		int num_g = 0;
		//		vector<kakEater>::iterator it = kakMonster.begin();
		//		float x [5] = { 0, 0.8, 1, 0.6, 0 };
		//		float y [5] = { 0, 0, 0.6, 1, 0.8 };
		//		for( ; it != kakMonster.end(); it++) {
		//			if( (*it).which_cake == c ) {
		//				int temp_bite = (  ( S - 1 ) * ( S * y[num_g] + x[num_g] ) ) ;
		//				num_g++;
		//	
		//				markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
		//				addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
		//			}//if
		//		}//for

		//		}
		//		break;
		//		
		//	case 6:
		//		{
		//		int num_g = 0;
		//		vector<kakEater>::iterator it = kakMonster.begin();
		//		float x [6] = { 0, 0.66, 1, 1, 0.33, 0 };
		//		float y [6] = { 0, 0, 0.33, 1, 1, 0.67 };
		//		for( ; it != kakMonster.end(); it++) {
		//			if( (*it).which_cake == c ) {
		//				int temp_bite = (  ( S - 1 ) * ( S * y[num_g] + x[num_g] ) ) ;
		//				num_g++;
		//	
		//				markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
		//				addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
		//			}//if
		//		}//for

		//		}
		//		break;
		//		
		//	case 7:
		//		{
		//		int num_g = 0;
		//		vector<kakEater>::iterator it = kakMonster.begin();
		//		float x [7] = { 0, 0.57, 1, 1, 0.71, 0.14, 0 };
		//		float y [7] = { 0, 0.14, 0.71, 1, 1, 0.57 };
		//		for( ; it != kakMonster.end(); it++) {
		//			if( (*it).which_cake == c ) {
		//				int temp_bite = (  ( S - 1 ) * ( S * y[num_g] + x[num_g] ) ) ;
		//				num_g++;
		//	
		//				markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
		//				addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
		//			}//if
		//		}//for

		//		}
		//		break;
		//		
		//	case 8:
		//		{
		//		int num_g = 0;
		//		vector<kakEater>::iterator it = kakMonster.begin();
		//		float x [8] = { 0, 0.5, 1, 1, 1, 0.5, 0, 0 };
		//		float y [8] = { 0, 0, 0, 0.5, 1, 1, 1, 0.5 };
		//		for( ; it != kakMonster.end(); it++) {
		//			if( (*it).which_cake == c ) {
		//				int temp_bite = (  ( S - 1 ) * ( S * y[num_g] + x[num_g] ) ) ;
		//				num_g++;
		//	
		//				markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
		//				addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
		//			}//if
		//		}//for

		//		}
		//		break;
		//		
		//	case 9:
		//		{
		//		int num_g = 0;
		//		vector<kakEater>::iterator it = kakMonster.begin();
		//		float x [9] = { 0, 0.45, 0.89,	  1,    1, 0.78, 0.34,    0,    0 };
		//		float y [9] = { 0,	  0,	0, 0.34, 0.78,    1,    1, 0.89, 0.45 };
		//		//float x [9] = { 0, 4/9 , 8/9 , 1, 1, 16/9 - 1, 3 - 24 / 9, 0, 0 };
		//		//float y [9] = { 0, 0, 0, 0.34, 0.78, 1, 1, 0.89, 0.45 };
		//		for( ; it != kakMonster.end(); it++) {
		//			if( (*it).which_cake == c ) {
		//				//int temp_bite = (  ( S - 1 ) * ( S * y[num_g] + x[num_g] ) ) ;
		//				int temp_bite = ( S - 1) * ( x[num_g] + S * y[num_g] ) ;
		//				num_g++;
		//	
		//				markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
		//				addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
		//			}//if
		//		}//for

		//		}
		//		break;
		//		
		//	case 10:
		//		{
		//		int num_g = 0;
		//		vector<kakEater>::iterator it = kakMonster.begin();
		//		float x [10] = { 0, 0.4, 0.8, 1, 1, 1, 0.6, 0.2, 0, 0 };
		//		float y [10] = { 0, 0, 0, 0.2, 0.6, 1, 1, 1, 0.8, 0.4 };
		//		for( ; it != kakMonster.end(); it++) {
		//			if( (*it).which_cake == c ) {
		//				int temp_bite = (  ( S - 1 ) * ( S * y[num_g] + x[num_g] ) ) ;
		//				num_g++;
		//	
		//				markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
		//				addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
		//			}//if
		//		}//for

		//		}
		//		break;

		//	}//switch


		//}//for

	//	for( int c = 0; c < C; c++) {
	//		int gpc = how_many_guests_per_cake[c];
	//		
	//	//	for( int g = 0; g < gpc; g++) {
	//			int num_g = 0;
	//			vector<kakEater>::iterator it = kakMonster.begin();
	//			for( ; it != kakMonster.end(); it++) {
	//				if( (*it).which_cake == c ) {
	//					int temp_bite = ( (float)S * (float)S - 1) * (float)num_g  / ( (float)gpc - 1) ;
	//					num_g++;
	//					//(*it).cake_bites.push_back(temp_bite); // görs i addPoints
	//					markBite(allaKakor, kakMonster, (*it).id, c, temp_bite);
	//					addPoints(allaKakor, kakMonster, (*it).id, c, temp_bite, I);
	//				}//if

	//			}// for it
	////		}//for g
	//	}//for c
				
	};//startCut

////////////////////////////////////////////////////////
	int findJoyfullestBite( const vector<helKaka> &allaKakor, const vector<kakEater> &kakMonster, const int which_cake, const int guest, const int S, const int I )//returning which bite
	{
		int temp_bite;
		int max_bite = -2;
		int temp_joy = 0;
		int max_joy = 0;
		int ss = S * S;

		for(int i = 0; i < kakMonster[guest].cake_bites.size(); i++) {
			temp_bite = kakMonster[guest].cake_bites[i]; 
			
			//up
			if( temp_bite - S >= 0 && allaKakor[which_cake].kB[temp_bite - S].eaten == -1 )
			{
				temp_joy = calcJoy( allaKakor, kakMonster, which_cake, guest, (temp_bite - S), I);
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
				if( temp_joy > max_joy )
				{
					max_joy = temp_joy;
					max_bite = temp_bite + S;
				}//if
			}//if

			//left
			if( temp_bite % S != 0 && allaKakor[which_cake].kB[temp_bite - 1].eaten == -1 )
			{
				temp_joy = calcJoy( allaKakor, kakMonster, which_cake, guest, (temp_bite - 1), I);
				if( temp_joy > max_joy )
				{
					max_joy = temp_joy;
					max_bite = temp_bite - 1;
				}//if
			}//if

			//right
			if( temp_bite % S != ( S - 1 ) && allaKakor[which_cake].kB[temp_bite + 1].eaten == -1 )
			{
				temp_joy = calcJoy( allaKakor, kakMonster, which_cake, guest, (temp_bite + 1), I);
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
		return !(lk.joyPoints < rk.joyPoints); // kolla att rätt
	}//greaterPointsComp

////////////////////////////////////////////////////////
	class CgreaterPointsComp : public binary_function<kakEater, kakEater, bool>
	{
	public:
		inline bool operator()( const kakEater lhs,const kakEater rhs) const//skall det vara &?
		{
			if( lhs.joyPoints < rhs.joyPoints)
				return false;
			if( lhs.joyPoints > rhs.joyPoints)
				return true;
			return false;
		}

	};//class CgreaterPointsComp

////////////////////////////////////////////////
	void cutUp( vector<helKaka> &allaKakor, vector<kakEater> &kakMonster, priority_queue<kakEater,vector<kakEater>, CgreaterPointsComp>  &kakEaterQueue, const int S, const int I)
	{
		kakEater temp_guest = kakEaterQueue.top();
		kakEaterQueue.pop();
		int temp_id = temp_guest.id;
		int temp_cake = temp_guest.which_cake;
		int temp_bite= findJoyfullestBite(allaKakor, kakMonster, temp_cake, temp_id, S, I);
		markBite(allaKakor, kakMonster, temp_id, temp_cake, temp_bite);
		addPoints(allaKakor, kakMonster, temp_id, temp_cake, temp_bite, I);
		kakEaterQueue.push(kakMonster[temp_id]);
	
		bool has_changed = true;
		bool first_not_changed = true;
		int temp_min = INT_MAX;
	
		while( !kakEaterQueue.empty() )
		//while( has_changed )
	//	for(int i = 0; i < 641; i++)
		{
			has_changed = false;
			temp_guest = kakEaterQueue.top();
			kakEaterQueue.pop();
			temp_id = temp_guest.id;
			temp_cake = temp_guest.which_cake;
	
			if( temp_guest.joyPoints > temp_min ) {
				break;
			}

			temp_bite= findJoyfullestBite(allaKakor, kakMonster, temp_cake, temp_id, S, I);

			if( temp_bite != -2 )
			{
				markBite(allaKakor, kakMonster, temp_id, temp_cake, temp_bite);
				addPoints(allaKakor, kakMonster, temp_id, temp_cake, temp_bite, I);
				kakEaterQueue.push(kakMonster[temp_id]);

				has_changed = true;
			//	temp_min = kakMonster[temp_id].joyPoints;
			}//if
			else if( first_not_changed == true ) {
				temp_min = kakMonster[temp_id].joyPoints;
				first_not_changed = false;
			}//else

		}//while
		
	};//cutUp

////////////////////////////////////////////////////////
	vector<int> split(int C, int G, int I, int S, vector<int> preferences, vector<int> cakes)
	{
		init(C, G, I, S, preferences, cakes);

		vector<int> how_many_guests_per_cake (C, 0);
		whichCake(allaKakor, kakMonster , S, how_many_guests_per_cake );
		//startCut(C, G, I, S, preferences, cakes);
		startCut(allaKakor, kakMonster , S, C, I, how_many_guests_per_cake);

		//föra över prio-kön till cutUp
		priority_queue<kakEater,vector<kakEater>, CgreaterPointsComp> kakEaterQueue;
		for(int i = 0; i < G; i++) {
			kakEaterQueue.push(kakMonster[i]);

		}//for
//		cutUp(allaKakor, kakMonster, kakEaterQueue, S, I);
//		cout << "nr 28 " << kakMonster[28].which_cake << endl;

		//output
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
