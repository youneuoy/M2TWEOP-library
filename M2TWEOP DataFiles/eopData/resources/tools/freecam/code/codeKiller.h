#pragma once
#include <iostream>
#include <regex>
#include "ss.h"
#include "AssemblyLine.h"


class CodeKiller
{
	AssemblyLine* code;
	int length;

public:

	CodeKiller() {};
	CodeKiller(std::string contents)
	{
		try
		{
			std::string fileContents = contents;
			fileContents = std::regex_replace(fileContents, std::regex("#.*"), "");
			fileContents = std::regex_replace(fileContents, std::regex("\n+"), "\n");
			fileContents = std::regex_replace(fileContents, std::regex("^\n+"), "");
			fileContents = std::regex_replace(fileContents, std::regex("\n+$"), "");
			fileContents = std::regex_replace(fileContents, std::regex(" +"), "");
			fileContents = std::regex_replace(fileContents, std::regex("\t+"), "");
			std::string* s = SS::split(fileContents, '\n');

			length = SS::count(fileContents, '\n') + 1;
			code = new AssemblyLine[length];
			for (int i = 0; i < length; i++)
				code[i] = AssemblyLine((LPVOID)std::stoul(s[i], nullptr, 16));

		}
		catch (...)
		{
			std::cerr <<"Failed to generate code killer " + contents + "\n";
			throw std::runtime_error("Failed to generate code killer " + contents);
			Sleep(6000);
			exit(0);
		}
	}

	void kill()
	{
		for (int i = 0; i < length; i++)
			code[i].nop();
	}

	void resurrect()
	{
		for (int i = 0; i < length; i++)
			code[i].reset();
	}
};