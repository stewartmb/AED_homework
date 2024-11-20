class MinHeap {
    int _size = 0;
    vector<pair<int,int>> v = {{-1,-1}}; //elements start at index 1\\\

    int p(int i){return i >> 1;}// i / 2
    int r(int i){return i << 1;} // i * 2
    int l(int i){return (i << 1) + 1;}  //i * 2 + 1

public:
    bool empty() const {
        return _size == 0;
    }
    pair<int,int> getMin() {
        return v[1];
    }

    void shiftUp(int i) {
        if(i > _size) return; //shifting item that doesnt exist
        if(i == 1) return;
        if(v[i].second < v[p(i)].second) swap(v[p(i)],v[i]); // if parent greater than child, swap
        shiftUp(p(i));
    }

    pair<int,int> popMin() {
        pair<int,int> min = v[1];
        swap(v[1],v[_size--]); //swap with last and decrease size
        shiftDown(1); //let shift down reorganize
        return min;//return max
    }

    void shiftDown(int i) {
        if(i > _size) return; //shifting i that doesnt exist
        int swapId = i;
        if(l(i) <= _size && v[i].second > v[l(i)].second) swapId = l(i);
        //if left child exists and smaller than parent then swap left
        if(r(i) <= _size && v[swapId].second > v[r(i)].second) swapId = r(i);
        //if right exists and its smaller than current smaller then swap right
        if(swapId != i) {
            swap(v[i],v[swapId]); //swap values of child and parent, after this parent will be smaller than both
            shiftDown(swapId);
        }
        return;
    }

    void insert(pair<int,int> e) {
        if(_size+1 >= v.size()) { //if heap has correct  size
            v.push_back({0,0});
        }
        v[++_size] = e; //
        shiftUp(_size);
        return;
    }
    pair<int,int> kthmin(int k) {
        if(k > _size) return {-1,-1};
        vector<pair<int,int>> save;
        while(--k) {
            save.push_back(this->popMin());
        }
        pair<int,int> ans = this->getMin();
        for(auto e: save) {
            this->insert(e);
        }
        return ans;
    }
    void print() {
        for(auto [n,w]: v) {
            if(n == -1) continue;
            cout<<n<<" "<<w<<endl;
        }
    }
    MinHeap(){}
};

class Solution {
public:
    int minimumEffortPath(vector<vector<int>>& heights) {
        int m = heights.size();
        int n = heights[0].size();
        vector<vector<pair<int, int>>> adj(m * n + 1);
        int e = 1;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (j < n - 1) {
                    adj.at(e).push_back(make_pair(
                            e + 1, abs(heights[i][j + 1] - heights[i][j])));
                    adj.at(e+1).push_back(
                            make_pair(e, abs(heights[i][j] - heights[i][j + 1])));
                }
                if (i < m - 1) {
                    adj.at(e).push_back(make_pair(
                            n + e, abs(heights[i + 1][j] - heights[i][j])));
                    adj.at(e+n).push_back(
                            make_pair(e, abs(heights[i][j] - heights[i + 1][j])));
                }
                e++;
            }
        }
        return dijkstra(adj,1);
    }

    int dijkstra(vector<vector<pair<int,int>>> adj, int src){
        vector<int> shortest(adj.size(),-1);
        MinHeap heap;
        int min = INT_MAX;
        int max = INT_MIN;
        heap.insert({src,0});
        while(!heap.empty()){
            auto [n,w] = heap.popMin();
            if (shortest[n] != -1) continue; //not do more because no negative edges
            shortest[n] = w;
            max = max < w ? w: max;
            if(n == adj.size()-1) return max;
            for(auto [node, wt]: adj[n]){
                if(shortest.at(node) == -1){
                    cout<<"Nodos para "<<n<<": "<<node<<",peso: "<<wt<<endl;
                    heap.insert({node,wt});
                }
            }
        }
        return max;
    }

    int getdiff(vector<vector<pair<int,int>>> adj, int n, int _n){
        for(auto [node, wt]: adj[n]){
            if(node == _n){
                return wt;
            }
        }
        return -1;
    }
};