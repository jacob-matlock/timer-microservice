#include <iostream> 
#include <sstream>   
#include <string>
#include <nlohmann/json.hpp>
#include <httplib.h>

using json = nlohmann::json;
using namespace std; 

/*
*   Note: I don't know how this will vary per each person's environment, but my compilation command is as follows. 
*           g++ timer_test.cpp -o test -lcpp-httplib -pthread
*/

int main() {
    // Create a JSON object with a "duration" and "count" field to POST to the microservice.
    json j;
    j["duration"] = 5;
    j["count"] = 2;  

    // Connect to port 8000, Initialize a variable for future storage of the timer ID
    httplib::Client client("http://localhost:8000");
    string timer_ID; 
 

    /*
    * This block tests creating a timer. Based on the JSON object defined earlier, the timer will run for a total of 10 seconds with 1 repetition.
    */
    cout << "Test 1: POST (Start new timer)" << endl; 
    if (auto response = client.Post("/timer/set-timer", j.dump(), "application/json")) {   // If the timer returns correctly: 

        // Print the status and response. 
        cout << "Status: " << response->status << endl; 
        cout << "Body: " << response->body << endl; 

        if (response->status == 200) {  // If the timer posted correctly
            // Save the returned ID
            stringstream ss; 
            ss << response->body;
            json response_body = json::parse(ss.str()); 
            timer_ID = response_body["timer id"].get<string>(); 
        }
        

    } else {
        // Print error. 
        cerr << "TCP Error: POST" << endl; 
        cerr << response.error() << endl; 
    }



    
    // Get general path for /timer/timer_id
    stringstream timer; 
    timer << "/timer/" << timer_ID; 
    string timer_path = timer.str(); 

    /*
    * This block tests pausing the timer, using the timer ID returned in the earlier POST call. 
    */
    cout << "Test 2: POST (Pause timer)" << endl;
    stringstream pause; 
    pause << timer_path << "/pause";
    if (auto response = client.Post(pause.str())) {     // If the POST call returns correctly:
        if (response->status == 404) {
            //No timer associated with ID
            cout << "Status: " << response->status << endl; 
            cout << "Body: " << response->body << endl;  
        } else if (response->status == 304) {
            // Timer already paused
            cout << "Status: " << response->status << endl; 
            cout << "Body: " << response->body << endl; 
        } else if (response->status == 409) {
            // Timer Inactive
            cout << "Status: " << response->status << endl; 
            cout << "Body: " << response->body << endl; 
        } else {
            // Timer successfully paused
            cout << "Status: " << response->status << endl; 
            cout << "Body: " << response->body << endl; 
        }
    } else {
        // Print error.
        cerr << "TCP Error: POST (pause)" << endl; 
        cerr << response.error() << endl;
    }

    /*
    * This block tests resuming the timer, using the timer ID returned in the earlier POST call. 
    */
    cout << "Test 3: POST (Resume timer)" << endl;
    stringstream resume; 
    resume << timer_path << "/resume"; 
    if (auto response = client.Post(resume.str())) {    // If the POST call returns correctly:
        if (response->status == 404) {
            //No timer associated with ID
            cout << "Status: " << response->status << endl; 
            cout << "Body: " << response->body << endl;  
        } else if (response->status == 304) {
            // Timer already active
            cout << "Status: " << response->status << endl; 
            cout << "Body: " << response->body << endl;  
        } else if (response->status == 409) {
            // Timer Inactive
            cout << "Status: " << response->status << endl; 
            cout << "Body: " << response->body << endl;  
        } else {
            // Timer successfully resumed
            cout << "Status: " << response->status << endl; 
            cout << "Body: " << response->body << endl;  
        }
    } else {
        // Print error.
        cerr << "TCP Error: POST (resume)" << endl; 
        cerr << response.error() << endl;
    }

    /*
    * This block tests retrieving the status of the timer, using the timer ID returned in the earlier POST call. 
    */
    cout << "Test 4: GET (Check timer status)" << endl;
    stringstream details; 
    details << timer_path << "/details";
    if(auto response = client.Get(details.str())) {       // If the GET call returns correctly:
        // Print timer status
        cout << "Status: " << response->status << endl; 
        cout << "Body: " << response->body << endl; 

    } else {
        // Print error.
        cerr << "TCP Error: GET" << endl; 
        cerr << response.error() << endl;
    }

    /*
    * This block tests deleting the timer, using the timer ID returned in the earlier POST call. 
    */
    cout << "Test 5: DELETE (Delete active timer)" << endl;
    stringstream deletion; 
    deletion << timer_path << "/delete";
    if(auto response = client.Delete(deletion.str())) {    // If the DELETE call returns correctly:
        if(response->status == 200) {
            // Successful Deletion
            cout << "Status: " << response->status << endl; 
            cout << "Body: " << response->body << endl;   
        } else {
            // Unsuccessful Deletion (Invalid ID)
            cout << "Status: " << response->status << endl; 
            cout << "Body: " << response->body << endl;  
        }
    } else {
        // Print error.
        cerr << "TCP Error: DELETE" << endl; 
        cerr << response.error() << endl;
    }


    return 0;
}