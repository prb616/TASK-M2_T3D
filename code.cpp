/*TASK - M2T3D
name : prabhjot singh
roll no. :2210994884*/

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

// Define the structure for traffic signal data
struct TrafficData {
    string time;
    int trafficLightId;
    int numCarsPassed;
};

const unsigned int maxBufferSize = 10;
const int numTrafficSignals = 10; // Update to produce data for 10 traffic signals

std::mutex mu;
std::condition_variable cond;
deque<TrafficData> buffer;

void producer(ifstream& input_file) {
    string line;

    // Read lines from the input file until reaching the end
    while (getline(input_file, line)) {
        istringstream iss(line);
        string time;
        int trafficLightId;
        int numCarsPassed;

        if (!(iss >> time >> trafficLightId >> numCarsPassed)) {
            cerr << "Invalid input format\n";
            continue; // Skip this line if invalid format
        }

        TrafficData data = { time, trafficLightId, numCarsPassed };

        {
            std::unique_lock<std::mutex> locker(mu);
            buffer.push_back(data);
        } // Unlock mutex after pushing data (automatically)

       }

    // Notify consumer that buffer has data
    cond.notify_one();
}


void consumer() {
    while (true) {
        std::unique_lock<std::mutex> locker(mu);
        cond.wait(locker, []() { return !buffer.empty(); });

        // Process the current data
        vector<TrafficData> currentData;
        currentData.reserve(buffer.size());
        while (!buffer.empty()) {
            currentData.push_back(buffer.front());
            buffer.pop_front();
        }

        locker.unlock();

        // Sort currentData based on time first, then on the number of cars passed
        sort(currentData.begin(), currentData.end(), [](const TrafficData& a, const TrafficData& b) {
            if (a.time != b.time) {
                return a.time < b.time; // Sort based on time
            } else {
                return a.numCarsPassed > b.numCarsPassed; // If timestamps are equal, sort based on cars passed
            }
        });

        // Display the sorted data
        cout << "Sorted traffic data:" << endl;
        for (const auto& data : currentData) {
            cout << "Time: " << data.time << ", Traffic Light ID: " << data.trafficLightId << ", Cars Passed: " << data.numCarsPassed << endl;
        }

        // If buffer is empty, break the loop
        if (buffer.empty()) {
            break;
        }
    }
}




int main() {
    // Start time
    auto start = chrono::steady_clock::now();

    // Open the input file
    ifstream input_file("log.txt");
    if (!input_file) {
        cerr << "Failed to open input file\n";
        return 1;
    }

    // Vector to store producer threads
    vector<thread> producers;

    // Create producer threads in a loop
    for (int i = 0; i < 1; ++i) { // Create 10 producer threads
        producers.emplace_back(producer, std::ref(input_file));
    }

    // Join all producer threads
    for (auto& p : producers) {
        p.join();
    }

    // Create the consumer thread
    std::thread c1(consumer);

    // Join the consumer thread
    c1.join();

    // End time
    auto end = chrono::steady_clock::now();

    // Calculate duration
    chrono::duration<double, std::milli> duration = end - start;

    // Print the time taken by the program to execute
    cout << "Time taken by the program: " << duration.count() << " milliseconds" << endl;

    return 0;
}
