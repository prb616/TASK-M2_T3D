/*TASK - M2T3D
name : prabhjot singh
roll no. :2210994884*/

#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

#define NUM_LIGHTS 12
#define MEASUREMENTS 13

// Function to print details of traffic volume measurements
void printDetails(int array[NUM_LIGHTS][MEASUREMENTS]){
    // Loop through each light
    for(int i = 1; i <= NUM_LIGHTS ; i++){
        // Print light number
        cout << to_string(i) + ":\t";
        // Loop through each measurement for the current light
        for (int j = 0 ; j < sizeof(array[i])/4 ; j++){
            // Print traffic volume measurement
            cout << to_string(array[i-1][j]) + "\t\t";
        }
        cout<<endl;
    }
}

// Function to calculate average traffic volume for each light
void caluclateTrafficVolume(int array[NUM_LIGHTS][MEASUREMENTS]){
    cout<<"calculating...\t";
    // Loop through each light
    for (int i = 0 ; i < NUM_LIGHTS ; i++){
        // Initialize total traffic volume for the current light
        array[i][0] = 0;
        // Loop through each measurement for the current light
        for (int j = 1 ; j < MEASUREMENTS ; j++){
            // Sum up traffic volume measurements
            array[i][0]+=array[i][j];
        }
        // Calculate average traffic volume for the current light
        array[i][0] = array[i][0] / (MEASUREMENTS-1);
    }
    cout<<"Done"<<endl;
}

int main(){

    int Array[NUM_LIGHTS][MEASUREMENTS];
    int arraySize = sizeof(Array[1])/4;      // 4 bytes per integer

    long rawTime;

    int lightNum;
    int trafficVolume;

    // Open log file for reading
    fstream log("log.txt", ios::in);
    // Check if the log file is opened successfully
    if(!log.is_open()){
        // Print error message if the log file is not found
        cout<<"Error: Log file not found"<<endl;
    } else {
        cout<<"Retrieving initial data from log.txt...\t";
        // Initialize all elements of Array to zero
        for(int i = 1 ; i <= NUM_LIGHTS ; i++){
            for (int j = 1 ; j < arraySize ; j++){
                Array[i-1][j]=0;
            }
        }
        
        // Read initial data from log file
        log >> rawTime >> lightNum >> trafficVolume;

        // Loop through each hour of recorded data
        for(int k = 0 ; k < 12 ; k++){
            // Loop through each light
            for(int i = 0 ; i < NUM_LIGHTS ; i++){
                // Read data for each light
                log >> rawTime >> lightNum >> trafficVolume;

                // Shift previous measurements by one position
                for(int j = sizeof(Array[0]) / sizeof(Array[0][0]) - 1 ; j>0;j--){
                    Array[lightNum-1][j] = Array[lightNum-1][j-1];
                }
                // Store the latest traffic volume measurement
                Array[lightNum-1][1] = trafficVolume;
            }
        }
        // Close the log file after reading
        log.close();
        cout<<"Complete"<<endl;

    }
    // Calculate average traffic volume for each light
    caluclateTrafficVolume(Array);

    // Print traffic details
    printDetails(Array);

    // Check if there is no traffic left
    bool noTraffic = true;
    for(int i = 0; i < NUM_LIGHTS; i++) {
        if(Array[i][0] != 0) {
            noTraffic = false;
            break;
        }
    }

    // Print message if there is no traffic left
    if(noTraffic) {
        cout << "No traffic is left." << endl;
    }

    return 0;
}
