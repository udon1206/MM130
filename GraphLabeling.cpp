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

int n;
int m;
std::vector<std::vector<int>> graph;

const int MAX_NODE_VAL = 2085044;
bool edge_val[MAX_NODE_VAL + 1];
int bfs(const int s, std::vector<int> &d, std::queue<int> &q)
{
  int last = -1;
  d.assign(n, inf);
  q.emplace(s);
  d[s] = 0;
  while (not q.empty())
  {
    const int cur = q.front();
    q.pop();
    last = cur;
    for (const auto &nxt : graph[cur])
    {
      if (graph[cur][nxt] and chmin(d[nxt], d[s] + 1))
      {
        q.emplace(nxt);
      }
    }
  }
  assert(last >= 0);
  return last;
}

int find_center()
{
  std::vector<int> d(n, inf);
  std::queue<int> q;
  const int end_point1 = bfs(0, d, q);
  const int end_point2 = bfs(end_point1, d, q);
  const int diameter = d[end_point2];
  int center = -1;
  for (int i = 0; i < n; ++i)
  {
    if (d[i] == diameter / 2)
      center = i;
  }
  assert(center >= 0);
  return center;
}

void input()
{
  cin >> n >> m;
  graph.resize(n);
  for (int i = 0; i < m; ++i)
  {
    int s, t;
    cin >> s >> t;
    graph[s].emplace_back(t);
    graph[t].emplace_back(s);
  }
}

bool simulate(const std::vector<int> &node_list, std::vector<int> &node_val, std::vector<int> &used_edge_val)
{
  for (const auto &e : used_edge_val)
    edge_val[e] = false;
  used_edge_val.clear();
  node_val.clear();
  node_val.resize(n, -1);
  int node_val_cur = 0;
  for (const auto &from : node_list)
  {
    while (true)
    {
      if (node_val_cur > MAX_NODE_VAL)
      {
        return false;
      }
      bool node_val_ok = true;
      for (const auto &to : graph[from])
      {
        if (node_val[to] == -1)
          continue;
        const int diff = std::abs(node_val_cur - node_val[to]);
        if (edge_val[diff])
        {
          node_val_ok = false;
          break;
        }
      }
      if (node_val_ok)
        break;
      node_val_cur += 1;
    }
    node_val[from] = node_val_cur;
    for (const auto &to : graph[from])
    {
      if (node_val[to] == -1)
        continue;
      const int diff = std::abs(node_val_cur - node_val[to]);
      if (not edge_val[diff])
      {
        edge_val[diff] = true;
        used_edge_val.emplace_back(diff);
      }
    }
    node_val_cur += 1;
  }
  return true;
}
int main()
{
  input();
  const int center = find_center();
  std::vector<int> d(n);
  std::queue<int> q;
  (void)bfs(center, d, q);
  std::vector<int> node_list(n);
  std::iota(node_list.begin(), node_list.end(), 0);
  std::sort(node_list.begin(), node_list.end(), [&](int i, int j)
            { return d[i] < d[j]; });
  std::vector<int> node_val, used_edge_val;
  std::cerr << simulate(node_list, node_val, used_edge_val) << endl;
  for (int i = 0; i < n; ++i)
  {
    cout << node_val[i] << " \n"[i + 1 == n];
  }
}