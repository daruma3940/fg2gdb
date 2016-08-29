#pragma once
#include <string>
#include <vector>
using namespace std;

/*
確保するのは1個だけで良い。使い終わったらclearして一個を使い回す。
*/
struct GameData {

public:
	string black_name;
	string white_name;
	string day;
	string event;
	int VorD;//Victory or defeat(勝敗) (0:引き分け,1:先手勝ち,2:後手勝ち)
	int plys=0;

	//rateが3000に満たないような奴らの試合はDBに書き出さない
	int black_rate=0;
	int white_rate=0;
	//棋戦　戦型はどうしようもない

	vector<string> moves;
public:
	void clear(){ 
		black_name.clear();
		white_name.clear();
		day.clear();
		VorD = plys = black_rate = white_rate = 0;
		moves.clear();
	}
};