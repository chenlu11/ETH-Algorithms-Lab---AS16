/*
http://www.cs.yale.edu/homes/aspnes/pinewiki/DepthFirstSearch.html

The starting and finishing times for each node display a property called the parenthesis property. 
This property means that all the children of a particular node in the depth first tree have a later discovery time and an earlier finish time than their parent.

Depth-first search in a tree
We can compute start and end times for each node. Here clock is a global variable initialized to zero.

TreeTimes(root):
  start[root] = clock; clock = clock + 1
  for each child of root:
    TreeTimes(child)
  end[root] = clock; clock = clock + 1

These times can be used to quickly test if one node is a descendant of another. 
The rule is that if x is a descendant of y, then start[y] < start[x] < end[x] < end[y]. 
This gives another picture of the tree, where each node is drawn as an interval.
*/
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct Tree {
    int id;
    int age;
    int parent = -1;
    int start;
    int end;
    int height;
    string name;
    vector<Tree*> child;
};

void bfs(Tree& tree, int& clock, int height, vector<vector<Tree*> >& buckets)
{
    tree.start = clock;
    clock++;
    for (Tree* childtree: tree.child) 
        bfs(*childtree, clock, height + 1, buckets);
    tree.end = clock;
    clock++;
    // cout << tree.name << "\tage: " << tree.age << "\tstart: " << tree.start << "\tend: " << tree.end << "\theight: " << height << endl;
    if (buckets.size() < height + 1)
        buckets.resize(height + 1);
    tree.height = height;
    buckets[height].push_back(&tree);
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for (int i = 0; i < t; i++)
    {
        int n, q; cin >> n >> q;
        map<string, int> indexMap;
        string s, s1, s2;
        int cur_age;
        vector<Tree> tree(n);
        for (int j = 0; j < n; j++)
        {
            cin >> s >> cur_age;
            indexMap[s] = j;
            tree[j].id = j;
            tree[j].age = cur_age;
            tree[j].name = s;            
        }
        for (int j = 0; j < n - 1; j++)
        {
            cin >> s1 >> s2;
            int id1 = indexMap[s1], id2 = indexMap[s2];
            tree[id1].parent = id2;
            tree[id2].child.push_back(&tree[id1]);
        }

        int root;
        for (int j = 0; j < n; j++)
        {
            if (tree[j].parent == -1)
            {
                root = j;
                break;
            }
        }
        int clock = 0;
        vector<vector<Tree*> > buckets;
        bfs(tree[root], clock, 0, buckets);

        // Two layer of binary search
        int cur_species;
        for (int j = 0; j < q; j++)
        {
            cin >> s >> cur_age;
            cur_species = indexMap[s];

            int height_low = 0, height_high = tree[cur_species].height;
            int bucket_low, bucket_high;
            while (true)
            {
                int height_mid = (height_low + height_high) / 2;
                // first find the parent in height height_mid
                bucket_low = 0;
                bucket_high = buckets[height_mid].size() - 1;
                while (bucket_low < bucket_high)
                {
                    int bucket_mid = (bucket_low + bucket_high) / 2;
                    if (buckets[height_mid][bucket_mid]->start > tree[cur_species].start)
                        bucket_high = bucket_mid - 1;
                    else
                        bucket_low = bucket_mid + 1;
                }
                if (height_low == height_high)
                    break;
                if (buckets[height_mid][bucket_low]->age > cur_age)
                    height_low = height_mid + 1;
                else
                    height_high = height_mid;    
            }
            cout << buckets[height_low][bucket_low]->name + " ";
        }
        cout << '\n';
    }
    return 0;
}

/*
1
8 3
luca 10
bacteria 7
archaea 8
eukaryota 7
aquifex 3
plants 4
animals 6
thermotoga 1
bacteria luca 
archaea luca 
eukaryota luca 
aquifex bacteria 
plants eukaryota 
animals eukaryota 
thermotoga aquifex 
thermotoga 4 
thermotoga 2 
plants 8

*/