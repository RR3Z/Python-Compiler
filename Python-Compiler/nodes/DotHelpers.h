#pragma once
#include <string>

using namespace std;

string dotConnection(int firstID, int secondID);

string dotLabel(int ID, string text);

string dotConnectionWithLabel(int firstID, int secondID, string text);