#include <iostream> // output library
#include <fstream> // file read library
#include <string>  // string library
#include "json.hpp" // json library
#include "priorityqueue.h" //PriorityQueue class 
using namespace std; // namespace std

int main(int argc, char** argv) { // command line arguments
    if(argc!=2){ // check if incorrect amount of arguments
        cout << "Usage: ./buildheap.exe [file_name].json" << endl; // output usage format
        return 0; // exit
    }

    // something in JSON() is causing misorder of numbers in output, should be ordered by int instead of string

    ifstream f(argv[1]); // read in file from command line
    nlohmann::json data = nlohmann::json::parse(f); // parse the file

    size_t max = data["metadata"]["maxHeapSize"]; // create variable of size specified in json
    PriorityQueue tree{max}; // create empty heap of max size

    unsigned int totalZeros = (int) floor(log10((double) data["metadata"]["numOperations"])) + 1; // used for finding number of zeroes as used in createheapoperationdata.cxx
    
    for(int i=1; i <= data["metadata"]["numOperations"]; i++){ // loop through all operations
        
        int opZeros =  (int) floor(log10((double) i)) + 1; // determine # of necessary 0s for current index operation, used in createheapoperationdata.cxx
        std::string opNum = std::string(totalZeros - opZeros, '0').append(std::to_string(i)); // subtract operation 0s from total 0s to find necessary number of 0s at index
        string name = "Op" + opNum; // create index name to find data in json file

        if((data[name]["operation"] == "removeMin") && (tree.size() >= 1)){ // if command is to remove minimum node
            tree.removeMin(); // then remove the min node
        }
        else if(data[name]["operation"] == "insert"){ // else if command is to insert
            double key = data[name]["key"]; // turn json element into type double
            tree.insert(key); // insert current key
        }
    }

    nlohmann::json output = tree.JSON(); // create output json value
    output["metadata"]["maxHeapSize"] = data["metadata"]["maxHeapSize"]; // copy maxHeapSize to output
    output["metadata"]["numOperations"] = data["metadata"]["numOperations"]; // copy numOperations to output
    cout << output.dump(2) << endl; // dump output

    return 0; // exit program
}
