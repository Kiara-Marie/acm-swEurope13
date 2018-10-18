#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <string>
#include <stack>
#define MAXN 200
#define MAXM 1000

using namespace std;

int N;
int M;

class Place{
	public:
	int label, pred;
	double cost;
	Place(int l,double c, int p);
};

class Compare
{
public:
    bool operator() (Place a, Place b)
    {
        return a.cost > b.cost;
    }
};

Place::Place(int l,double c,int p){
	label = l;
	cost = c;
	pred = p;
}
array<vector<Place>,MAXN> shortestFrom;
vector<pair<int,double>> adj[MAXN];
vector<tuple<double,double,int>> positions(MAXN);
bool shouldAdd(pair<int,double> currEdge, vector<Place> shortest,priority_queue<Place,vector<Place>,Compare> pq);
pair<double,double> findEdge(int from, int to,string mode);
vector<Place> performQuery(int a, int b);
double eucDist(int from, int to);
int main(){
	vector<Place> v;
	fill(shortestFrom.begin(),shortestFrom.end(), v);
	cin>>N>>M;	
	int x,y,level;
	// get the positions of the places
	for (int i = 0; i < N; i++){
		cin>>level>>x>>y;
		positions[i] = tuple<double,double,int>(x,y,level);
	}
	// figure out the edges between the places
	int from,to;
	double weight;
	string mode;
	for (int i = 0; i < M; i++){
		cin>>from>>to>>mode;
		// find the weight traversing this edge forward and backward
		pair<double,double> forwardBackward = findEdge(from,to,mode);
		//cout<<"there is an edge from "<<from<<" to "<<to<<"with weight"
		//	<<forwardBackward.first<<","<<forwardBackward.second<<"\n";
		adj[from].push_back(pair<int,double>(to,forwardBackward.first));
		adj[to].push_back(pair<int,double>(from,forwardBackward.second));
	}
	// now for the queries!
	int Q;
	cin>>Q;
	int a,b;
	for(int i = 0; i< Q; i++){
		cin>>a>>b;	
		//cout<<"working on query from "<<a<<" to "<<b<<"\n";
		if (shortestFrom[a].empty()){
			vector<Place> shortest = performQuery(a,b);	
			shortestFrom[a] = shortest;
		}
		vector<Place> answer = shortestFrom[a];
		stack<int> toPrint;
		int curr = b;
		while(curr != a){
			//cout<<"traversing graph, at node "<<curr<<"\n";
			int next = answer[curr].pred;
			toPrint.push(curr);
			curr = next;

		}
		toPrint.push(curr);
		cout << endl;
		while(!toPrint.empty()){
			cout<<toPrint.top() << " ";
			toPrint.pop();
		}
	}
	cout << endl;
	return 0;
}
pair<double,double> findEdge(int from, int to,string mode){
	if (mode.compare("walking") == 0 || mode.compare("stairs") == 0){
		double dist = eucDist(from,to);
		return pair<double,double>(dist,dist);
	}
	if (mode.compare("lift") == 0){
		return pair<double,double>(1,1);
	}
	if (mode.compare("escalator") == 0){
		double dist = 3*eucDist(from,to);
		return pair<double,double>(1,dist);
	}else{
		cout<<"you scfrewed up Kiara\n";
		return pair<double,double>(0,0);
	}
}
double eucDist(int from, int to) {
	int fx,fy,fz,tx,ty,tz;
	fx = get<0>(positions[from]);
	fy = get<1>(positions[from]);
	fz = get<2>(positions[from])*5;
	tx = get<0>(positions[to]);
	ty = get<1>(positions[to]);
	tz = get<2>(positions[to])*5;
	return sqrt((fx-tx)*(fx-tx) + (fy-ty)*(fy-ty) + (fz-tz)*(fz-tz));
 }


vector<Place> performQuery(int a, int b){
	priority_queue<Place, std::vector<Place>, Compare> pq;
	Place emptyPlace(MAXN+5,0,0);
	vector<Place> shortest((vector<Place>::size_type)MAXN,emptyPlace);
	Place start(a,0,a);
	pq.push(start);
	Place currNode(a,0,a);
	while(!(pq.empty())){
		currNode = pq.top();
		pq.pop();
	//	cout<<"next thing to pop off pq is "<<currNode.label<<" , "<<currNode.cost<<"\n";
		if(shortest[currNode.label].label != currNode.label){
			shortest[currNode.label] = currNode;
		}else{
			continue;
		}
		vector<pair<int,double>> toAdd = adj[currNode.label];
		pair<int,double> currEdge; 
		while(!(toAdd.empty())){
			currEdge = toAdd.back();
			toAdd.pop_back();
			//cout<<"considering edge to"<< currEdge.first<<"\n";
			if(shouldAdd(currEdge,shortest,pq)){
				//cout<<"added edge\n";
				double currWeight = currNode.cost+currEdge.second;
				Place placeToAdd(currEdge.first,currWeight,currNode.label);
				pq.push(placeToAdd);
			}else{
	//			cout<<"did not add edge\n";
			}
		}
	}
	return shortest;
}

bool shouldAdd(pair<int,double> currEdge, vector<Place> shortest,priority_queue<Place,vector<Place>,Compare> pq){
	vector<Place>::iterator it;
	if(currEdge.first == 0 ||currEdge.first == 4){
		//cout<<"edge to 0 has weight"<<currEdge.second<<"\n";
	}
	for(it = shortest.begin(); it <shortest.end(); it++){
		if(it->label == currEdge.first){
			//cout<<"edge was already in shortest\n";
			return false;
		}
	}
	while(!(pq.empty())){
		if(pq.top().label == currEdge.first){
			if(pq.top().cost <= currEdge.second){
				return false;
			}
			return  true;
		}
		pq.pop();
	}
	return true;
} 
	


