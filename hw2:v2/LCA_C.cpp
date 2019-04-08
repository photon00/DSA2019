#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
#include <algorithm>

#define OFFSET 0x61


typedef struct{
	int   				level;
	std::vector<int>	children;
} Node;

std::vector<int>  num_idx;  // mapping from string index to node index
std::vector<Node> nodes;
std::vector<int>  childrens[200001];
int ancestors[200001][20];  // build Jump Pointer Algorithm 2^i ancestors

int n, N;

void insert(int parent_num, char c){
	int parent_idx = num_idx[parent_num];
	if (nodes[parent_idx].children[c-OFFSET] != 0){
		num_idx.push_back(nodes[parent_idx].children[c-OFFSET]);
	}
	else {
		std::vector<int> children;
		Node new_node = {nodes[parent_idx].level+1, children};
		new_node.children.assign(26, 0);

		nodes.push_back(new_node);
		num_idx.push_back(nodes.size()-1);
		nodes[parent_idx].children[c-OFFSET] = nodes.size()-1;
		childrens[parent_idx].push_back(nodes.size()-1);
		ancestors[nodes.size()-1][0] = parent_idx;
	}
}

void dfs(int x){
	for (int i=1; i<=N; ++i){
		ancestors[x][i] = ancestors[ancestors[x][i-1]][i-1];
	}
	for (int i=0; i<childrens[x].size(); ++i){
		int child_idx = childrens[x][i];
		dfs(child_idx);
	}
}

void init(){
	N = (int)ceil(log2(n));
	dfs(0);
}

int lca(int a, int b){
	if (nodes[a].level > nodes[b].level) std::swap(a, b);
	
	int nodeA_level = nodes[a].level;
	for (int i=(int)log2(nodes[b].level - nodeA_level)+1; i>=0; --i){
		if (nodes[b].level > nodeA_level && nodes[ancestors[b][i]].level >= nodeA_level){
			b = ancestors[b][i];
		}
	}
	for (int i=(int)log2(nodes[a].level)+1; i>=0; --i){
		if (ancestors[a][i] != ancestors[b][i]){
			a = ancestors[a][i], b = ancestors[b][i];
		}
	}
	if (a != b) return nodes[ancestors[a][0]].level;
	return nodes[a].level;
}

int main(){
	using namespace std;
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	cin >> n;
	num_idx.reserve(n);
	num_idx.push_back(0);
	
	nodes.reserve(n);
	vector<int> children;
	Node root = {0, children};
	root.children.assign(26, 0);
	nodes.push_back(root);
	
	int parent_num;
	char c;
	for (int i=0; i<n; ++i){
		cin >> parent_num >> c;
		insert(parent_num, c);
	}
	init();
	
	int q, a, b;
	cin >> q;
	for (int i=0; i<q; ++i){
		cin >> a >> b;
		cout << lca(num_idx[a], num_idx[b]) << '\n';
	}
	return 0;
}




