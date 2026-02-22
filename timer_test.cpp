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


    // TO DO - Update port
    httplib::Client client("http://localhost:6000");

    int timer_ID = -1; 

    // Create Timer
    if (auto response = client.Post("/timers", j, "application/json")) {
        // If the timer returns correctly: 

        // Print the status and response. 
        cout << "Status: " << response->status << endl; 
        cout << "Body: " << response->body << endl; 

        // Save the returned ID
        stringstream ss; 
        ss << "R\"(" << response->body << ")\"";
        json response_body = json::parse(ss.str()); 
        timer_ID = response_body["ID"]; 

    } else {
        cerr << "HTTP Error: POST" << endl; 
    }

    // Get status of previously created timer:
    stringstream timer; 
    timer << "/timers/" << to_string(timer_ID); 
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