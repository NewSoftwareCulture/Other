#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const string file_path = "D:/merge.txt";

vector<string> split(const string& text, char ch)
{
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

string tabulation(const string& str)
{
    return str.size() < 7 ? "\t\t" : "\t";
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
        vector<string> words_in_text = split(text, ' ');
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
        vector<string> words_in_text = split(text, ' ');
        string runway = words_in_text[0];
        int speed = atoi(words_in_text[1].c_str());
        int x_coordinate = atoi(words_in_text[2].c_str());
        int y_coordinate = atoi(words_in_text[3].c_str());
        string track_id = words_in_text[4];
        return AirportDetails(runway, speed, x_coordinate, y_coordinate, track_id);
    }

};

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
            cout << flight.track_id << tabulation(flight.track_id);
        else
            cout << airport.track_id << tabulation(airport.track_id);
        std::cout << " " << flight.destination << tabulation(flight.destination) << flight.departure << tabulation(flight.departure) << flight.flight_altitude << tabulation(to_string(flight.flight_altitude)) << airport.runway << tabulation(airport.runway) << airport.speed << tabulation(to_string(airport.speed)) << airport.x_coordinate << tabulation(to_string(airport.x_coordinate)) << airport.y_coordinate << endl;
    }
    void write_in_file() const
    {
        ofstream file;
        file.open(file_path, ios::app);
        if (flight.track_id != "")
            file << flight.track_id << tabulation(flight.track_id);
        else
            file << airport.track_id << tabulation(airport.track_id);
        file << " " << flight.destination << tabulation(flight.destination) << flight.departure << tabulation(flight.departure) << flight.flight_altitude << tabulation(to_string(flight.flight_altitude)) << airport.runway << tabulation(airport.runway) << airport.speed << tabulation(to_string(airport.speed)) << airport.x_coordinate << tabulation(to_string(airport.x_coordinate)) << airport.y_coordinate << endl;
    }
};

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

void combination_structs(const vector <FlightDetails>& flights, const vector <AirportDetails>& airports, map <string, Pair>& merge)
{
    for (const FlightDetails& flight : flights)
        merge[flight.track_id].flight = flight;

    for (const AirportDetails& airport : airports)
        merge[airport.track_id].airport = airport;
}

void print_struct(const map <string, Pair>& merge)
{
    for (map<string, Pair>::const_iterator it = merge.begin(); it != merge.end(); ++it)
    it->second.print();
}

void write_in_file(const map <string, Pair>& merge)
{
    ofstream file(file_path);		//clear file
    file.close();
    for (map<string, Pair>::const_iterator it = merge.begin(); it != merge.end(); ++it)
        it->second.write_in_file();
}

int main()
{
    vector <FlightDetails> flights;
    vector <AirportDetails> airports;
    map <string, Pair> merge;

    read_FlightDetails(flights);
    read_AirportDetails(airports);
    
    update_struct(airports);
    combination_structs(flights, airports, merge);
    print_struct(merge);
    write_in_file(merge);
}
