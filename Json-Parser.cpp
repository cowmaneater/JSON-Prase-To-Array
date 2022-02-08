// JsonParse_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Objective:
// {
// }
// ... and ...
//{
//	"Name": "BitCoin",
//	"Symbol" : "BTC",
//}
//
//

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>
#include <stdio.h>
#include <ctype.h>
using namespace std;

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

const int ARRSIZE = 5;
const string DATA_FILE_PATH = "TestData\\";
////////////////////////////
class JsonParser
{
	// Work here
	//declaring two string arrrays and declaring them to be the array size constant
public:
	string namesA [ARRSIZE ];
	string valuesA [ARRSIZE];
	string array_values[ARRSIZE];

	

public:
	// Open File function to open file and then take the text contents of the JSON file into the filestream, 
	//had to use the stream buffer iterator in order to read the entire contents from the filestream into the ifstreamInput varible
	// attempted to just use the fstreamInput << ifs but would only return the first character
	//the fstreamInput will allow me to then with the parse function to read the string and parse at a later point
	//
	bool OpenFile(string filename) {

		ifstream ifs(filename);
		//open file return true/false
		if (ifs.is_open()) {
			//
			fstreamInput = string((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());


			return true;
		}
		else {
			return false;
		}


	}
	bool ParseJson() {
		RemoveChars(fstreamInput);
		int i = 0;
		//int a is for the second while statement for the array parsing
		int a = 0;
		string name;
		string value;
		//array value for second parsing, mimicing the value string 
		string arrayValue;


		//puts holder into a string stream to be broken up into respective arrays
		//not sure how to limit this further than by array size. EG if there is only 2 array members but array intilizied to 50
		stringstream JSONLINE(fstreamHolder);
		while ( i != ARRSIZE) {
			


			getline(JSONLINE, name, '\n');
			getline(JSONLINE, value, '\n');

			

			namesA[i] = name;
			//calling of the validator, invalid doesn't get added to value array
			if (validateValue(value) == true) {
				valuesA[i] = value;
			}

			i++;

			


		}
		//I am just mimicing the last while statment for array parsing, this time its array value break array value break
		stringstream JSONLINE2(ArrayHolder);
		
		while (a != ARRSIZE) {



			getline(JSONLINE2, arrayValue, '\n');




			//the values for the array are typically gonna be numbers so I should validate it the same way I do the values
			//calling of the validator, invalid doesn't get added to value array
			if (validateValue(arrayValue) == true) {
				array_values[a] = arrayValue;
			}

			a++;




		}
		//If operations were done correctly on fstreamInput, fstreamHolder shouldn't be the same, all JSON should have some whitespace and brackets
		//dont' think I need to include another one for the arrays, this shoulds still prove that it isn't the same
		if (fstreamInput != fstreamHolder) {
			return true;
		}
		else {
			return false;
		}






		
	}
	//this is the function to validate JSON numbers, out of the examples I have however they appear to all be valid so no failure should happen
	bool validateValue(string value) {
		for (int i = 0; i < value.size(); i++) {
			//checks if the next character after the decimil point is a digit, if not it fails (also i'm not sure if the ! is the right format
			if (value[i] == '.' && !isdigit(value[i + 1])) {
				return false;
			}
			//returns false if after e acommpanied by plostive or negative doesn't have a digit after it
			else if ((value[i] == 'e' || value[i] == 'E') && (value[i + 1] == '+' || value[i + 1] == '-') && !isdigit(value[i + 2])) {
				return false;
			}
			//returns false if after e there is not positive or negative sign when accompanied by a digit
			else if ((value[i] == 'e' || value[i] == 'E') && isdigit(value[i+2]) && (value[i+1] != '-' && value[i+1] != '+')) {
				return false;
			}
		
		
		
		}
		//this returns false on leading zero
		if (value[0] == '0' && isdigit(value[1])) {

			return false;
			}
		//returns false if there is a leading zero without a decimal point but with a negative sign
		else if (value[0] == '-' && value[1] =='0' && value[2] != '.') {
			return false;
		}
		else {
			return true;
		}

	}
	//this is function that will remove characters from the string in the parameter, so in this case I wanna remove the white space, quotes, and {} in order to parse the data easier

	void RemoveChars(string removeIT) {

		for (int i = 0; i < removeIT.size(); i++) {
			//i put this in here to replace commas with a line break in order to parse easier down the road. It will always look like name : value break name : value

			//trying to have a seprate nested if loop for the array, with its own iteration but also iterates main loop, breaks out of it with ] character
			//
			if (removeIT[i] == '[') {
				for (int a = 0; a < removeIT.size(); a++) {
					if (removeIT[i]==']') {
						i++;
						break;

					}
					else if (removeIT[i] == ',') {
						ArrayHolder += '\n';
						i++;

					}
					//this tests if there is a space and before the space there is an alpha numeric key that isn't a line break to enter in that space eg. Market cap instead of MarketCap
					else if (removeIT[i] == ' ' && removeIT[i - 1] != '\n' && isalnum(removeIT[i - 1])) {
						ArrayHolder += removeIT[i];
						i++;
					}
					//had to include the [ character in this but otherwise jsut mimicing the outer loop
					else if (removeIT[i] != '['&& removeIT[i] != ' ' && removeIT[i] != '{' && removeIT[i] != '"' && removeIT[i] != '}' && removeIT[i] != '\n') {

						ArrayHolder += removeIT[i];
						i++;
					}
					else {
						//forgot to add the last i interator
						i++;
					}

				}


			}

			else if (removeIT[i] == ',') {
				fstreamHolder += '\n';

			}
			//this was placed here after the above edit so the order is now name break value break name break value
			else if (removeIT[i] == ':') {
				fstreamHolder += '\n';

			}
			//this tests if there is a space and before the space there is an alpha numeric key that isn't a line break to enter in that space eg. Market cap instead of MarketCap
			else if (removeIT[i] == ' ' && removeIT[i-1] != '\n' && isalnum(removeIT[i - 1])) {
				fstreamHolder += removeIT[i];
			}
			else if (removeIT[i] != ' ' && removeIT[i] != '{' && removeIT[i] != '"'  && removeIT[i] != '}' && removeIT[i] != '\n') {
				////nested if to check if white space has a character before it, if so don't delete it
				//if (removeIT[i] == ' '&& isalpha(removeIT[i-1]))
				//{
				//	
				//}
				//else {

				//}
				//removed this as it wasn't working
				fstreamHolder += removeIT[i];
			}

			else {

			}

		}



	}

private:
	string fstreamInput;
	string fstreamHolder;
	string ArrayHolder;


};
///////////////// Testing ////////////////////////////////
TEST_CASE("Testing my JSON Parser")
{
	JsonParser jp;


	SUBCASE("testing parsing of empty JSON object")
	{
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject1.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.namesA[0] == "");
		CHECK(jp.valuesA[0] == "");
	}
	SUBCASE("testing parsing of simple JSON object (only strings for values)")
	{
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject2.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		// Validate the parsed JSON object
		CHECK(jp.namesA[0] == "Name");
		CHECK(jp.namesA[1] == "Symbol");
		CHECK(jp.valuesA[0] == "BitCoin");
		CHECK(jp.valuesA[1] == "BTC");
	}
	SUBCASE("Testing parsing of simple JSON object (strings and numbers for values)")
	{
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject3a.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		//validate the parsed JSON object
		CHECK(jp.namesA[0] == "Rank");
		CHECK(jp.namesA[1] == "Name");
		CHECK(jp.namesA[2] == "Symbol");
		CHECK(jp.namesA[3] == "Market Cap");
		CHECK(jp.valuesA[0] == "3");
		CHECK(jp.valuesA[1] == "Binance Coin");
		CHECK(jp.valuesA[2] == "BNB");
		CHECK(jp.valuesA[3] == "90321378765");

	}
	SUBCASE("Testing parsing of simple JSON object (strings and numbers for values)")
	{
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject3b.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		//validate the parsed JSON object
		CHECK(jp.namesA[0] == "Rank");
		CHECK(jp.namesA[1] == "Name");
		CHECK(jp.namesA[2] == "Symbol");
		CHECK(jp.namesA[3] == "Market Cap");
		CHECK(jp.valuesA[0] == "21");
		CHECK(jp.valuesA[1] == "Algorand");
		CHECK(jp.valuesA[2] == "ALGO");
		CHECK(jp.valuesA[3] == "10198650050.12345");

	}
	SUBCASE("Testing parsing of simple JSON object (strings and negative numbers with decmials for values)")
	{
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject3c.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		//validate the parsed JSON object
		CHECK(jp.namesA[0] == "Rank");
		CHECK(jp.namesA[1] == "Name");
		CHECK(jp.namesA[2] == "Symbol");
		CHECK(jp.namesA[3] == "Market Cap");
		CHECK(jp.valuesA[0] == "0");
		CHECK(jp.valuesA[1] == "111 Test this 111");
		CHECK(jp.valuesA[2] == "TEST");
		CHECK(jp.valuesA[3] == "-12345.67890e-789");

	}
	//updating git, having trouble merging
	//subcases for array testing
	SUBCASE("Testing parsing of simple JSON object (strings and empty array)")
	{
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject4a.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		//validate the parsed JSON object
		CHECK(jp.namesA[0] == "Rank");
		CHECK(jp.namesA[1] == "Name");
		CHECK(jp.namesA[2] == "Symbol");
		CHECK(jp.namesA[3] == "Market Cap");
		CHECK(jp.valuesA[0] == "1");
		CHECK(jp.valuesA[1] == "Bitcoin");
		CHECK(jp.valuesA[2] == "BTC");
		CHECK(jp.array_values[0] == "");

	}
	SUBCASE("Testing parsing of simple JSON object (strings and array with strings)")
	{
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject4b.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		//validate the parsed JSON object
		CHECK(jp.namesA[0] == "Rank");
		CHECK(jp.namesA[1] == "Name");
		CHECK(jp.namesA[2] == "Symbol");
		CHECK(jp.namesA[3] == "Market Cap");
		CHECK(jp.valuesA[0] == "21");
		CHECK(jp.valuesA[1] == "Algorand");
		CHECK(jp.valuesA[2] == "ALGO");
		//this was orignial one thosand two thosand and three thosand, not sure if you wanted me to convert this or it was a mistake
		CHECK(jp.array_values[0] == "1000");
		CHECK(jp.array_values[1] == "2000");
		CHECK(jp.array_values[2] == "3000");

	}
	SUBCASE("Testing parsing of simple JSON object (strings and array with numbers)")
	{
		// Open the file
		CHECK(jp.OpenFile(DATA_FILE_PATH + "JsonObject4c.json") == true);

		// Invoke the parsing process
		CHECK(jp.ParseJson() == true);

		//validate the parsed JSON object
		CHECK(jp.namesA[0] == "Rank");
		CHECK(jp.namesA[1] == "Name");
		CHECK(jp.namesA[2] == "Symbol");
		CHECK(jp.namesA[3] == "Market Cap");
		CHECK(jp.valuesA[0] == "1");
		CHECK(jp.valuesA[1] == "Bitcoin");
		CHECK(jp.valuesA[2] == "BTC");
		CHECK(jp.array_values[0] == "1000");
		CHECK(jp.array_values[1] == "0");
		CHECK(jp.array_values[2] == "-12345.67890e-789");

	}
	//merge 2 complete

}
