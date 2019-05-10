#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

vector<string> split(const string& text, bool flag = false)			// x - word sequence number, flag - character switch
{
	char ch = ' ';
	if (flag) ch = '\t';

	vector<string> words_in_text;

	for (int i = 0; i < text.size() - 1; i++)
	{
		if (i == 0 || (text[i] == ch && text[i + 1] != ch))
		{
			int j;
			int substr_size = 0;
			for (j = i + 1; j < text.size(); j++)
			{
				if (text[j] != ch)
					substr_size++;
				else
					break;
			}
			if (i == 0) words_in_text.push_back(text.substr(i, substr_size + 1));
			else words_in_text.push_back(text.substr(i + 1, substr_size));
			i = j - 1;
		}
	}
	return words_in_text;
}

struct FlightDetails
{
	FlightDetails(const string& track_id = "", const string& destination = "", const string& departure = "", int flight_al = 0)
		: TRACK_ID(track_id)
		, DESTINATION(destination)
		, DEPARTURE(departure)
		, FLIGHT_ALTITUDE(flight_al)
	{}
	string TRACK_ID;
	string DESTINATION;
	string DEPARTURE;
	int FLIGHT_ALTITUDE;

	void print() const
	{
		std::cout << TRACK_ID << " " << DESTINATION << " " << DEPARTURE << " " << FLIGHT_ALTITUDE << endl;
	}

	static FlightDetails from_string(const string& text)
	{
		vector<string> words_in_text = split(text);
		string TRACK_ID = words_in_text[0];
		string DESTINATION = words_in_text[1];
		string DEPARTURE = words_in_text[2];
		int FLIGHT_ALTITUDE = atoi(words_in_text[3].c_str());
		return FlightDetails(TRACK_ID, DESTINATION, DEPARTURE, FLIGHT_ALTITUDE);
	}

	FlightDetails& operator=(const FlightDetails& other)
	{
		if (&other == this)
			return *this;
		TRACK_ID = other.TRACK_ID;
		DESTINATION = other.DESTINATION;
		DEPARTURE = other.DEPARTURE;
		FLIGHT_ALTITUDE = other.FLIGHT_ALTITUDE;
		copy_count = std::max(copy_count, other.copy_count) + 1;
		return *this;
	}

	size_t copy_count = 0;
};

struct AirportDetails
{
	AirportDetails(const string& runway = "", int speed = 0, int x = 0, int y = 0, const string& track_id = "")
		: RUNWAY(runway)
		, SPEED(speed)
		, X_COORDINATE(x)
		, Y_COORDINATE(y)
		, TRACK_ID(track_id)
	{}
	string RUNWAY;
	int SPEED;
	int X_COORDINATE;
	int Y_COORDINATE;
	string TRACK_ID;

	void print() const
	{
		cout << RUNWAY << " " << SPEED << " " << X_COORDINATE << " " << Y_COORDINATE << " " << TRACK_ID << endl;
	}

	static AirportDetails from_string(const string& text)
	{
		vector<string> words_in_text = split(text);
		string RUNWAY = words_in_text[0];
		int SPEED = atoi(words_in_text[1].c_str());
		int X_COORDINATE = atoi(words_in_text[2].c_str());
		int Y_COORDINATE = atoi(words_in_text[3].c_str());
		string TRACK_ID = words_in_text[4];
		return AirportDetails(RUNWAY, SPEED, X_COORDINATE, Y_COORDINATE, TRACK_ID);
	}

};

string tabulation(string str)
{
	if (str.size() < 7)
		return "\t\t";
	else
		return "\t";
}

void print_structs(const vector<FlightDetails>& flights, const vector<AirportDetails>& airport)
{
	cout << endl;
	for (int i = 0; i < flights.size(); i++)
		flights[i].print();
	cout << endl;
	for (int i = 0; i < airport.size(); i++)
		airport[i].print();
}

