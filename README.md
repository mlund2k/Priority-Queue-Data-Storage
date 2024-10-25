# Priority-Queue-Data-Storage


This project utilizes a self created priority queue class data structure to store and sort data on fictitious sports teams.


This program was compiled using WSL (Windows Subsystem for Linux) on Windows 10 and 11 and runs perfectly in such environments.


**Steps to run:**
- Copy files into one directory
- Set the location of the files as your directory within your terminal of choice
- Execute ```make``` to compile the project
- The following steps are optional, and are used purely to test the functionality of the priority heap class:
    - Execute ```./createheapoperationdata.exe <numOperations> <maxHeapSize>``` to create test data to use in the following program. Specify both a number of operations to create and a maximum heap size in the respective arguments. Output will show up in ```ex.json```.
    - Execute ```./buildheap.exe ex.json``` to store the properly formatted data from a file such as ex.json or similar, to a new priority heap data structure.
- The following steps utilize the priority heap to actually function on an example of sorting teams using a greedy algorithm:
    - Execute ```./createteamdata.exe <numPlayers>``` to create test data of teams to use in the following program. Specify an even number of players to create teams from in the respective argument (The program will pair each player with every other available player leading to a arithmetic sum up to N-1, where N is the number of players specified, so 20 players will create 190 teams, and about 1000 lines of code). Output will show up in ```exteams.json```.
    - Execute ```./createteamdata.exe <numPlayers>``` to sort player/team data from the previous step into fair teams. The sorting is executed through a greedy algorithm choosing the winning percent of the team closest to 50% at the time in order to create the fairest competition possible.


If compiling raises a fatal error on ```#include <json/json.h>```, make sure to run the following commands to install json on your computer:
```sudo apt-get install libjsoncpp-dev```
```sudo ln -s /usr/include/jsoncpp/json/ /usr/include/json```
Credits: https://github.com/stardust95/TinyCompiler/issues/2#issuecomment-410946235
