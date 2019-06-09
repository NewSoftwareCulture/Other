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
		: track_id(track_id)
		, destination(destination)
		, departure(departure)
		, flight_altitude(flight_al)
	{}
	string track_id;
	string destination;
	string departure;
	int flight_altitude;

	void print() const
	{
		std::cout << track_id << " " << destination << " " << departure << " " << flight_altitude << endl;
	}

	static FlightDetails from_string(const string& text)
	{
		vector<string> words_in_text = split(text);
		string track_id = words_in_text[0];
		string destination = words_in_text[1];
		string departure = words_in_text[2];
		int flight_altitude = atoi(words_in_text[3].c_str());
		return FlightDetails(track_id, destination, departure, flight_altitude);
	}

	FlightDetails& operator=(const FlightDetails& other)
	{
		if (&other == this)
			return *this;
		track_id = other.track_id;
		destination = other.destination;
		departure = other.departure;
		flight_altitude = other.flight_altitude;
		copy_count = std::max(copy_count, other.copy_count) + 1;
		return *this;
	}

	size_t copy_count = 0;
};

struct AirportDetails
{
	AirportDetails(const string& runway = "", int speed = 0, int x = 0, int y = 0, const string& track_id = "")
		: runway(runway)
		, speed(speed)
		, x_coordinate(x)
		, y_coordinate(y)
		, track_id(track_id)
	{}
	string runway;
	int speed;
	int x_coordinate;
	int y_coordinate;
	string track_id;

	void print() const
	{
		cout << runway << " " << speed << " " << x_coordinate << " " << y_coordinate << " " << track_id << endl;
	}

	static AirportDetails from_string(const string& text)
	{
		vector<string> words_in_text = split(text);
		string runway = words_in_text[0];
		int speed = atoi(words_in_text[1].c_str());
		int x_coordinate = atoi(words_in_text[2].c_str());
		int y_coordinate = atoi(words_in_text[3].c_str());
		string track_id = words_in_text[4];
		return AirportDetails(runway, speed, x_coordinate, y_coordinate, track_id);
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
		if (airport[i].track_id[airport[i].track_id.size() - 1] != '*')
		{
			merge << airport[i].track_id << tabulation(airport[i].track_id)
				<< "XXXX" << "\t\tXXXX" << "\t\tXXXX\t\t"
				<< airport[i].runway << tabulation(airport[i].runway)
				<< airport[i].speed << tabulation(to_string(airport[i].speed))
				<< airport[i].x_coordinate << tabulation(to_string(airport[i].x_coordinate))
				<< airport[i].y_coordinate << endl;
		}
		else
			airport[i].track_id = airport[i].track_id.substr(0, airport[i].track_id.size()-1);
	}
}

void match_search(ofstream &merge, vector<FlightDetails>& flights, vector<AirportDetails>& airport)
{
	for (int i = 0; i < flights.size(); i++)
	{
		bool match_is_found = false;

		merge	<< flights[i].track_id << tabulation(flights[i].track_id)
				<< flights[i].destination << tabulation(flights[i].destination)
				<< flights[i].departure << tabulation(flights[i].departure)
				<< flights[i].flight_altitude << tabulation(to_string(flights[i].flight_altitude));

		for (int j = 0; j < flights.size(); j++)
		{
			if (flights[i].track_id == airport[j].track_id)
			{
				merge	<< airport[j].runway << tabulation(airport[j].runway)
						<< airport[j].speed << tabulation(to_string(airport[j].speed))
						<< airport[j].x_coordinate << tabulation(to_string(airport[j].x_coordinate))
						<< airport[j].y_coordinate << endl;
				airport[j].track_id += "*";
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
		airport[i].x_coordinate += 10000;
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
		if (flight.track_id != "")
			cout << flight.track_id;
		else
			cout << airport.track_id;
		std::cout << " " << flight.destination << " " << flight.departure << " " << flight.flight_altitude << " " << airport.runway << " " << airport.speed << " " << airport.x_coordinate << " " << airport.y_coordinate << endl;
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
		merge[flight.track_id].flight = flight;
	
	for (AirportDetails airport : airports)
		merge[airport.track_id].airport = airport;

	for (map<string, Pair>::const_iterator it = merge.begin(); it != merge.end(); ++it)
		it->second.print();
	
	write_in_file(flights, airports, "D:/merge.txt");			//Task 3
	update_struct(airports);
	write_in_file(flights, airports, "D:/merge2.txt");

	/*for (map<string, Pair>::const_iterator it = merge.begin(); it != merge.end(); ++it)
	{
		std::cout << it->second.flight.copy_count << std::endl;
		
	}*/

}
