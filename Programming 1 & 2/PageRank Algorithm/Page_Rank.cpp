

#include <iostream>
#include <map>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
using namespace std;
class AdjacencyList
{
private:
	// Think about what member variables you need to initialize
	map<string, int> outDegreeGraph;
	map<string, vector<pair<string, float>>> graph; // where the strings are the verticy name and the flat is the ranking
	map<string, float> pageRanks;
	map<string, float> newRanks;

public:
	// Think about what helper functions you will need in the algorithm
	void PageRank();
	void insertPair(string from, string to);
	void printGraphStrings();
	void getAdjacent(string from);
	void updateLinks();
	void updateRanks();
	void updateNewRanks();
	void printOutput();
	void outDegrees();
};
void AdjacencyList::PageRank()
{
	// power iteration recalculates the RANK of each function;; hence i must edit or update the pageRanks map!!!
	// the new page rank is calculated by.... a summation of the multiplicationn of the 'to' values page rank and out degree!
	map<string, vector<pair<string, float>>>::iterator it1;
	for (it1 = graph.begin(); it1 != graph.end(); it1++)
	{
		float newRankVal = 0;
		for (int i = 0; i < graph[it1->first].size(); i++)
		{
			float outDegree = graph[it1->first][i].second;
			// get ranking of the VECTOR STRING- NOT the string of the to value but the from!
			float ranking = pageRanks[graph[it1->first][i].first];
			// calculate new page rank!
			newRankVal += outDegree * ranking;
			// after the loop is over, all the values will be summed and must then be placed in a map to later update the rank of the main domain
		}
		newRanks[it1->first] = newRankVal;
		// putting it in a new map makes it so the previous rank value can still be used for the other websites that must be calculated with teh origgianl rank before updating it!
	}
	// after the for loop is complete, you must then update the pageRanks map (that which contians the previous page ranks) with the newRanks so when this function is called AGAIN (p times), it will take the correct values! <3
	updateNewRanks();
}
void AdjacencyList::insertPair(string from, string to)
{
	if (outDegreeGraph.find(from) != outDegreeGraph.end())
	{
		// cout << "ALREADY EXISTS" << outDegreeGraph[from] << endl;
		// already exists in the map
		outDegreeGraph[from]++;
		if (outDegreeGraph.find(to) == outDegreeGraph.end())
		{
			outDegreeGraph[to] = {};
		}
	}
	else
	{
		outDegreeGraph.insert({from, 1});
		if (outDegreeGraph.find(to) == outDegreeGraph.end())
		{
			outDegreeGraph[to] = {};
		}
	}
	// Create pairGraph
	graph[to].push_back(make_pair(from, 1)); // MUST UPDATE THE FLOAR BASED ON PAGE RANK?!?!?!
	if (graph.find(from) == graph.end())
	{
		graph[from] = {};
	}
}
void AdjacencyList::updateLinks()
{
	// get currecnt size of the map (number of verticies)
	map<string, vector<pair<string, float>>>::iterator it;
	for (it = graph.begin(); it != graph.end(); it++)
	{
		// update ranking to 1/current size (at the end it will be 1/all vertecies)
		for (int i = 0; i < graph[it->first].size(); i++)
		{
			string domain = graph[it->first][i].first;
			graph[it->first][i].second = (float)1 / outDegreeGraph[domain];
		}
	}
}
void AdjacencyList::updateRanks()
{
	float rank = (float)1 / graph.size();
	map<string, int>::iterator it;
	for (it = outDegreeGraph.begin(); it != outDegreeGraph.end(); it++)
	{
		pageRanks[it->first] += rank;
	}
}
void AdjacencyList::updateNewRanks()
{
	map<string, float>::iterator it;
	for (it = pageRanks.begin(); it != pageRanks.end(); it++)
	{
		// just simply replace the values!!!
		pageRanks[it->first] = newRanks[it->first];
	}
}
// prints the PageRank of all pages after p powerIterations in ascending alphabetical order of webpagesand rounding rank to two decimal places
void AdjacencyList::printOutput()
{
	map<string, float>::iterator it;
	for (it = pageRanks.begin(); it != pageRanks.end(); it++)
	{
		cout << it->first << " " << fixed << setprecision(2) << it->second << endl;
	}
}
// This class and method are optional. To accept the input, you can use this method:
int main()
{
	int no_of_lines, power_iterations;
	std::string from, to;
	string line;
	std::cin >> no_of_lines;
	std::cin >> power_iterations;
	getline(cin, line);
	AdjacencyList Created_Graph;
	for (int i = 0; i < no_of_lines; i++)
	{
		getline(cin, line);
		istringstream in(line);
		in >> from;
		in >> to;
		// Add verticies/ edge(from->to) to adjacency lsit STRING, VECTOR PAIRS)
		Created_Graph.insertPair(from, to);
	}
	// now make graph based on the final map
	Created_Graph.updateRanks();
	Created_Graph.updateLinks();
	// because when power iterations is 0, you print out the INITIAL rnakings, i have i=1 <3
	for (int i = 1; i < power_iterations; i++)
	{
		Created_Graph.PageRank();
	}
	Created_Graph.printOutput();
	return 0;
}