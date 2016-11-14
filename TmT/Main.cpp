#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "CUESDK.h"
#include <vector>
#include <stdio.h>
#include<conio.h>

#include "Screen.h"

#include <future>

#include <vector>
#include <windows.h>

using namespace std;
using namespace fun;

map<char, int> win_map = {
	{ 'a', 65 },
	{ 'b', 66 },
	{ 'c', 67 },
	{ 'd', 68 },
	{ 'e', 69 },
	{ 'f', 70 },
	{ 'g', 71 },
	{ 'h', 72 },
	{ 'i', 73 },
	{ 'j', 74 },
	{ 'k', 75 },
	{ 'l', 76 },
	{ 'm', 77 },
	{ 'n', 78 },
	{ 'o', 79 },
	{ 'p', 80 },
	{ 'q', 81 },
	{ 'r', 82 },
	{ 's', 83 },
	{ 't', 84 },
	{ 'u', 85 },
	{ 'v', 86 },
	{ 'w', 87 },
	{ 'x', 88 },
	{ 'y', 89 },
	{ 'z', 90 },
	{ ',', CLK_CommaAndLessThan },
	{ '.', CLK_PeriodAndBiggerThan },
	{ '(', CLK_9 },
	{ ')', CLK_0 },
	{ '{', CLK_BracketLeft },
	{ '}', CLK_BracketRight },
	{ '\'',CLK_GraveAccentAndTilde },
	{ '\\',CLK_Backslash },
	{ '\"',CLK_ApostropheAndDoubleQuote },
	{ ' ', CLK_Space },
	{ '1', CLK_1 },
	{ '2', CLK_2 },
	{ '3', CLK_3 },
	{ '4', CLK_4 },
	{ '5', CLK_5 },
	{ '6', CLK_6 },
	{ '7', CLK_7 },
	{ '8', CLK_8 },
	{ '9', CLK_9 },
	{ '0', CLK_0 },
	{ '-', CLK_MinusAndUnderscore },
	{ '_', CLK_MinusAndUnderscore },
	{ '=', CLK_EqualsAndPlus },
	{ ':', CLK_SemicolonAndColon },
	{'+', 0xBB},
	{ '\n', CLK_Enter }
};

map<char, CorsairLedId> my_map = {
	{ 'a', CLK_A },
	{ 'b', CLK_B },
	{ 'c', CLK_C },
	{ 'd', CLK_D },
	{ 'e', CLK_E },
	{ 'f', CLK_F },
	{ 'g', CLK_G },
	{ 'h', CLK_H },
	{ 'i', CLK_I },
	{ 'j', CLK_J },
	{ 'k', CLK_K },
	{ 'l', CLK_L },
	{ 'm', CLK_M },
	{ 'n', CLK_N },
	{ 'o', CLK_O },
	{ 'p', CLK_P },
	{ 'q', CLK_Q },
	{ 'r', CLK_R },
	{ 's', CLK_S },
	{ 't', CLK_T },
	{ 'u', CLK_U },
	{ 'v', CLK_V },
	{ 'w', CLK_W },
	{ 'x', CLK_X },
	{ 'y', CLK_Y },
	{ 'z', CLK_Z },
	{ ',', CLK_CommaAndLessThan },
	{ '.', CLK_PeriodAndBiggerThan },
	{ '(', CLK_9 },
	{ ')', CLK_0 },
	{ '{', CLK_BracketLeft },
	{ '}', CLK_BracketRight },
	{ '\'',CLK_GraveAccentAndTilde },
	{ '\\',CLK_Backslash },
	{ '\"',CLK_ApostropheAndDoubleQuote },
	{ ' ', CLK_Space },
	{ '1', CLK_1 },
	{ '2', CLK_2 },
	{ '3', CLK_3 },
	{ '4', CLK_4 },
	{ '5', CLK_5 },
	{ '6', CLK_6 },
	{ '7', CLK_7 },
	{ '8', CLK_8 },
	{ '9', CLK_9 },
	{ '0', CLK_0 },
	{ '-', CLK_MinusAndUnderscore },
	{ '_', CLK_MinusAndUnderscore },
	{ '=', CLK_EqualsAndPlus },
	{ ':', CLK_SemicolonAndColon },
	{ '+', CLK_EqualsAndPlus },
	{ '\n', CLK_Enter }
};

const char* toString(CorsairError error)
{
	switch (error) {
	case CE_Success:
		return "CE_Success";
	case CE_ServerNotFound:
		return "CE_ServerNotFound";
	case CE_NoControl:
		return "CE_NoControl";
	case CE_ProtocolHandshakeMissing:
		return "CE_ProtocolHandshakeMissing";
	case CE_IncompatibleProtocol:
		return "CE_IncompatibleProtocol";
	case CE_InvalidArguments:
		return "CE_InvalidArguments";
	default:
		return "unknown error";
	}
}

int handshake()
{
	CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError()) {
		std::cout << "Handshake failed: " << toString(error) << std::endl;
		getchar();
		return -1;
	}
}

void performPulseEffect(std::vector<CorsairLedColor> &ledColorsVec)
{
	static auto waveDuration = 100; //500
	const auto timePerFrame = 10;
	for (auto x = .0; x < 2; x += static_cast<double>(timePerFrame) / waveDuration) {
		auto val = (1 - pow(x - 1, 2)) * 255;
		for (auto &ledColor : ledColorsVec)
			ledColor.g = val;
		CorsairSetLedsColorsAsync(ledColorsVec.size(), ledColorsVec.data(), nullptr, nullptr);

		if (GetAsyncKeyState(VK_OEM_PLUS) && waveDuration > 100)
			waveDuration -= 100;
		if (GetAsyncKeyState(VK_OEM_MINUS) && waveDuration < 2000)
			waveDuration += 100;
		std::this_thread::sleep_for(std::chrono::milliseconds(timePerFrame));
	}
}

