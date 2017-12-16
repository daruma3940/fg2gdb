/*===============================================================================

���̃\�t�g��floodgate�̊������Z�I�Ŋw�K���邽�߂̌`���ɕϊ����邽�߂̂��̂ł���
�Q�l�Ffggather


�i�Z�I�̌`���j
<pre>
1�s��: <�����ԍ�> <�΋ǊJ�n��> <��薼> <��薼> <���s(0:��������,1:��菟��,2:��菟��)> <�萔> <����> <��^>
2�s��: <CSA�`���̎w����(1��6����)����s�ɕ��ׂ�����>
</pre>

�ifloodgate�̌`���j

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
�i�����Ɏv�l�Ǝw���肪������ƕ��ׂ���B�j
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
std::vector<filepath> pathlist;//�t�@�C���p�X�̃��X�g

GameData gd;

int writed_games=0;
//--------------------------------------------------------------------------------------
//�w�肳�ꂽ�f�B���N�g����**.csa�𔭌���
//���̃t�@�C���ւ̃p�X��list[]�ɕۊǂ���B
//�t�@�C���͍ċA�I�Ɍ�����iOK�j
//--------------------------------------------------------------------------------------
void find_csa(const char *path, std::vector<filepath>& list_) {

	std::cout << "csa file finding";

	//�܂��̓f�B���N�g�����Ō������t�@�C���̗�
	int filenum = 0;
	sys::path p(path); // �񋓂̋N�_

	//�ċA�I��
	//�Q�l�F�@http://qiita.com/episteme/items/0e3c2ee8a8c03780f01e
	std::for_each(sys::recursive_directory_iterator(p), sys::recursive_directory_iterator(),
		[&](const sys::path& p) {
		if (sys::is_regular_file(p)) { // �t�@�C���Ȃ�...
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

//space��_�ɕς��邽�߂̊֐��@�ispace�̓Z�p���[�^�[�Ȃ̂ł����ϊ�����j
string space2bar(string str) {

	string rtn = str;
	int found = str.find(' ');
	if(found!=string::npos)
		rtn[found] = '_';

	return rtn;
}

// -------------------------------------------------------------------------------------------
//  �t�@�C���̊ۓǂ�
// -------------------------------------------------------------------------------------------

// �t�@�C�����ۓǂ݂���B�t�@�C�������݂��Ȃ��Ƃ��G���[�ɂ͂Ȃ�Ȃ��B��s�̓X�L�b�v����B
//�Q�l�F�@��˂��牤
int read_all_lines(std::string filename, std::vector<std::string>& lines)
{
	fstream fs(filename, ios::in);
	if (fs.fail()) {
		cout << "can't open" << endl;
		return 1; // �ǂݍ��ݎ��s
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

//gikou�`���ŏ����o��
int write_game_info(std::string filename,GameData GD) {

	fstream fs;
	//�㏑�����[�h�ɂ��Ȃ��Ƃ����Ȃ�
	fs.open(filename, ios::app);

	//1�s��
	//<�����ԍ�> <�΋ǊJ�n��> <��薼> <��薼> <���s(0:��������,1:��菟��,2:��菟��)> <�萔> <����> <��^> (�킩��Ȃ��Ƃ���͂��ׂ�0�ɂ��Ă���B)
	fs << GD.event<<" " << GD.day <<" "<< GD.black_name <<" "<< GD.white_name << " " <<GD.VorD<<" "<<GD.plys<<" 0"<<" 0"<< endl;
	//2�s��
	// <CSA�`���̎w����(1��6����)����s�ɕ��ׂ�����>(��s�͓���Ȃ��Ă����͂��ł���)
	for (auto move : GD.moves) {
		_ASSERT(move.size() == 6|| move.size() == 0);
		fs << move;
	}
	fs << endl;
	return 0;
}


//�������ۓǂ݂��Ă������玎���̏���ǂݎ��A�����𖞂����Ίw�K�pDB�Ƃ��ď����o���B
//���ꂪ���̃v���O�����̊́I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I
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
		}//���������������I
		 //�����܂ł���

		for (auto line : lines) {

			//�������́I�I�I�I�I�I�I�I�I

			if (line.find("N+") != string::npos) { gd.black_name = space2bar(line.substr(2)); }//OK
			else if (line.find("N-") != string::npos) gd.white_name = space2bar(line.substr(2));//OK
			else if (line.find("START_TIME") != string::npos)gd.day = space2bar(line.substr(12));//OK
			else if (line.find("$EVENT") != string::npos)gd.event = space2bar(line.substr(7));//OK
			//�ǂ�����Č�납�當����𔲂��o���H�H
			//�Q�l:http://simd.jugem.jp/?eid=131 
			//��������
			else if (line.find("black_rate") != string::npos) {//OK

				const char *str = line.c_str();
				int c = ':';
				const char *ret= strrchr(str, c);//��납�璲�ׂ�B

				int num = ret - str+1;

				gd.black_rate=std::stoi(line.substr(num));
				_ASSERT(0<gd.black_rate&&gd.black_rate < 4000);//�������Ȓl�ɂȂ��Ă��Ȃ���
				if (gd.black_rate < 2800) goto NO_WRITE;//rate 3000�����ł���Ώ����o���Ȃ�
			}
			else if (line.find("white_rate") != string::npos) {

				const char *str = line.c_str();
				int c = ':';
				const char *ret = strrchr(str, c);

				int num = ret - str + 1;

				gd.white_rate = std::stoi(line.substr(num));
				//R��4000�ȉ��ł���Ƃ��Ă�����elmo���o�Ă����̂ł���ł͂��߂ɂȂ��Ă��܂���(;^_^A�R���s���[�^�����̐i���͑����Ȃ�
				//�܂���������1���͒����Ȃ����낤...
				_ASSERT(0<gd.white_rate&&gd.white_rate < 10000);
				if (gd.white_rate < 2800) goto NO_WRITE;
			}
			else if (line.find("summary") != string::npos) {

				//���̏����̏ꍇ�͂������������ł���̂ŏ����o���Ȃ�
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
					const char *ret = strrchr(str, c);//�Ō�́F�����̕�����


				
					
					
					string summary = string(ret);
					if (summary.find("lose") != string::npos) {
						gd.VorD = 1;//���̏���
					}
					else if (summary.find("win") != string::npos) {
						gd.VorD = 2;//���̏���
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

//�g����
void useage() {

	std::cerr << "���̃v���O������floodgate�`���̊������Z�I�̊w�K�`���ɕϊ����邽�߂̃v���O�����ł��B" << endl
		<< "���ƌ��̃��[�g�����܂��Ă���ꍇ�͂���炪3000�ȏ�łȂ���Εϊ����ď����o������͂��܂���" << endl
		<< "�g�p���@" << endl
		<< "fg2gdb [folder_name]" << endl
		<< "[foldername]�Ŏw�肳�ꂽ�t�H���_����*.csa�t�@�C�����ċA�I�ɒT���܂��B" << endl;

}


//--------------------------main�֐��I�I------------------------------------------------------
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
