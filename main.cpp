/* @Author
 * Student Name: Ugurcan Polat
 * Student ID : 150140117
 * Date: 11.05.2018
 * * * * * * * * * * * * * * * * * *
 
 Compile command: g++ main.cpp -o thomassShops
 
 * * * * * * * * * * * * * * * * * */

#include <fstream>  // ifstream
#include <iostream> // cout
#include <sstream>  // stringstrem
#include <stack>    // stack
#include <cstdlib>  // atoi
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
    void findShopsFarthersFromEachOther(int& s, int& t) const;
    vector<bool> findAllReachableShops(int s) const;
    bool isThereAPath(int s, int t, vector<int>& path) const;
  public:
    Network(const vector< vector<bool> >& shops, int nRoads, int nShops);
    int findMinNumberOfClosedShops();
};

int main(int argc, const char * argv[]) {
    // Input file name argument must be passed with run command
    if (argc != 2) {
        cout << endl << "Missing arguments to proceed." << endl << endl;
        return 0;
    }

    ifstream inputFile(argv[1]); // Open file
    
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
    vector< vector<bool> > shopMatrix(numberOfShops,
                                      vector<bool>(numberOfShops,false));
    
    for(int i = 0; i < numberOfRoads; i++) {
        getline(inputFile, line); // Read line
        
        stringstream linestream(line);
        // Create stringstream to parse with space ' '
        linestream.str(line);
        
        // Parse the line
        getline(linestream, read[0], ' ');
        linestream >> read[1];
        
        int shopID1 = atoi(read[0].c_str());
        int shopID2 = atoi(read[1].c_str());
        
        // Directed roads
        shopMatrix[shopID1-1][shopID2-1] = true;
        shopMatrix[shopID2-1][shopID1-1] = true;
    }
    
    // Close the input file since it is no longer needed
    inputFile.close();
    
    // Create network and find minimum number of shops to be closed
    Network network(shopMatrix, numberOfRoads, numberOfShops);
    cout << network.findMinNumberOfClosedShops() << endl;
    return 0;
}

Network::Network(const vector< vector<bool> >& copy, int nRoads, int nShops) {
    shops = residualShops = copy;
    numberOfRoads = nRoads;
    numberOfShops = nShops;
}

void Network::findShopsFarthersFromEachOther(int& s, int& t) const {
    // This function ,as the name implies, finds the shops which are farthest
    // from each other.
    
    int maxDistance = 0;
    vector<int> path(numberOfShops,-1); // initialize with -1
    
    for (int i = 0; i < numberOfShops; i++) {
        for (int j = 1; j < numberOfShops; j++) {
            if (isThereAPath(i, j, path)) { // Check if there is a path
                int c = 0;
                for (int k = j; k != i; c++)
                    k = path[k]; // Traverse the path
                
                if (c > maxDistance) { // If distance is greater than max
                    maxDistance = c;
                    s = i; // new source
                    t = j; // new sink
                }
            }
            path.assign(path.size(), -1); // Reset the path
        }
    }
}

vector<bool> Network::findAllReachableShops(int s) const {
    // This function uses DFS to find all shops reachable from the given
    // shop |s|. This function implemented according to pseudocode given
    // in the slides.
    
    vector<bool> reached(numberOfShops,false); // Initial value false
    stack<int> dfsStack;
    
    dfsStack.push(s);
    reached[s] = true; // Source has been reached
    
    while(!dfsStack.empty()) {
        int u = dfsStack.top();
        dfsStack.pop();
        
        // Check the unreached shops and mark them as reached and add them
        // to the stack in order to check reached shops next.
        for (int i = 0; i < numberOfShops; i++) {
            if (!reached[i] && residualShops[u][i]) {
                dfsStack.push(i);
                reached[i] = true;
            }
        }
    }
    
    // Return all shops can be reached from the given source |s|.
    return reached;
}

bool Network::isThereAPath(int s, int t, vector<int>& path) const {
    // This function uses BFS to check if there is path between source |s| and
    // sink |t|. This function implemented according to pseudocode given
    // in the slides.
    
    // Flag check if the shop has been explored or not.
    vector<bool> flag(numberOfShops,false); // Initial value is false
    
    queue<int> bfsQueue;
    flag[s] = true; // Mark source as explored.
    bfsQueue.push(s);
    
    while (!bfsQueue.empty() && !flag[t]) {
        int u = bfsQueue.front();
        bfsQueue.pop();
        
        // Check all shops can be explored from the popped shop.
        for (int i = 0; i < numberOfShops && !flag[t]; i++) {
            if (!flag[i] && residualShops[u][i]) {
                bfsQueue.push(i);
                flag[i] = true;
                path[i] = u;
            }
        }
    }
    
    // Return the sink flag. It will be true if sink has been explored,
    // therefore there is a path between source and sink.
    return flag[t];
}