//0 not pressed
//1 pressed
//-127 or -128 being hold
short correctKeyPressed(char c)
{
	for(int x = 32; x < 128; x++)
	{
		if (x == 36)
			continue;
		short m = GetKeyState(x);
		if (m == 1 && c == x)
			return 1;
		if (m == 1 && c != x)
		{
			cout << (char)x << ": " << (int)x << endl;
			return -1;
		}
	}
	return 0;
}

std::vector<CorsairLedColor> getAvailableKeys()
{
	auto colorsVector = std::vector<CorsairLedColor>();
	for (auto deviceIndex = 0; deviceIndex < CorsairGetDeviceCount(); deviceIndex++) {
		if (auto deviceInfo = CorsairGetDeviceInfo(deviceIndex)) {
			switch (deviceInfo->type) {
			case CDT_Mouse: {
				auto numberOfKeys = deviceInfo->physicalLayout - CPL_Zones1 + 1;
				for (auto i = 0; i < numberOfKeys; i++) {
					auto ledId = static_cast<CorsairLedId>(CLM_1 + i);
					colorsVector.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
				}
			} break;
			case CDT_Keyboard: {
				auto ledPositions = CorsairGetLedPositions();
				if (ledPositions) {
					for (auto i = 0; i < ledPositions->numberOfLed; i++) {
						auto ledId = ledPositions->pLedPosition[i].ledId;
						colorsVector.push_back(CorsairLedColor{ ledId, 0, 0, 0 });
					}
				}
			} break;
			case CDT_Headset: {
				colorsVector.push_back(CorsairLedColor{ CLH_LeftLogo, 0, 0, 0 });
				colorsVector.push_back(CorsairLedColor{ CLH_RightLogo, 0, 0, 0 });
			} break;
			}
		}
	}
	return colorsVector;
}

void changeColor(int color)
{	//14 default
	//78 color
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// you can loop k higher to see more color choices
	SetConsoleTextAttribute(hConsole, color);
}

void ScoreBoard(COORD prev, int score, int errors)
{
	COORD loc;
	loc.X = 33;
	loc.Y = 0;
	setCursorPosition(loc);

	cout << "Score:  " << score;
	loc.Y++;
	setCursorPosition(loc);
	cout << "Errors: " << errors;

	//Set back to previous location
	setCursorPosition(prev);
}

int main()
{
	changeColor(78);
	if (handshake() == -1)
	{
		return -1; // Some error
	}

	int score = 0;
	//Add time variable.
	int errorCount = 0;
	auto ledColor = CorsairLedColor();
	string line;
	ifstream myfile("typinglesson.txt");
	int lineY = 1;
	int lineX = 0;

	if (myfile.is_open())
	{
		cout << "\t Teach Me Typing \t" << endl;

		while (getline(myfile, line))
		{
			//Print Score
			ScoreBoard(getCursorPosition(), score, errorCount);


			//Print line
			cout << line << endl;

			COORD loc;
			loc.X = lineX;
			loc.Y = lineY;

			for (char c : line)
			{
				loc.X = lineX;
				changeColor(14);
				setCursorPosition(loc);
				cout << c;
				setCursorPosition(loc);
				changeColor(78);

				//highlight character to press
				COORD loc = getCursorPosition();
				vector<CorsairLedColor> vec;
				char cc = c;
				if (c >= 65 && c <= 90)
					cc = c + 32;
				switch (c)
				{
				case ' ':
					ledColor.ledId = my_map[' '];
					break;
				case '\n':
					ledColor.ledId = my_map['\n'];
					break;
				default:
					ledColor.ledId = my_map[cc];
				}

				ledColor.g = 255;
				vec.push_back(ledColor);
				bool okay = CorsairSetLedsColors(vec.size(), vec.data());
				//performPulseEffect(vec);
				vec.clear();
				//User input
				while (true) 
				{
					char presssed = _getch();

					if (presssed >= 65 && presssed <= 90)
						presssed = presssed + 32;


					if (presssed == cc)
					{
						score++;
						break;
					}
					else 
					{
						errorCount++;
						//Pulse Red
						ledColor.r = 255;
						ledColor.g = 0;
						ledColor.b = 0;
						vec.push_back(ledColor);
						//CorsairSetLedsColorsAsync(vec.size(), vec.data(), nullptr, nullptr);
						performPulseEffect(vec);
					}
					//Print Score
					ScoreBoard(getCursorPosition(), score, errorCount);
				}
				//Print Score
				ScoreBoard(getCursorPosition(), score, errorCount);

				//Reset
				vec.clear();
				ledColor.g = 0;
				ledColor.r = 0;
				ledColor.b = 0;
				vec.push_back(ledColor);
				okay = CorsairSetLedsColors(vec.size(), vec.data());
				if (okay == false)
					cout << "Pooped! Error in setting colors!" << endl;
				lineX++;
			}
			lineX = 0;
			lineY++;
			cout << endl;
		}
		myfile.close();

		cls();
		cout << "You achieved Grandma level \nWith the following result:" << endl;
		cout << "Score: " << score << endl;
		cout << "Errors: " << errorCount << endl;
				

		auto colorsVector = getAvailableKeys();
		if (!colorsVector.empty()) {
			//std::cout << "Working... Use \"+\" or \"-\" to increase or decrease speed.\nPress Escape to close program...";
			while (!GetAsyncKeyState(VK_ESCAPE)) {
				performPulseEffect(colorsVector);
				std::this_thread::sleep_for(std::chrono::milliseconds(25));
			}
		}
	}

	else cout << "Unable to open file";

	system("pause");
	return 0;
}