#include <iostream>
#include <random>
#include <ctime>
#include <fstream>
#include <iomanip> // for setw
#include <sstream> // for stringstream
#include <time.h>

using namespace std;

#define NUM_LIGHTS 12 // number of traffic lights

int main(){

    // Open log file in append mode
    fstream log("log.txt", ios::app);
    // Check if log file is opened successfully
    if(!log.is_open()){
        // Print error message if log file is not found
        cout<<"Error: Log file not found"<<endl;
        return 1; // Exit the program with error code
    }

    // Get current time
    time_t rawTime;
    time(&rawTime);

    // Convert raw time to tm struct to extract hours and minutes
    struct tm * timeinfo;
    timeinfo = localtime(&rawTime);
    stringstream timeFormat;
    timeFormat << setw(2) << setfill('0') << timeinfo->tm_hour << ":" << setw(2) << setfill('0') << timeinfo->tm_min;

    // Write header to log file
    //log << "Timestamp\tLight ID\tNumber of Cars\n";

    // Seed random number generator
    srand (time(NULL));

    // Generate traffic information for each traffic light
    for (int i = 1 ; i <= NUM_LIGHTS ; i++){
        
        // Random number of cars
        int cars = rand() % ((10 - 1) + 1) + 1;

        // Write traffic information to log file in table format
        log << timeFormat.str() << "\t\t" << i << "\t\t" << cars << endl; // Log entry format: timestamp (in hrs:min); light number; number of cars (random)
    }

    // Print message indicating log file has been updated
    cout<<"log.txt has been updated at "<<ctime(&rawTime)<<endl;

    // Close log file
    log.close();

    return 0;
}
