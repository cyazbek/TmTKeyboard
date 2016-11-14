// progress.cpp : Defines the entry point for the console application.
//

#include "CUESDK.h"

#include <iostream>
#include <algorithm>
#include <thread>
#include <future>

#include <vector>
#include <windows.h>


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


const char* toString(CorsairError error) 
{
	switch (error) {
	case CE_Success : 
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

double getKeyboardWidth(CorsairLedPositions *ledPositions)
{
	const auto minmaxLeds = std::minmax_element(ledPositions->pLedPosition, ledPositions->pLedPosition + ledPositions->numberOfLed,
		[](const CorsairLedPosition &clp1, const CorsairLedPosition &clp2) {
		return clp1.left < clp2.left;
	});
	return minmaxLeds.second->left + minmaxLeds.second->width - minmaxLeds.first->left;
}

int main()
{
	CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError()) {
		std::cout << "Handshake failed: " << toString(error) << std::endl;
		getchar();
		return -1;
	}

	const auto ledPositions = CorsairGetLedPositions();
	if (ledPositions && ledPositions->numberOfLed > 0) {
				
		const auto keyboardWidth = getKeyboardWidth(ledPositions);
		const auto numberOfSteps = 50;
		std::cout << "Working... Press Escape to close program...";
		for (auto n = 51; !GetAsyncKeyState(VK_ESCAPE); n--) {

			if (n == 0)
				n = 51;

			std::vector<CorsairLedColor> vec;

			CorsairLedId poop[5][21] = { {CLK_GraveAccentAndTilde,	CLK_1,			CLK_2,			CLK_3,			CLK_4,			CLK_5,			CLK_6,		CLK_7,			CLK_8,				CLK_9,					CLK_0,						CLK_MinusAndUnderscore,			CLK_EqualsAndPlus,		CLK_Backspace,		CLK_Insert,		CLK_Home,		CLK_PageUp,		CLK_NumLock,		CLK_KeypadSlash,		CLK_KeypadAsterisk,			CLK_KeypadMinus},
										{CLK_Tab,					CLK_Q,			CLK_W,			CLK_E,			CLK_R,			CLK_T,			CLK_Y,		CLK_U,			CLK_I,				CLK_O,					CLK_P,						CLK_BracketLeft,				CLK_BracketRight,		CLK_Backslash,		CLK_Delete,		CLK_End,		CLK_PageDown,	CLK_Keypad7,		CLK_Keypad8,			CLK_Keypad9,				CLK_KeypadPlus},
										{CLK_CapsLock,				CLK_A,			CLK_S,			CLK_D,			CLK_F,			CLK_G,			CLK_H,		CLK_J,			CLK_K,				CLK_L,					CLK_SemicolonAndColon,		CLK_ApostropheAndDoubleQuote,	CLK_Enter,				CLI_Invalid,		CLI_Invalid,	CLI_Invalid,	CLI_Invalid,	CLK_Keypad4,		CLK_Keypad5,			CLK_Keypad6,				CLI_Invalid},
										{CLK_LeftShift,				CLI_Invalid,	CLK_Z,			CLK_X,			CLK_C,			CLK_V,			CLK_B,		CLK_N,			CLK_M,				CLK_CommaAndLessThan,	CLK_PeriodAndBiggerThan,	CLK_SlashAndQuestionMark,		CLK_RightShift,			CLI_Invalid,		CLI_Invalid,	CLK_UpArrow,	CLI_Invalid,	CLK_Keypad1,		CLK_Keypad2,			CLK_Keypad3,				CLK_KeypadEnter},
										{CLK_LeftCtrl,				CLK_LeftGui,	CLK_LeftAlt,	CLI_Invalid,	CLI_Invalid,	CLI_Invalid,	CLK_Space,	CLI_Invalid,	CLI_Invalid,		CLI_Invalid,			CLK_RightAlt,				CLK_RightGui,					CLK_Application,		CLK_RightCtrl,		CLK_LeftArrow,	CLK_DownArrow,	CLK_RightArrow,	CLK_Keypad0,		CLI_Invalid,			CLK_KeypadPeriodAndDelete,	CLI_Invalid}
									   };
				 
			auto ledColor = CorsairLedColor();
 			ledColor.ledId = poop[4][11];

			//Reset 
			for (int i = 0; i < 5; ++i)
			{
				for (int z = 0; z < 21; ++z)
				{
					std::cout << z << " " << i << std::endl;
					if (poop[i][z] == CLI_Invalid)
						continue;
					 
					
					ledColor.ledId = poop[i][z];
					ledColor.r = 255;
					ledColor.g = 255;
					ledColor.b = 24;
					vec.push_back(ledColor);
					CorsairSetLedsColors(vec.size(), vec.data()); 

					
				}
			}

			vec.clear();
			
			for (int z = 21; z >= 0 ; --z)
			{
				for (int i = 0; i < 5; ++i)
				{
					std::cout << z << " " << i << std::endl;
					if (poop[i][z] == CLI_Invalid)
						continue;

					ledColor.ledId = poop[i][z];
					ledColor.g = 255;
					vec.push_back(ledColor);
					CorsairSetLedsColors(vec.size(), vec.data());

				}
			}


			std::cout << "ENDED" << std::endl;

			

			

			std::this_thread::sleep_for(std::chrono::milliseconds(25));
		}
	}
	return 0;
}

