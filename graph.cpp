#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <map>
#include <queue>

using namespace std;

struct node{
    typedef pair<double,node*> ve;
    vector<ve> adj; //cost of edge, destination node
    string name;
    node(string s)
    {
        name=s;
    }
    double totCost;     //Used by BFS alforithm
    node* parent;       //Used by BFS algorithm
    bool current, visited;  //Used by shortest path method
    
};

class graph {
    public: 
    typedef std::map<string, node *> nmap;
    nmap work;
    void addNode(const string&);
    void addEdge(const string& from, const string& to, double cost);
    double findShortestPath(const string& from, const string& to);
    double breadthFirstSearch(const string& from, const string& to);
    void deleteNode(node*); 
    virtual ~graph();
    string gIntToString(const int n);
    
    private:
    int numNodes = 0;
};

void graph::addNode(const string &name) {
    nmap::iterator itr=work.begin();
    itr=work.find(name);
    if(itr==work.end())
    {
        node *n;
        n= new node(name);
        work[name]=n;
        numNodes++;
        return;
    }
    cerr<<"Node "<<name<<" already exists!"<< endl;
    return;
}

void graph::addEdge(const string& from, const string& to, double cost) {
    node *f=(work.find(from)->second);
    node *t=(work.find(to)->second);
    
    //Optimization #2: Avoid duplicate edges
    /* If an edge already exists between from and to nodes, do not add another.
    Note, however, that depending upon the metadata contained in edges or waht they represent (e.g. roads),
    duplicate edges may be acceptable or even desirable.  This is specifically for the hackerrank problem.
    */
    for (auto it = f->adj.begin() ; it != f->adj.end(); ++it) {
        if ((it->second == t)&&(it->first == cost)) {
            //duplicate edge, discard this edge
            cerr << "Discarding duplicate edge between nodes " << from << " and " << to << endl;
            return;
        }
    }    
    
    pair<double,node *> fwdEdge, bkwdEdge; 
    fwdEdge = make_pair(cost,t);
    f->adj.push_back(fwdEdge);
    //Undirected graph.  Add bi-directional edge so that edge appears in the adjacency list of both from and to nodes.
    bkwdEdge = make_pair(cost,f);
    t->adj.push_back(bkwdEdge);
    return;
}

double graph::breadthFirstSearch(const string& from, const string& to) {
    //Returns sum of the cost of all edges between from and to.
    //In the hackerrank problem, however, the cost of all edges was 6.
    //https://en.wikipedia.org/wiki/Breadth-first_search 
    
    node *destination = (work.find(to)->second);
    if (destination->adj.empty()) {
        //Optimization #1: If destination node has no links, it is unreachable.  Avoid search.
        cerr << "Node " << to << " has no links.  It is unreachable." << endl;
        return -1.0;
    }
    
    double cost;
    
    // Set distance of all nodes to infinity (-1)
    for (int n=1; n<=numNodes; n++) {
        string NodeName;
        NodeName = gIntToString(n);
        node *np = (work.find(NodeName)->second);
        if (np) {
            np->totCost=-1.0;   //distance between this node and the 'from' node is not yet determined
            np->parent=NULL;    //predecessor of this node has not yet been determined 
        }
    }
    
    node *source = (work.find(from)->second);
    source->totCost = 0.0; //distance of from node to itself is 0.
    
    std::queue<node*> Q;
    Q.push(source);    //Add source to queue, as we will explore this node first
    
    bool found = false;
    while (!Q.empty() && !found) {
        node *currNode = Q.front();
        Q.pop();
        if (currNode == destination) {
            cost = currNode->totCost;
            found = true;
        } else {
            for (auto it = currNode->adj.begin() ; it != currNode->adj.end(); ++it) {
                //For each node n that is adjacent to currNode:
                node *n = it->second;
                if (n->totCost == -1.0) {   //if node not previously visited
                    n->totCost = currNode->totCost + it->first;
                    n->parent = currNode;
                    Q.push(n);
                } //end if node not previously visited
            } //end for each adjacent node
        } //end if we have reached our destination  
    } //end while queue Q not empty
    
    if (!found) cost = -1.0;
    return cost;
}