void handling_mismatches(ofstream &merge, vector<AirportDetails>& airport)
{
	for (int i = 0; i < airport.size(); i++)
	{
		if (airport[i].TRACK_ID[airport[i].TRACK_ID.size() - 1] != '*')
		{
			merge << airport[i].TRACK_ID << tabulation(airport[i].TRACK_ID)
				<< "XXXX" << "\t\tXXXX" << "\t\tXXXX\t\t"
				<< airport[i].RUNWAY << tabulation(airport[i].RUNWAY)
				<< airport[i].SPEED << tabulation(to_string(airport[i].SPEED))
				<< airport[i].X_COORDINATE << tabulation(to_string(airport[i].X_COORDINATE))
				<< airport[i].Y_COORDINATE << endl;
		}
		else
			airport[i].TRACK_ID = airport[i].TRACK_ID.substr(0, airport[i].TRACK_ID.size()-1);
	}
}

void match_search(ofstream &merge, vector<FlightDetails>& flights, vector<AirportDetails>& airport)
{
	for (int i = 0; i < flights.size(); i++)
	{
		bool match_is_found = false;

		merge	<< flights[i].TRACK_ID << tabulation(flights[i].TRACK_ID)
				<< flights[i].DESTINATION << tabulation(flights[i].DESTINATION)
				<< flights[i].DEPARTURE << tabulation(flights[i].DEPARTURE)
				<< flights[i].FLIGHT_ALTITUDE << tabulation(to_string(flights[i].FLIGHT_ALTITUDE));

		for (int j = 0; j < flights.size(); j++)
		{
			if (flights[i].TRACK_ID == airport[j].TRACK_ID)
			{
				merge	<< airport[j].RUNWAY << tabulation(airport[j].RUNWAY)
						<< airport[j].SPEED << tabulation(to_string(airport[j].SPEED))
						<< airport[j].X_COORDINATE << tabulation(to_string(airport[j].X_COORDINATE))
						<< airport[j].Y_COORDINATE << endl;
				airport[j].TRACK_ID += "*";
				match_is_found = true;
			}
		}
		if (!match_is_found)
			merge << "XXXX" << "\t\tXXXX" << "\t\tXXXX" << "\t\tXXXX" << endl;
	}
	handling_mismatches(merge, airport);
}

void write_in_file(vector<FlightDetails>& flights, vector<AirportDetails>& airport, string way)
{
	ofstream file(way);
	match_search(file, flights, airport);
	file.close();
}

void update_struct(vector<AirportDetails>& airport)
{
	for (int i = 0; i < airport.size(); i++)
		airport[i].X_COORDINATE += 10000;
}

void read_FlightDetails(vector <FlightDetails>& flights)
{
	ifstream file("D:/flight_details.txt");
	string text;

	while (getline(file, text))
		flights.push_back(FlightDetails::from_string(text));
	file.close();
}

void read_AirportDetails(vector <AirportDetails>& airport)
{
	ifstream file("D:/airport_details.txt");
	string text;

	while (getline(file, text))
		airport.push_back(AirportDetails::from_string(text));
	file.close();
}

struct Pair
{
	Pair()
	{
	}
	FlightDetails flight;
	AirportDetails airport;

	void print() const
	{
		if (flight.TRACK_ID != "")
			cout << flight.TRACK_ID;
		else
			cout << airport.TRACK_ID;
		std::cout << " " << flight.DESTINATION << " " << flight.DEPARTURE << " " << flight.FLIGHT_ALTITUDE << " " << airport.RUNWAY << " " << airport.SPEED << " " << airport.X_COORDINATE << " " << airport.Y_COORDINATE << endl;
	}
};

int main()
{
	
	vector <FlightDetails> flights;
	read_FlightDetails(flights);

	vector <AirportDetails> airports;
	read_AirportDetails(airports);

	map <string, Pair> merge;
	for (FlightDetails flight : flights)
		merge[flight.TRACK_ID].flight = flight;
	
	for (AirportDetails airport : airports)
		merge[airport.TRACK_ID].airport = airport;

	for (map<string, Pair>::const_iterator it = merge.begin(); it != merge.end(); ++it)
	{
		it->second.print();
	}
	
	print_structs(flights, airports);
	write_in_file(flights, airports, "D:/merge.txt");			//Task 3
	update_struct(airports);
	write_in_file(flights, airports, "D:/merge2.txt");
	print_structs(flights, airports);

	for (map<string, Pair>::const_iterator it = merge.begin(); it != merge.end(); ++it)
	{
		std::cout << it->second.flight.copy_count << std::endl;
	}
}
