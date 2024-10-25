#include <iostream> // output library
#include <fstream> // file read library
#include <string>  // string library
#include <cstdlib> // library for math stuff
#include "json.hpp" // json library
#include "priorityqueue.h" //PriorityQueue class 
using namespace std; // namespace std

Value findFairTeam(const nlohmann::json& data, nlohmann::json& out, size_t teams, int a); // function that finds the fairest team pair
nlohmann::json createNewHeap(nlohmann::json& data, Value p, size_t teams, size_t num); // function that creates the new heap after removing the fairest pair

int main(int argc, char** argv) {
    if(argc!=2){ // check if incorrect amount of arguments
        cout << "Usage: ./createteams.exe [file_name].json" << endl; // output usage format
        return 0; // exit
    }

    ifstream f(argv[1]); // read in file from command line
    nlohmann::json data = nlohmann::json::parse(f); // parse the file

    size_t num = data["metadata"]["numPlayers"]; // create variable of size specified in json
    size_t teams = 0; // keep track of total number of teams

    for (size_t i=1; i < num; i++){ // loop to determine number of teams from num players
        teams += i; // add to summation
    }

    PriorityQueue tree{teams}; // create empty heap of max size

    for(size_t j=0; j < teams; j++){ // loop through all possible teams in json
        std::pair<int,int> p; // create team pair
        p.first = data["teamStats"][j]["playerOne"]; // insert player 1 data
        p.second = data["teamStats"][j]["playerTwo"]; // insert player 2 data
        
        std::pair<double,Value>  t; // create team pair with associated win%
        t.first = data["teamStats"][j]["winPercentage"]; // add win percentage to team data
        t.second = p; // add team members pair to team data
        
        tree.insert(t); // insert team keyvaluepair into tree

    }

    nlohmann::json output; // create output json to store selected teams
    
    nlohmann::json temp = tree.JSON(); // create temp json tree to be modified in our search
    
    for(int k=0; num >= 2; k++){ // loop through all teams
        std::pair<int,int> val; // declare arbitrary value pair to store result of fair team function
        val = findFairTeam(temp,output,teams,k); // call findFairTeam, store to value
        temp = createNewHeap(temp,val,teams,num); // call createNewHeap with the previous value
        
        num-=2; // decrease by number of total players by 2
        teams = 0; // set teams back to 0 to recalculate it
        for (size_t l=1; l < num; l++){ // re-determine number of teams from num players
            teams += l; // add to summation
        }
    }
    
    cout << output.dump(2) << endl; // dump output
    return 0; // exit program

}

Value findFairTeam(const nlohmann::json& data, nlohmann::json& out, size_t teams, int a){ // function to find fairest team
    double fair = data["1"]["key"]; // initialize first entry's key to compare to
    double check = 0; // declare variable to store compared key values
    size_t team = 0; // find specific most fair team
    for(size_t k=1; k<=teams; k++){ // loop through all current teams
        check = data[to_string(k)]["key"]; // store current key to double type variable for comparison
        if((abs(check - 50.0) <= abs(fair - 50.0)) || (teams==1)){ // find team closest to 50 win%
            fair = data[to_string(k)]["key"]; // then set fair to team closest to 50 win%
            team = k; // save index of team for writing to output
        }
    }
    std::pair<int,int> p; // create pair to store team players
    p.first = data[to_string(team)]["value"][0]; // store first team player
    p.second = data[to_string(team)]["value"][1]; // store second team player
    out["teams"].push_back(p); // append this team to list in output json
    return p; // return fairest pair
}

nlohmann::json createNewHeap(nlohmann::json& data, Value p, size_t teams, size_t num){ // function to create an updated heap excluding the fairest team members
    nlohmann::json temp; // define a temporary json value to hold this new heap
    int p1 = p.first; // read in first fair team member
    int p2 = p.second; // read in second fair team member
    size_t total = 1; // initialize a total to keep track of number of teams
    for(size_t i=1; i<=teams; i++){ // loop through all teams
        std::pair<int,int> d = data[to_string(i)]["value"]; // read in team pair at current index to dummy variable
        if((d.first != p1) && (d.first != p2) && (d.second != p1) && (d.second != p2)){ // check if the fair team members are not on the given team
            std::pair<int,int> t; // create arbitrary pair variable to hold valid team
            t.first = d.first; // copy first team member
            t.second = d.second; // copy second team member
            
            temp[to_string(total)]["key"] = data[to_string(i)]["key"]; // copy win% of current index team to temp
            temp[to_string(total)]["value"] = t; // copy current index team members to temp
            total++; // increment total
        }
    }
    teams = 0; // reset teams counter to recalculate new number of teams
    for(size_t l=1; l < (num-2); l++){ // re-determine number of teams from num players
        teams += l; // add to summation
    }
    temp["metadata"]["size"] = teams; // set new number of teams in metadata of temp json for reuse in this function

    return temp; // return entire temp json
}