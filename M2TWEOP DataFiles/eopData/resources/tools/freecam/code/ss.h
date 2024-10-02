#pragma once
#include <string>
#include <list>
#include <iostream>
#include <fstream>
using namespace std;

//String stuff (ss) I am aware that the name needs replacing.
namespace SS
{
    string removeCharacters(string str, char c)
    {
        string result;
        for (size_t i = 0; i < str.size(); i++)
        {
            char currentChar = str[i];
            if (currentChar != c)
                result += currentChar;
        }
        return result;
    }

    string getLineWith(list<string> text, string line)
    {
        for (string str : text)
            if (str.find(line) != std::string::npos)
                return str;
        return "";
    }

    int find(string text, string marker)    // Made due to some confusion :/
    {
        for (int i = 0; i < text.length(); i++)
        {
            bool full_match = true;
            if (text.at(i) == marker.at(0))
            {
                for (int j = 1; j < marker.length(); j++)
                    if (text.at(i + j) != marker.at(j))
                    {
                        full_match = false;
                        break;
                    }
            }
            else
                full_match = false;

            if (full_match)
                return i;
        }
        return -1;
    }

    string getRightOfReg(string line, string seperator)
    {
        string out = std::regex_replace(line, std::regex(".*"+seperator), "");
        return line.substr(find(line, seperator) + seperator.length(), line.size());
    }

    string getRightOf(string line, string seperator)
    {
        return line.substr(find(line, seperator) + seperator.length(), line.size());
    }

    string getLeftOf(string line, string seperator)
    {
        return line.substr(0, find(line, seperator));
    }

    string readFile(string f)
    {
        ifstream file;
        file.open(f);

        if (file.is_open())
        {
            string line;
            string text;
            getline(file, text);
            while (getline(file, line))
                text += "\n" + line;

            return text;
        }
        else
        {
            std::cerr << "Failed to read file " + f + "\n";
            throw std::runtime_error("Failed to open file " + f);
            Sleep(6000);
            exit(0);
        }
    }

    float percentageToFloat(string in)
    {
        float out = -1.0f;
        if (find(in, "%") != -1)
            out = stof(std::regex_replace(in, std::regex("\%"), "")) / 100.0f;
        else
            out = stof(in);

        return out;
    }

    int count(string text, char symbol)
    {
        return (int)(std::count(text.begin(), text.end(), symbol));
    }

    string* split(string in, char seperator)
    {
        string* out = new string[count(in, seperator) + 1];
        int i = 0, s = 0;
        string comp = in + "";
        while (i < count(in, seperator) + 1)
        {
            out[i++] = comp.substr(0, comp.find_first_of(seperator));
            comp = comp.substr(comp.find_first_of(seperator) + 1, comp.length());
        }
        return out;
    }
}