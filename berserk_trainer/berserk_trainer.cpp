#include "stdafx.h"

#include <iostream>
#include <vector>
#include <Windows.h>

#include "proc.h"
#include "mem.h"
#include "ascii.h"

bool bHealth = false, bFrenzy = false, bDBlow = false;

void Menu()
{
	std::cout << "HOTKEY = FUNCTIONS = STATUS\n";
	std::cout << "===========================\n\n";
	std::cout << "NUM0 = Infinite Health = " << bHealth << "\n";
	std::cout << "NUM1 = Max Frenzy = " << bFrenzy << "\n";
	std::cout << "NUM2 = Max Death Blow = " << bDBlow << "\n\n";
	std::cout << "INSERT = EXIT TRAINER\n";
}

int main()
{
	HANDLE hProcess = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	uintptr_t moduleBase = 0, playerPtr = 0, healthAddr = 0, frenzyAddr = 0, dblowAddr = 0;
	const float maxFrenzy = 1000, maxDblow = 1000;
	const int maxHealth = 99999;
	DWORD procId = GetProcId(L"BERSERK.exe");

	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		moduleBase = GetModuleBaseAddress(procId, L"BERSERK.exe");
		AsciiArt();
		getchar();
		ClearScreen();
		Menu();
	}

	else
	{
		std::cout << "Berserk process not found, press enter to exit\n";
		getchar();
		return(0);
	}

	DWORD dwExit = 0;

	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		playerPtr = moduleBase + 0x7F0DD0;
		healthAddr = FindDMAAddy(hProcess, playerPtr, { 0x20,0xC54 });
		frenzyAddr = FindDMAAddy(hProcess, playerPtr, { 0x20,0xC74 });
		dblowAddr = FindDMAAddy(hProcess, playerPtr, { 0x20,0xE04 });

		if (GetAsyncKeyState(VK_NUMPAD0) & 1)
		{
			bHealth = !bHealth;

			ClearScreen();
			Menu();
		}
		if (bHealth)
		{
			mem::PatchEx((BYTE*)healthAddr, (BYTE*)&maxHealth, sizeof(maxHealth), hProcess);
		}

		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bFrenzy = !bFrenzy;

			ClearScreen();
			Menu();
		}
		if (bFrenzy)
		{
			mem::PatchEx((BYTE*)frenzyAddr, (BYTE*)&maxFrenzy, sizeof(maxFrenzy), hProcess);
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bDBlow = !bDBlow;

			ClearScreen();
			Menu();
		}
		if (bDBlow)
		{
			mem::PatchEx((BYTE*)dblowAddr, (BYTE*)&maxDblow, sizeof(maxDblow), hProcess);
		}

		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			return(0);
		}

		Sleep(100);
	}

	ClearScreen();
	std::cout << "Berserk process not found, press enter to exit\n";
	getchar();
	return(0);
}