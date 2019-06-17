#include <iostream>
#include "Json.h"
#include "JsonText.h"
#include "JsonPath.h"
#include "TrivialValues.h"
#include <fstream>
using std::cin;
using std::cout;
using std::endl;

void executeCommand(const char* cmd, char* buffer);
void printHelp();

int main()
{
	char command[50];
	char buffer[1024];
	Json& json = Json::getJson();
	printHelp();

	while (true)
	{
		cin >> command;
		if (strcmp(command, "exit") == 0)
			break;
		else if (strcmp(command, "print") == 0)
		{
			json.printCommand();
			cin.ignore();
		}
		else if (strcmp(command, "items_print") == 0)
		{
			json.printItemsCommand();
			cin.ignore();
		}
		else if (strcmp(command, "help") == 0)
		{
			printHelp();
			cin.ignore();
		}
		else
		{
			cin.ignore();
			cin.getline(buffer, 1024);
			executeCommand(command, buffer);
		}
	}
	system("PAUSE");
	return 0;
}

void executeCommand(const char* cmd, char* buffer)
{
	Json& json = Json::getJson();
	if (strcmp(cmd, "validate") == 0)
	{
		json.validateCommand(buffer);
	}
	else if (strcmp(cmd, "load") == 0)
	{
		json.loadCommand(buffer);
	}
	else if (strcmp(cmd, "items_load") == 0)
	{
		json.loadItemsCommand(buffer);
	}
	else if (strcmp(cmd, "item_print") == 0)
	{
		json.printSingleItemCommand(buffer);
	}
	else if (strcmp(cmd, "save") == 0)
	{
		json.saveCommand(buffer, false, false);
	}
	else if (strcmp(cmd, "save_min") == 0)
	{
		json.saveCommand(buffer, true, false);
	}
	else if (strcmp(cmd, "save_excerpt") == 0)
	{
		json.saveCommand(buffer, false, true);
	}
	else if (strcmp(cmd, "save_excerpt_min") == 0)
	{
		json.saveCommand(buffer, true, true);
	}
	else if (strcmp(cmd, "items_save") == 0)
	{
		json.saveItemsCommand(buffer, false);
	}
	else if (strcmp(cmd, "items_save_min") == 0)
	{
		json.saveItemsCommand(buffer, true);
	}
	else if (strcmp(cmd, "item_save") == 0)
	{
		json.saveSingleItemCommand(buffer, false);
	}
	else if (strcmp(cmd, "item_save_min") == 0)
	{
		json.saveSingleItemCommand(buffer, true);
	}
	else if (strcmp(cmd, "replace") == 0)
	{
		json.replaceCommand(buffer);
	}
	else if (strcmp(cmd, "swap") == 0)
	{
		json.swapCommand(buffer);
	}
	else if (strcmp(cmd, "delete") == 0)
	{
		json.deleteCommand(buffer);
	}
	else if (strcmp(cmd, "add") == 0)
	{
		json.addCommand(buffer);
	}
	else
		cout << "Invalid command." << endl;
}

void printHelp()
{
	cout << "-------------------------------------------------------------------------" << endl;
	cout << "validate <filename>" << endl;
	cout << "load <filename>" << endl;
	cout << "items_load <key>" << endl;
	cout << "print" << endl;
	cout << "items_print" << endl;
	cout << "item_print <index>" << endl;
	cout << "save <filename>" << endl;
	cout << "save_min <filename>" << endl;
	cout << "save_excerpt <path> <filename>" << endl;
	cout << "save_excerpt_min <path> <filename>" << endl;
	cout << "items_save <filename>" << endl;
	cout << "items_save_min <filename>" << endl;
	cout << "item_save <index> <filename>" << endl;
	cout << "item_save_min <index> <filename>" << endl;
	cout << "replace <path> <value>" << endl;
	cout << "swap <path1> <path2>" << endl;
	cout << "delete <path>" << endl;
	cout << "add <path> <value>" << endl;
	cout << "help" << endl;
	cout << "exit" << endl;
	cout << "-------------------------------------------------------------------------" << endl;
}