double graph::findShortestPath(const string& from, const string& to) {
    //Returns sum of the cost of all edges for the shortest path between from and to nodes.
    //https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm 
    
    node *destination = (work.find(to)->second);
    if (destination->adj.empty()) {
        //Optimization #1: If destination node has no links, it is unreachable.  Avoid search.
        cerr << "Node " << to << " has no links.  It is unreachable." << endl;
        return -1.0;
    }
    
    double cost;
    
    // Set distance of all nodes to infinity (-1)
    for (int n=1; n<=numNodes; n++) {
        string NodeName;
        NodeName = gIntToString(n);
        node *np = (work.find(NodeName)->second);
        if (np) {
            np->totCost=-1.0;   //distance between this node and the 'from' node is not yet determined
            np->parent=NULL;    //predecessor of this node has not yet been determined 
            np->visited=false;  //mark as not visitied, or in the set of unvisited nodes
            np->current=false;
        }
    }
    
    node *source = (work.find(from)->second);
    source->totCost = 0.0; //distance of from node to itself is 0.
    source->current = true; //set source node as current
    
    std::queue<node*> Q;
    Q.push(source);    //Add source to queue, as we will explore this node first
    
    bool found = false;
    while (!Q.empty() && !found) {
        node *currNode = Q.front();
        Q.pop();
        if (currNode == destination) {
            cost = currNode->totCost;
            found = true;
        } else {
            for (auto it = currNode->adj.begin() ; it != currNode->adj.end(); ++it) {
                //For each node n that is adjacent to currNode:
                node *n = it->second;
                if (n->totCost == -1.0) {   //if node not previously visited
                    n->totCost = currNode->totCost + it->first;
                    n->parent = currNode;
                    Q.push(n);
                } //end if node not previously visited
            } //end for each adjacent node
        } //end if we have reached our destination  
    } //end while queue Q not empty
    
    if (!found) cost = -1.0;
    return cost;
}


void graph::deleteNode(node* delnode) {
    //delete all edges in adjacency list
    /*
        Edges are stored in an adjacency list in each node implemented in a STL vector or pairs.  Therefore, we should not
        need to explicitly delete the edges as they will by cleaned up by the STL when we delete the owning node (below).
    */
    
    //Finally, delete node
    delete delnode;
    numNodes--;
    return;
}

//graph class Destructor function
graph::~graph() {
    //Destructor function for graph class to delete nodes, adjacency list vector(s), and map. 
    //For all nodes
    int temp = numNodes;
    for (int n=1; n<=temp; n++) {
        string NodeName;
        NodeName = gIntToString(n);
        node *np = (work.find(NodeName)->second);
        if (np) {
            deleteNode(np);
        }
    }
    //delete map (not sure if I need to do this either, since the map (work) is an STL container.  But, it wont hurt anything.
    nmap::iterator itb=work.begin();
    nmap::iterator ite=work.begin();
    work.erase(itb,ite);
    //delete pairs?
    /*
        From: http://www.cplusplus.com/forum/general/36777/
        "STL containers generally store their elements on the free store and perform their own memory management. 
        Does anyone know, however, if one creates STL pair<T, T> objects that are not inserted into a container, 
        whether or not those objects will automatically be removed from the free store? In other words, if I create a 
        pair using pair() or make_pair(), do I need to call delete? ... Answer: No."
    */
}

string graph::gIntToString ( int Number ) {
    ostringstream ss;
    ss << Number;
    return ss.str();
}

//TODO Consider making IntToString a graph member function and overload the methods to accept int
string IntToString ( int Number ) {
    ostringstream ss;
    ss << Number;
    return ss.str();
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */ 
    //Hackerrank test harness
    int T, N, M;
    cin >> T; //Get number of test cases
    for (int testcase=1; testcase<=T; testcase++) {
        cin >> N; //Get number of nodes
        cin >> M; //Get number of edges
        
        graph G;  //Create a graph for this test case
        
        //Create all of the nodes
        for (int n=1; n<=N; n++) {
            string newNodeName;
            newNodeName = IntToString(n);
            G.addNode(newNodeName);
        }
        
        //Add all of the edges
        for (int m=1; m<=M; m++) {
            string from, to;
            cin >> from >> to;
            G.addEdge(from,to,6); //cost of all edges in hackerrank problem was 6
        }
        
        //Get starting node
        int S;
        string startName;
        cin >> S;
        startName = IntToString(S);
        
        //Find shortest path from S to all other nodes, starting with first
        for (int n=1; n<=N; n++) {
            double dCost;
            int cost;
            if (n!=S) {
                string destination;
                destination = IntToString(n);
                dCost = G.findShortestPath(startName,destination);
                cost = (int) dCost;
                cout << cost << " ";
            }
        }
        cout << endl;
        
    } //end for all test cases
    return 0;
}
