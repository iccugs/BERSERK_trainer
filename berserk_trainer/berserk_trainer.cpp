#include "stdafx.h"

#include <iostream>
#include <vector>
#include <Windows.h>

#include "proc.h"
#include "mem.h"
#include "ascii.h"

bool bHealth = false, bFrenzy = false, bDBlow = false, bGold = false, bMenu = true;

void Menu()
{

	if (bMenu)
	{
		std::cout << "HOTKEY = FUNCTIONS = STATUS\n";
		std::cout << "===========================\n\n";
		std::cout << "NUM0 = Infinite Health = " << bHealth << "\n";
		std::cout << "NUM1 = Max Frenzy = " << bFrenzy << "\n";
		std::cout << "NUM2 = Max Death Blow = " << bDBlow << "\n\n";
		//std::cout << "HOME = Character Info" << "\n";
		std::cout << "DELETE = EXIT TRAINER\n";
	}
	//if (!bMenu)
	//{
		//std::cout << "       CHARACTER INFO\n";
		//std::cout << "===========================\n\n";
		//std::cout << "HEALTH: " << healthAddr << "/" << maxhealthAddr << "\n";
		//std::cout << "FRENZY: " << frenzyAddr << "/ 1000" << "\n";
		//std::cout << "DEATH BLOW: " << dblowAddr << "/ 1000" << "\n";
		//std::cout << "GOLD: " << goldAddr << "\n\n";
		//std::cout << "HOME = Hack Info" << "\n";
		//std::cout << "DELETE = EXIT TRAINER\n";
	//}
}


int main()
{
	HANDLE hProcess = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12);
	uintptr_t moduleBase = 0;
	uintptr_t playerPtr = 0;
	uintptr_t healthAddr = 0;
	uintptr_t maxhealthAddr = 0;
	uintptr_t frenzyAddr = 0;
	uintptr_t dblowAddr = 0;
	//uintptr_t goldPtr = 0, goldAddr = 0;
	const float maxFrenzy = 1000, maxDblow = 1000;
	const int maxHealth = 99999, maxGold = 9999999;
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
		uintptr_t playerPtr = moduleBase + 0x7F0DD0;
		uintptr_t healthAddr = FindDMAAddy(hProcess, playerPtr, { 0x20,0xC54 });
		uintptr_t maxhealthAddr = FindDMAAddy(hProcess, playerPtr, { 0x20,0xC50 });
		uintptr_t frenzyAddr = FindDMAAddy(hProcess, playerPtr, { 0x20,0xC74 });
		uintptr_t dblowAddr = FindDMAAddy(hProcess, playerPtr, { 0x20,0xE04 });

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

		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bDBlow = !bDBlow;

			ClearScreen();
			Menu();
		}
		if (bDBlow)
		{
			mem::PatchEx((BYTE*)dblowAddr, (BYTE*)&maxDblow, sizeof(maxDblow), hProcess);
		}

		if (GetAsyncKeyState(VK_HOME) & 1)
		{
			bMenu = !bMenu;

			ClearScreen();
			Menu();
		}

		if (GetAsyncKeyState(VK_DELETE) & 1)
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