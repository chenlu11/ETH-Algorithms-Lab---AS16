#include <iostream>
#include <queue>
#include <vector>
#include <climits>
using namespace std;

struct Edge
{
    int target;
    int weight;
    Edge(int t, int w): target(t), weight(w) {}
};

struct Node 
{
    int id;
    int weight;
    bool step_flag; // true: odd, false: even
    Node(int i, int w, bool flag): id(i), weight(w), step_flag(flag) {}
};

int main()
{
    ios_base::sync_with_stdio(false);
    int testcases; cin >> testcases;
    while (testcases--)
    {
        int n, m, s, t;
        cin >> n >> m >> s >> t;
        vector<vector<Edge> > adjacency_list(n);
        for (int i = 0; i < m; i++)
        {
            int si, ti, wi;
            cin >> si >> ti >> wi;
            adjacency_list[si].push_back(Edge(ti, wi));
        }

        vector<int> even_edge_even_weight(n, INT_MAX); // the smallest EVEN weight from s to each node with EVEN steps
        vector<int> even_edge_odd_weight(n, INT_MAX); // the smallest ODD weight from s to each node with EVEN steps;
        vector<int> odd_edge_even_weight(n, INT_MAX); // the smallest EVEN weight from s to each node with ODD steps;
        vector<int> odd_edge_odd_weight(n, INT_MAX); // the smallest ODD weight from s to each node with ODD steps;

        queue<Node> Q;
        Q.push(Node(s, 0, false));
        even_edge_even_weight[s] = 0;
        while (!Q.empty())
        {
            Node node = Q.front();
            Q.pop();
            int current = node.id;
            for (int i = 0; i < adjacency_list[current].size(); i++)
            {
                Edge edge = adjacency_list[current][i];
                int target = edge.target;
                int path_weight = node.weight + edge.weight;
                if (node.step_flag) 
                {
                    if (path_weight % 2 == 0)
                    {
                        if (path_weight < even_edge_even_weight[target])
                        {
                            even_edge_even_weight[target] = path_weight;
                            Q.push(Node(target, path_weight, false));
                        }
                    }
                    else
                    {
                        if (path_weight < even_edge_odd_weight[target])
                        {
                            even_edge_odd_weight[target] = path_weight;
                            Q.push(Node(target, path_weight, false));
                        }
                    }
                }
                else
                {
                    if (path_weight % 2 == 0)
                    {
                        if (path_weight < odd_edge_even_weight[target])
                        {
                            odd_edge_even_weight[target] = path_weight;
                            Q.push(Node(target, path_weight, true));
                        }
                    }
                    else
                    {
                        if (path_weight < odd_edge_odd_weight[target])
                        {
                            odd_edge_odd_weight[target] = path_weight;
                            Q.push(Node(target, path_weight, true));
                        }
                    }
                }
            }
        }
        if (odd_edge_odd_weight[t] == INT_MAX)
            cout << "no" << endl;
        else
            cout << odd_edge_odd_weight[t] << endl;

    }
}