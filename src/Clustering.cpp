#include "Clustering.h"

void Clustering::run() //Wooju
{
    cout << "Haha" << endl;
}

void Clustering::writeDistanceMatrixToFile()
{
    string path = "../python-tools/";
    std::ofstream output;
    output.open(path + "distances.csv", std::ios::out);
    for (int i = 0; i < num_of_agents; i++)
    {
        for (int j = 0; j < num_of_agents - 1; j++)
        {
            output << getDistance(i, j) << ",";
        }
        output << getDistance(i, num_of_agents - 1) << endl;
    }
    output.close();
    output.open(path + "agents.csv", std::ios::out);
    output << "start,goal" << endl;
    for (int i = 0; i < num_of_agents; i++)
    {
        output << getStartVertex(i) << "," << getFirstGoalVertex(i) << endl;
    }
    output.close();
    cout << (double)(clock() - start_time) / CLOCKS_PER_SEC << "s" << endl;
}

int Clustering::getDistance(int a1, int a2) // Jiaoyang will do this
{
    if (distances[a1][a2] >= 0)
        return distances[a1][a2];

    assert(mdds[a1]->levels.size() == mdds[a2]->levels.size());
    int min_distance = INT_MAX;
    for (int t = 0; t < (int)mdds[a1]->levels.size(); t++)
    {
        for (const auto& node1: mdds[a1]->levels[t])
        {
            for (const auto& node2: mdds[a2]->levels[t])
            {
                int distance;
                if (lookahead == 0)
                    distance = G.get_Manhattan_distance(node1.location, node2.location);
                else if (node1.location == node2.location)
                    distance = node1.cost + node2.cost;
                else
                    distance = 3 * lookahead;
                min_distance = min(min_distance, distance);
                if (min_distance == 0)
                {
                    distances[a1][a2] = 0;
                    distances[a2][a1] = 0;
                    return 0;
                }
            }
        }
    }
    distances[a1][a2] = min_distance;
    distances[a2][a1] = min_distance;
    return min_distance;
}


void Clustering::updateLocations(const vector<State>& starts,
                                 const vector< vector<pair<int, int> > >& goal_locations)
{
    start_time = clock();
    num_of_agents = (int)starts.size();
    landmarks.resize(num_of_agents);
    mdds.resize(num_of_agents);
    distances.resize(num_of_agents, vector<int>(num_of_agents, -1));
    for (int i = 0; i < num_of_agents; i++)
    {
        landmarks[i].resize(goal_locations[i].size() + 1);
        landmarks[i][0] = starts[i].location;
        for (int j = 0; j < (int)goal_locations[i].size(); j++)
        {
            landmarks[i][j + 1] = goal_locations[i][j].first;
        }
        mdds[i] = mdd_helper.getMDD(landmarks[i]);
    }
}