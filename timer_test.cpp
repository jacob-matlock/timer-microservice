#include <iostream> 
#include <sstream>   
#include <string>
#include <nlohmann/json.hpp>
#include <httplib.h>

using json = nlohmann::json;
using namespace std; 


int main() {
    json j;
    j["timer_length"] = 5;
    j["repeat_count"] = 2; 


    httplib::Client client("http://localhost:8000");

    int timer_ID = -1; 

    // Create Timer
    if (auto response = client.Post("/timer/set-timer", j, "application/json")) {
        // If the timer returns correctly: 

        // Print the status and response. 
        cout << "Status: " << response->status << endl; 
        cout << "Body: " << response->body << endl; 

        // Save the returned ID
        stringstream ss; 
        ss << "R\"(" << response->body << ")\"";
        json response_body = json::parse(ss.str()); 
        timer_ID = response_body["timer id"]; 

    } else {
        cerr << "HTTP Error: POST" << endl; 
    }



    // Get general path for /timer/timer_id
    stringstream timer; 
    timer << "/timer/" << to_string(timer_ID); 
    string timer_path = timer.str(); 


    // Pause timer
    stringstream pause; 
    pause << timer_path << "/pause";
    if (auto response = client.Post(pause.str())) {
        if (response->status == 404) {
            //No timer associated iwth ID
            cout << response->body << endl; 
        } else if (response->status == 304) {
            // Timer already paused
            cout << response->body << endl; 
        } else if (response->status == 409) {
            // Timer Inactive
            cout << response->body << endl; 
        } else {
            cout << "Timer Paused" << endl; 
        }
    } else {
        cerr << "HTTP Error: POST (pause)" << endl; 
    }

    // Resume timer
    stringstream resume; 
    resume << timer_path << "/resume"; 
    if (auto response = client.Post(resume.str())) {
        if (response->status == 404) {
            //No timer associated iwth ID
            cout << response->body << endl; 
        } else if (response->status == 304) {
            // Timer already paused
            cout << response->body << endl; 
        } else if (response->status == 409) {
            // Timer Inactive
            cout << response->body << endl; 
        } else {
            cout << "Timer Resumed" << endl; 
        }
    } else {
        cerr << "HTTP Error: POST (pause)" << endl; 
    }



    // Get status of previously created timer:
    if(auto response = client.Get(timer.str())) {
        // Save response as JSON
        stringstream ss; 
        ss << "R\"(" << response->body << ")\"";
        json response_body = json::parse(ss.str());

        // Print timer status
        cout << response_body["status"] << endl; 
    } else {
        cerr << "HTTP Error: GET" << endl; 
    }

    // Delete previously created timer using saved timer ID
    if(auto response = client.Delete(timer.str())) {
        if(response->status == 200) {
            cout << "Timer " << timer_ID << " deleted." << endl; 
        } else {
            cout << "Invalid Timer ID" << endl; 
        }
    }

    return 0; 
}