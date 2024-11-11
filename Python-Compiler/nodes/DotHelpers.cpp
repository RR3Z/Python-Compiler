#include "DotHelpers.h"

string dotConnection(int firstID, int secondID) {
	return to_string(firstID) + "->" + to_string(secondID) + "\n";
}

string dotLabel(int ID, string text) {
	return to_string(ID) + " [label=\"" + text + "\"]" + "\n";
}

string dotConnectionWithLabel(int firstID, int secondID, string text) {
	return to_string(firstID) + "->" + to_string(secondID) + " [label=\"" + text + "\"]" + "\n";
}