int Network::findMinNumberOfClosedShops() {
    // If number of shops is less than 5, no need to take action.
    if(numberOfShops < 5)
        return 0;
    
    int s,t,j;
    // Find source and sink shops.
    findShopsFarthersFromEachOther(s, t);
    
    int cs = 0, ct = 0, adjS = s, adjT = t;
    // Find adjacent nodes of source and sink.
    for (int i = 0; i < numberOfShops; i++) {
        if (shops[s][i]) {
            adjS = i;
            cs++;
        }
        
        if (shops[t][i]) {
            adjT = i;
            ct++;
        }
    }
    
    // If source has only one adjacent make that adjacent node source in order
    // to avoid having less than 1 node in sub-network after dividing.
    if (cs == 1)
        s = adjS;
    
    // If sink has only one adjacent make that adjacent node sink in order
    // to avoid having less than 1 node in sub-network after dividing.
    if (ct == 1)
        t = adjT;
    
    // Create vector that holds path from source to sink. Initial value -1.
    vector<int> path(numberOfShops,-1);
    
    // FORD-FULKERSON ALGORITHM
    // Make forward edges backward until there is no path from source to sink
    while(isThereAPath(s, t, path)) {
        for (int i = t; i != s;) {
            j = path[i];
            residualShops[i][j] = true;
            residualShops[j][i] = false;
            i = path[i];
        }
        path.assign(path.size(), -1); // Reset path with value -1
    }
    
    // Cut-A (first sub-network) holds all nodes reachable from source.
    // Other nodes are in Cut-B.
    vector<bool> cutA = findAllReachableShops(s);
    
    // Find number of nodes in cut-A.
    int numOfCutA = 0;
    for (int i = 0; i < numberOfShops; i++)
        if (cutA[i]) numOfCutA++;
    
    // Number of nodes in cut-B
    int numOfCutB = numberOfShops - numOfCutA;
    
    // If cut-A or cut-B has only one node, get adjacent nodes of that node
    // to the cut with one node.
    if (numOfCutA == 1) {
        for (int i = 0; i < numberOfShops; i++) {
            for (int j = 0; j < numberOfShops; j++) {
                if (cutA[i] && !cutA[j] && shops[i][j]) {
                    cutA[j] = true;
                    numOfCutA++;
                }
            }
            
            if (numOfCutB + numOfCutA != numberOfShops) {
                numOfCutB = numberOfShops - numOfCutA;
                break;
            }
            
        }
    } else if (numOfCutB == 1) {
        for (int i = 0; i < numberOfShops; i++) {
            for (int j = 0; j < numberOfShops; j++) {
                if (cutA[i] && !cutA[j] && shops[i][j]) {
                    cutA[i] = false;
                    numOfCutA--;
                }
            }
            
            if (numOfCutB + numOfCutA != numberOfShops) {
                numOfCutB = numberOfShops - numOfCutA;
                break;
            }
        }
    }
    
    // If above operations make some cut empty, return 0
    if (numOfCutA == 0 || numOfCutB == 0)
        return 0;
    
    // Vector that holds closed shops, initial value is false.
    vector<bool> closedShops(numberOfShops,false);
    
    for (int i = 0; i < numberOfShops; i++) {
        for (int j = 0; j < numberOfShops; j++) {
            // ith node is in cut-A and jth node in cut-B, there is a
            // road between them
            if (cutA[i] && !cutA[j] && shops[i][j]) {
                // Remove from cut-A if there are more than 2 nodes
                if (numOfCutA > 2) {
                    closedShops[i] = true;
                    numOfCutA--;
                // Remove from cut-B if there are more than 2 nodes
                } else if (numOfCutB > 2) {
                    closedShops[j] = true;
                    numOfCutB--;
                } 
            }
        }
    }
    
    
    // Check if there is still a road between cut-A and cut-B after
    // Ford-Fulkerson operations. If there is a road, return 0.
    for (int i = 0; i < numberOfShops; i++) {
        for (int j = 0; j < numberOfShops; j++) {
            if (cutA[i] && !cutA[j] && shops[i][j] && (!closedShops[i] &&
                                                       !closedShops[j])) {
                return 0;
            }
        }
        
    }
    
    // Calculate number of closed shops and return.
    int numOfClosedShops = numberOfShops - (numOfCutA + numOfCutB);
    return numOfClosedShops;
}
