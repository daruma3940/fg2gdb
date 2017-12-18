/*===============================================================================

このソフトはfloodgateの棋譜を技巧で学習するための形式に変換するためのものである
参考：fggather


（技巧の形式）
<pre>
1行目: <棋譜番号> <対局開始日> <先手名> <後手名> <勝敗(0:引き分け,1:先手勝ち,2:後手勝ち)> <手数> <棋戦> <戦型>
2行目: <CSA形式の指し手(1手6文字)を一行に並べたもの>
</pre>

（floodgateの形式）

N+Gikou_i3_5010U
N-SILENT_MAJORITY_i7-3517U
'Max_Moves:256
'Least_Time_Per_Move:0
'Increment:10
$EVENT:wdoor+floodgate-600-10F+Gikou_i3_5010U+SILENT_MAJORITY_i7-3517U+20160629110002
$START_TIME:2016/06/29 11:00:02
P1-KY-KE-GI-KI-OU-KI-GI-KE-KY
P2 * -HI *  *  *  *  * -KA *
P3-FU-FU-FU-FU-FU-FU-FU-FU-FU
P4 *  *  *  *  *  *  *  *  *
P5 *  *  *  *  *  *  *  *  *
P6 *  *  *  *  *  *  *  *  *
P7+FU+FU+FU+FU+FU+FU+FU+FU+FU
P8 * +KA *  *  *  *  * +HI *
P9+KY+KE+GI+KI+OU+KI+GI+KE+KY
+
'rating:Gikou_i3_5010U+84a8e60cfef7d11ce017595eea78c60d:SILENT_MAJORITY_i7-3517U+e900b38f1abb4c21681fe96a7e3277b4
'black_rate:Gikou_i3_5010U+84a8e60cfef7d11ce017595eea78c60d:3189.0
'white_rate:SILENT_MAJORITY_i7-3517U+e900b38f1abb4c21681fe96a7e3277b4:3215.0
+7776FU
T0
'** 104
-8384FU
（ここに思考と指し手がずらっと並べられる。）
%TORYO
'P1-KY *  *  *  *  *  * -OU-KY
'P2 *  *  *  *  *  *  * -FU *
'P3 *  *  * -GI * -FU-KI * -FU
'P4 *  *  *  *  *  * +FU+FU *
'P5-FU * +KI-GI-FU *  *  *  *
'P6 *  * +FU+FU+KA * -KE *  *
'P7+KY *  * -KE * +FU *  * +FU
'P8 *  * +GI-FU+FU *  * -KI+KY
'P9-RY+FU *  * +GI+OU-KI *  *
'P+00FU00FU00HI00KA00KE00KE
'P-00FU00FU
'+
'summary:toryo:Gikou_i3_5010U lose:SILENT_MAJORITY_i7-3517U win
'$END_TIME:2016/06/29 11:45:26

================================================================================*/


#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <crtdbg.h>
#include <string.h>


#include "main.h"

using namespace std;
namespace sys = std::tr2::sys;

#define Test_directory "C:/Users/daruma/Desktop/wdoor2015/2015/"
//"C:/Users/daruma/Desktop/wdoor2015/TEST/"
//"c:/book/"      
//"C:/Users/daruma/Desktop/wdoor2015/TEST/"
#define MAX_data 100000
//#define TEST

#ifdef TEST
#define TEST_outputfile  "C:/Users/daruma/Desktop/records.txt"  
#else
#define TEST_outputfile "records.txt"
#endif
//"c:/book/record.txt"
//"C:/Users/daruma/Desktop/records.txt"  
//"./records.txt"


typedef string filepath;
std::vector<filepath> pathlist;//ファイルパスのリスト

GameData gd;

int writed_games=0;
//--------------------------------------------------------------------------------------
//指定されたディレクトリの**.csaを発見し
//そのファイルへのパスをlist[]に保管する。
//ファイルは再帰的に見つける（OK）
//--------------------------------------------------------------------------------------
void find_csa(const char *path, std::vector<filepath>& list_) {

	std::cout << "csa file finding";

	//まずはディレクトリ内で見つけたファイルの列挙
	int filenum = 0;
	sys::path p(path); // 列挙の起点

	//再帰的に
	//参考：　http://qiita.com/episteme/items/0e3c2ee8a8c03780f01e
	std::for_each(sys::recursive_directory_iterator(p), sys::recursive_directory_iterator(),
		[&](const sys::path& p) {
		if (sys::is_regular_file(p)) { // ファイルなら...
			//cout << path << p.filename() << endl;
			if (p.filename().string().find("csa")!=string::npos) {
				list_.push_back(p.string());
				if (filenum++ % 50000 == 0)
					std::cout << ".";
			}
		}
	});
	cout <<"file number"<< filenum << endl;
}

//spaceを_に変えるための関数　（spaceはセパレーターなのでこう変換する）
string space2bar(string str) {

	string rtn = str;
	int found = str.find(' ');
	if(found!=string::npos)
		rtn[found] = '_';

	return rtn;
}

// -------------------------------------------------------------------------------------------
//  ファイルの丸読み
// -------------------------------------------------------------------------------------------

// ファイルを丸読みする。ファイルが存在しなくともエラーにはならない。空行はスキップする。
//参考：　やねうら王
int read_all_lines(std::string filename, std::vector<std::string>& lines)
{
	fstream fs(filename, ios::in);
	if (fs.fail()) {
		cout << "can't open" << endl;
		return 1; // 読み込み失敗
	}

	while (!fs.fail() && !fs.eof())
	{
		std::string line;
		getline(fs, line);
		if (line.length())
			lines.push_back(line);
	}
	fs.close();
	return 0;
}

