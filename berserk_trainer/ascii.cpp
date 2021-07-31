#include "stdafx.h"
#include "ascii.h"

void AsciiArt()
{
	std::cout << "  ____                          _\n";
	std::cout << " | __ )  ___ _ __ ___  ___ _ __| | __\n";
	std::cout << " |  _ \\ / _ \\ '__/ __|/ _ \\ '__| |/ /\n";
	std::cout << " | |_) |  __/ |  \\__ \\  __/ |  |   <\n";
	std::cout << " |____/ \\___|_|  |___/\\___|_|  |_|\\_\\\n";
	std::cout << " |_   _| __ __ _(_)_ __   ___ _ __\n";
	std::cout << "   | || '__/ _` | | '_ \\ / _ \\ '__|\n";
	std::cout << "   | || | | (_| | | | | |  __/ |\n";
	std::cout << "   |_||_|  \\__,_|_|_| |_|\\___|_|\n\n";
	std::cout << "\t\t\t By: Paradox\n\n";
	std::cout << "press enter to continue...\n";
}

void ClearScreen()
{
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count, cellCount;
	COORD homeCoords = { 0,0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE)return;
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))return;
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR)' ',
		cellCount,
		homeCoords,
		&count
	))return;

	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
	))return;

	SetConsoleCursorPosition(hStdOut, homeCoords);
}