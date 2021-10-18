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
      if (chmin(d[nxt], d[s] + 1))
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

int simulate(const std::vector<int> &node_list, std::vector<int> &node_val, int center = 0)
{
  int node_val_cur_list[2] = {center, center};
  for (const auto &from : node_list)
  {
    if (m <= 20000)
    {
      node_val_cur_list[0] = center, node_val_cur_list[1] = center;
    }
    assert(node_val[from] == -1);
    while (true)
    {
      bool action = false;
      bool node_val_set = false;
      for (int kkt = 0; kkt < 2; ++kkt)
      {
        auto &node_val_cur = node_val_cur_list[kkt];
        if (node_val_cur > max_val_min or node_val_cur < 0)
        {
          continue;
        }
        action = true;
        if (used_node_val[node_val_cur])
        {
          node_val_cur += (kkt ? 1 : -1);
          continue;
        }
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
            node_val_ok = false;
            break;
          }
          used_edge_val[diff] = true;
        }
        if (node_val_ok)
        {
          node_val_set = true;
          assert(not used_node_val[node_val_cur]);
          used_node_val[node_val_cur] = true;
          node_val[from] = node_val_cur;
          node_val_cur += (kkt ? 1 : -1);
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
        node_val_cur += (kkt ? 1 : -1);
      }
      if (not action)
        return inf;
      if (node_val_set)
        break;
    }
  }
  return *std::max_element(node_val.begin(), node_val.end());
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

void used_val_from_node_val(const std::vector<int> &node_val)
{
  for (int from = 0; from < n; ++from)
  {
    used_node_val[node_val[from]] = true;
    for (const auto &to : graph[from])
    {
      const int diff = std::abs(node_val[from] - node_val[to]);
      used_edge_val[diff] = true;
    }
  }
}
void generate_erase_node_val_list(int erase_cnt, std::vector<int> &node_list, const std::vector<int> &node_val, std::vector<int> &d, std::queue<int> &q)
{
  d.assign(n, inf);
  if (node_list.empty())
  {
    const int s = std::max_element(node_val.begin(), node_val.end()) - node_val.begin();
    d[s] = 0;
    q.emplace(s);
    erase_cnt -= 1;
    node_list.emplace_back(s);
  }
  else
  {
    for (const auto &e : node_list)
    {
      d[e] = 0;
      q.emplace(e);
      erase_cnt -= 1;
    }
  }
  while (not q.empty())
  {
    const int cur = q.front();
    q.pop();
    for (const auto &nxt : graph[cur])
    {
      if (chmin(d[nxt], d[cur] + 1) and erase_cnt > 0)
      {
        node_list.emplace_back(nxt);
        q.emplace(nxt);
        erase_cnt -= 1;
      }
    }
  }
}
int main()
{
  auto start = std::chrono::system_clock::now();
  input();
  // const int center = find_center();
  std::vector<int> d(n);
  std::queue<int> q;
  // (void)bfs(center, d, q);
  std::vector<int> node_list(n);
  std::iota(node_list.begin(), node_list.end(), 0);
  std::shuffle(node_list.begin(), node_list.end(), rnd);
  // std::sort(node_list.begin(), node_list.end(), [&](int i, int j)
  //           { return d[i] < d[j]; });
  std::vector<int> node_val(n, -1);
  max_val_min = simulate(node_list, node_val);
  auto ret = node_val;
  for (int simulate_count = 0;; ++simulate_count)
  {
    if (simulate_count == 1)
    {
      simulate_count = 0;
      const auto end = std::chrono::system_clock::now();
      const double time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
      if (time > 8000)
        break;
    }
    const int node1 = xor64() % n;
    const int node2 = xor64() % n;
    std::swap(node_list[node1], node_list[node2]);
    erase_node_val(node_list, node_val);
    const int max_val = simulate(node_list, node_val, max_val_min / 2);
    if (chmin(max_val_min, max_val) or max_val_min == max_val)
    {
      ret = node_val;
    }
    else
    {
      std::swap(node_list[node1], node_list[node2]);
    }
  }
  // for (int i = 0; i <= max_val_min; ++i)
  // {
  //   used_node_val[i] = false;
  //   used_edge_val[i] = false;
  // }
  // node_val = ret;
  // used_val_from_node_val(node_val);
  // node_list.clear();
  // for (int simulate_count = 0;; ++simulate_count)
  // {
  //   if (simulate_count == 5)
  //   {
  //     simulate_count = 0;
  //     const auto end = std::chrono::system_clock::now();
  //     const double time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  //     if (time > 9000)
  //       break;
  //   }
  //   generate_erase_node_val_list(1 << bit_shift, node_list, node_val, d, q);
  //   erase_node_val(node_list, node_val);
  //   // std::reverse(node_list.begin(), node_list.end());
  //   const int max_val = simulate(node_list, node_val);
  //   if (chmin(max_val_min, max_val))
  //   {
  //     ret = node_val;
  //     bit_shift = 0;
  //     node_list.clear();
  //   }
  //   else
  //   {
  //     if ((1 << bit_shift) >= n)
  //       break;
  //     bit_shift += 1;
  //   }
  // }
  for (int i = 0; i < n; ++i)
  {
    cout << ret[i] << " \n"[i + 1 == n];
  }
}