/* @Author
 * Student Name: Ugurcan Polat
 * Student ID : 150140117
 * Date: 8.05.2018
 * * * * * * * * * * * * * * * * * *
 
 Compile command: g++ main.cpp -o thomassShops
 
 * * * * * * * * * * * * * * * * * */

#include <fstream>  // ifstream
#include <iostream> // cout
#include <sstream>  // stringstrem
#include <stack>    // stack
#include <string>   // string
#include <vector>   // vector
#include <queue>    // queue

using namespace std;

class Network {
  private:
    vector< vector<bool> > shops;
    vector< vector<bool> > residualShops;
    int numberOfRoads;
    int numberOfShops;
    bool bfsIsThereAPath(int s, int t, vector<int>& path) const;
  public:
    Network(const vector< vector<bool> >& shops, int nRoads, int nShops);
    vector<bool>& operator[](int i);
    int findMinNumberOfClosedShops();
};

int main(int argc, const char * argv[]) {
    // Input file name argument must be passed with run command
    if (argc != 2) {
        cout << endl << "Missing arguments to proceed." << endl << endl;
        return 0;
    }
    
    // Get input file name and open the file
    string input_filename(argv[1]);
    ifstream inputFile(input_filename, ifstream::in); // Read
    
    if (!inputFile.is_open()) { // Error while openning the file
        cout << endl << "Error opening input file." << endl << endl;
        return 0;
    }
    
    string line, read[2];
    getline(inputFile, line); // Read the first line
    
    // Create stringstream to parse with space ' '
    stringstream linestream(line);
    
    // Parse the line with ' '
    getline(linestream, read[0], ' ');
    linestream >> read[1];
    
    // Get number of shops and number of roads from the first line
    int numberOfShops = atoi(read[0].c_str());
    int numberOfRoads = atoi(read[1].c_str());
    
    // Create matrix of integers that holds if there is a road between
    // two shops or not. If there is a road the value is 1, otherwise 0.
    vector< vector<bool> > shopMatrix(numberOfShops, vector<bool>(numberOfShops,false));
    
    for(int i = 0; i < numberOfRoads; i++) {
        getline(inputFile, line); // Read line
        
        // Create stringstream to parse with space ' '
        linestream.str(line);
        
        // Parse the line
        getline(linestream, read[0], ' ');
        linestream >> read[1];
        
        int shopID1 = atoi(read[0].c_str());
        int shopID2 = atoi(read[1].c_str());
        
        shopMatrix[shopID1-1][shopID2-1] = true;
        shopMatrix[shopID2-1][shopID1-1] = true;
    }
    
    // Close the input file since it is no longer needed
    inputFile.close();
    
    Network network(shopMatrix, numberOfRoads, numberOfShops);
    cout << network.findMinNumberOfClosedShops() << endl;
    return 0;
}

Network::Network(const vector< vector<bool> >& copy, int nRoads, int nShops) {
    shops = copy;
    numberOfRoads = nRoads;
    numberOfShops = nShops;
}

bool Network::bfsIsThereAPath(int s, int t, vector<int>& path) const {
    vector<bool> flag(numberOfShops,false);
    
    queue<int> bfsQueue;
    flag[s] = true;
    bfsQueue.push(s);
    
    while (!bfsQueue.empty()) {
        int u = bfsQueue.front();
        bfsQueue.pop();
        
        for (int i = 0; i < numberOfShops; i++) {
            if (!flag[i] && residualShops[u][i]) {
                bfsQueue.push(i);
                flag[i] = true;
                path[i] = u;
            }
        }
    }
    
    return flag[t];
}

int Network::findMinNumberOfClosedShops() {
    return 0;
}
