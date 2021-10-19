#include <bits/stdc++.h>

using ll = long long;
using std::cin;
using std::cout;
using std::endl;
std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
template <class T>
inline bool chmax(T &a, T b)
{
  if (a < b)
  {
    a = b;
    return 1;
  }
  return 0;
}
template <class T>
inline bool chmin(T &a, T b)
{
  if (a > b)
  {
    a = b;
    return 1;
  }
  return 0;
}
const int inf = (int)1e9 + 7;
const long long INF = 1LL << 60;

uint32_t xor64(void)
{
  static uint64_t x = 88172645463325252ULL;
  x = x ^ (x << 13);
  x = x ^ (x >> 7);
  return x = x ^ (x << 17);
}

int n;
int m;
std::vector<std::vector<int>> graph;
int max_val_min = inf;
const int MAX_NODE_VAL = 2085044;
bool used_edge_val[MAX_NODE_VAL + 1];
bool used_node_val[MAX_NODE_VAL + 1];
using bs = std::bitset<500>;
bs bad_node;
std::vector<bs> bitset_graph;
void input()
{
  cin >> n >> m;
  graph.resize(n);
  bitset_graph.resize(n);
  for (int i = 0; i < m; ++i)
  {
    int s, t;
    cin >> s >> t;
    bitset_graph[s][t] = bitset_graph[t][s] = true;
    graph[s].emplace_back(t);
    graph[t].emplace_back(s);
  }
}

int simulate(std::vector<int> node_list, std::vector<int> &node_val)
{
  int node_val_cur = 0;
  for (; not node_list.empty(); ++node_val_cur)
  {
    bad_node.reset();
    for (auto itr = node_list.begin(); itr != node_list.end(); itr++)
    {
      const int from = *itr;
      if ((bad_node & bitset_graph[from]).count() > 0)
        continue;
      bool node_val_ok = true;
      int graph_idx = 0;
      for (; graph_idx < (int)graph[from].size(); graph_idx++)
      {
        const int to = graph[from][graph_idx];
        if (node_val[to] == -1)
          continue;
        const int diff = std::abs(node_val_cur - node_val[to]);
        if (used_edge_val[diff])
        {
          bad_node[to] = true;
          node_val_ok = false;
          break;
        }
        used_edge_val[diff] = true;
      }
      if (node_val_ok)
      {
        node_val[from] = node_val_cur;
        (void)node_list.erase(itr);
        break;
      }
      else
      {
        for (int i = 0; i < graph_idx; ++i)
        {
          const int to = graph[from][i];
          if (node_val[to] == -1)
            continue;
          const int diff = std::abs(node_val_cur - node_val[to]);
          used_edge_val[diff] = false;
        }
      }
    }
  }
  return node_val_cur;
}

void erase_node_val(const std::vector<int> &node_list, std::vector<int> &node_val)
{
  for (const auto &from : node_list)
  {
    if (node_val[from] == -1)
      continue;
    for (const auto &to : graph[from])
    {
      if (node_val[to] == -1)
        continue;
      const int diff = std::abs(node_val[to] - node_val[from]);
      used_edge_val[diff] = false;
    }
    used_node_val[node_val[from]] = false;
    node_val[from] = -1;
  }
}
int main()
{
  auto start = std::chrono::system_clock::now();
  input();
  std::vector<int> node_list(n);
  std::iota(node_list.begin(), node_list.end(), 0);
  std::sort(node_list.begin(), node_list.end(), [&](int i, int j)
            { return graph[i].size() > graph[j].size(); });
  std::vector<int> node_val(n, -1);
  double one_exe_time = 0;
  {
    auto st = std::chrono::system_clock::now();
    max_val_min = simulate(node_list, node_val);
    auto ed = std::chrono::system_clock::now();
    one_exe_time = std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count();
  }
  auto ret = node_val;
  for (int simulate_count = 0;; ++simulate_count)
  {
    if (simulate_count == 1)
    {
      simulate_count = 0;
      const auto end = std::chrono::system_clock::now();
      const double time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
      if (time + one_exe_time * 2 > 9000)
        break;
    }
    const int node1 = xor64() % n;
    const int node2 = xor64() % n;
    std::swap(node_list[node1], node_list[node2]);
    erase_node_val(node_list, node_val);
    const int max_val = simulate(node_list, node_val);
    if (chmin(max_val_min, max_val) or max_val_min == max_val)
    {
      ret = node_val;
    }
    else
    {
      std::swap(node_list[node1], node_list[node2]);
    }
  }

  for (int i = 0; i < n; ++i)
  {
    cout << ret[i] << " \n"[i + 1 == n];
  }
}