//gikou形式で書き出し
int write_game_info(std::string filename,GameData GD) {

	fstream fs;
	//上書きモードにしないといけない
	fs.open(filename, ios::app);

	//1行目
	//<棋譜番号> <対局開始日> <先手名> <後手名> <勝敗(0:引き分け,1:先手勝ち,2:後手勝ち)> <手数> <棋戦> <戦型> (わからないところはすべて0にしてある。)
	fs << "0"<<" " << GD.day <<" "<< GD.black_name <<" "<< GD.white_name << " " <<GD.VorD<<" "<<GD.plys<<" 0"<<" 0"<< endl;
	//2行目
	// <CSA形式の指し手(1手6文字)を一行に並べたもの>(空行は入れなくていいはずである)
	for (auto move : GD.moves) {
		_ASSERT(move.size() == 6|| move.size() == 0);
		fs << move;
	}
	fs << endl;
	return 0;
}


//棋譜を丸読みしてそこから試合の情報を読み取り、条件を満たせば学習用DBとして書き出す。
//これがこのプログラムの肝！！！！！！！！！！！！！！！！！！！！！！！！！！！
int read_game_info(std::vector<filepath>& list_) {

	vector<string> lines;

	cout << "writing";

	int num = 0;

	//string::npos;
	for (auto filename : list_) {

		if (num++ % 1000 == 0)
			cout << ".";

		if (
			read_all_lines(filename, lines)
			) {
			cout << "read error" << endl; continue;
		}//ここがおかしい！
		 //ここまでおｋ

		for (auto line : lines) {

			//ここが肝！！！！！！！！！

			if (line.find("N+") != string::npos) { gd.black_name = space2bar(line.substr(2)); }//OK
			else if (line.find("N-") != string::npos) gd.white_name = space2bar(line.substr(2));//OK
			else if (line.find("START_TIME") != string::npos)gd.day = space2bar(line.substr(12));//OK
			else if (line.find("$EVENT") != string::npos)gd.event = space2bar(line.substr(7));//OK
			//どうやって後ろから文字列を抜き出す？？
			//参考:http://simd.jugem.jp/?eid=131 
			//少し汚い
			else if (line.find("black_rate") != string::npos) {//OK

				const char *str = line.c_str();
				int c = ':';
				const char *ret= strrchr(str, c);//後ろから調べる。

				int num = ret - str+1;

				gd.black_rate=std::stoi(line.substr(num));
				_ASSERT(0<gd.black_rate&&gd.black_rate < 4000);//おかしな値になっていないか
				if (gd.black_rate < 2800) goto NO_WRITE;//rate 3000未満であれば書き出さない
			}
			else if (line.find("white_rate") != string::npos) {

				const char *str = line.c_str();
				int c = ':';
				const char *ret = strrchr(str, c);

				int num = ret - str + 1;

				gd.white_rate = std::stoi(line.substr(num));
				//Rは4000以下であるとしていたがelmoが出てきたのでこれではだめになってしまった(;^_^Aコンピュータ将棋の進歩は早いなぁ
				//まあさすがに1万は超えないだろう...
				_ASSERT(0<gd.white_rate&&gd.white_rate < 10000);
				if (gd.white_rate < 2800) goto NO_WRITE;
			}
			else if (line.find("summary") != string::npos) {

				//この条件の場合はおかしい試合であるので書き出さない
				if ((gd.plys<50&&line.find(":toryo:") != string::npos)
					|| (gd.plys<50 && line.find(":kachi:") != string::npos)
					|| (gd.plys<50 && line.find(":sennichite:") != string::npos)
					|| (gd.plys<50 && line.find(":time up:") != string::npos)
					|| line.find(":abnormal:") != string::npos
					|| line.find(":illegal move:") != string::npos
					|| line.find(":oute_sennichite:") != string::npos
					)
				{
					goto NO_WRITE;
				}
				else {

					const char *str = line.c_str();
					int c = ':';
					const char *ret = strrchr(str, c);//最後の：より後ろの文字列


				
					
					
					string summary = string(ret);
					if (summary.find("lose") != string::npos) {
						gd.VorD = 1;//先手の勝ち
					}
					else if (summary.find("win") != string::npos) {
						gd.VorD = 2;//後手の勝ち
					}
					else {
						gd.VorD = 0;
					}
				}
			}//end of summary
			 //moves
			else if (line[0] == '+' || line[0] == '-') {
				string move_str = line.substr(1, 6);
				gd.moves.push_back(move_str);
				gd.plys++;
				_ASSERT(gd.plys <= 257);
			}



		}//endof foreach line_

		if (gd.black_rate != 0 && gd.white_rate != 0 && gd.plys > 0) {
			write_game_info(string(TEST_outputfile), gd);
			writed_games++;
		}
	NO_WRITE:
		gd.clear();
		lines.clear();
	}//endof foreach list_


	return 0;
}

//使い方
void useage() {

	std::cerr << "このプログラムはfloodgate形式の棋譜を技巧の学習形式に変換するためのプログラムです。" << endl
		<< "先手と後手のレートが求まっている場合はそれらが3000以上でなければ変換して書き出したりはしません" << endl
		<< "使用方法" << endl
		<< "fg2gdb [folder_name]" << endl
		<< "[foldername]で指定されたフォルダ内の*.csaファイルを再帰的に探します。" << endl;

}


//--------------------------main関数！！------------------------------------------------------
int main(int argc, char **argv) {

#ifdef TEST
	find_csa(Test_directory, pathlist);

#endif
#ifndef TEST
	if (argc <= 1) {useage(); exit(1);}

	//string filepath_command=string(argv[1]);
	find_csa(argv[1], pathlist);

#endif
	read_game_info(pathlist);

	cout << "finish! writedgames"<<writed_games << endl;

}
