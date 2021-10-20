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

struct MyEasyBitset
{
  unsigned long long S1, S2;
  MyEasyBitset() : S1(0), S2(0) {}

  void set(int idx, bool val)
  {

    if (idx >= 64)
    {
      if (val)
      {
        S2 = S2 | (1LL << (idx - 64));
      }
      else
      {
        if (S2 >> (idx - 64) & 1)
        {
          S2 ^= (1LL << (idx - 64));
        }
      }
    }
    else
    {
      if (val)
      {
        S1 = S1 | (1LL << idx);
      }
      else
      {
        if (S1 >> idx & 1)
        {
          S1 ^= (1LL << idx);
        }
      }
    }
  }

  void reset()
  {
    S1 = S2 = 0;
  }

  bool get(int idx) const
  {
    if (idx >= 64)
      return S2 >> (idx - 64) & 1;
    else
      return S1 >> idx & 1;
  }

  bool and_any(const MyEasyBitset &e) const
  {
    return ((S1 & e.S1) > 0) or ((S2 & e.S2) > 0);
  }
};

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
std::vector<MyEasyBitset> my_easy_bitset_graph;
std::vector<MyEasyBitset> bad_nodes;
void input()
{
  cin >> n >> m;
  graph.resize(n);
  bitset_graph.resize(n);
  if (n < 120)
    my_easy_bitset_graph.resize(n);
  for (int i = 0; i < m; ++i)
  {
    int s, t;
    cin >> s >> t;
    bitset_graph[s][t] = bitset_graph[t][s] = true;
    if (n < 120)
    {
      my_easy_bitset_graph[s].set(t, 1);
      my_easy_bitset_graph[t].set(s, 1);
    }
    graph[s].emplace_back(t);
    graph[t].emplace_back(s);
  }
}

int simulate1(std::vector<int> node_list, std::vector<int> &node_val)
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

int simulate2(const std::vector<int> &node_list, std::vector<int> &node_val)
{
  int node_val_cur = 0;
  for (const auto &from : node_list)
  {
    node_val_cur = 0;
    assert(node_val[from] == -1);
    while (true)
    {
      while (used_node_val[node_val_cur])
      {
        node_val_cur += 1;
      }
      if (node_val_cur > max_val_min)
      {
        return inf;
      }
      if (bad_nodes[node_val_cur].and_any(my_easy_bitset_graph[from]))
      {
        node_val_cur += 1;
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
          bad_nodes[node_val_cur].set(to, 1);
          node_val_ok = false;
          break;
        }
        used_edge_val[diff] = true;
      }
      if (node_val_ok)
        break;
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
      node_val_cur += 1;
    }
    assert(not used_node_val[node_val_cur]);
    used_node_val[node_val_cur] = true;
    node_val[from] = node_val_cur;
    node_val_cur += 1;
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

struct State
{
  int n;
  int used_node_val_count;
  std::vector<int> node_val;
  std::bitset<10000> used_edge_val;
  State(int n) : n(n), used_node_val_count(0)
  {
    node_val.assign(n, -1);
    used_edge_val.reset();
  }

  bool operator<(const State &s) const
  {
    return used_node_val_count < s.used_node_val_count;
  }

  bool operator>(const State &s) const
  {
    return used_node_val_count > s.used_node_val_count;
  }
};
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
  int exe_simulate_function = 1;
  {
    auto st = std::chrono::system_clock::now();
    max_val_min = simulate1(node_list, node_val);
    auto ed = std::chrono::system_clock::now();
    one_exe_time = std::chrono::duration_cast<std::chrono::milliseconds>(ed - st).count();
  }

  auto ret = node_val;
  if (max_val_min < 10000 and n < 120)
  {
    const int beam_width = 1000;
    const int transition = 5;
    MyEasyBitset bad_node;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq, npq;
    pq.emplace(State(n));
    State st(n);
    for (int node_val_cur = 0;; ++node_val_cur)
    {
      // std::cerr << node_val_cur << " " << pq.top().used_node_val_count << "\n";
      while (not pq.empty())
      {
        st = pq.top();
        pq.pop();
        int remain_transition = transition;
        bad_node.reset();
        for (const auto &node : node_list)
        {
          const int from = node;
          if (st.node_val[from] >= 0)
            continue;
          if (bad_node.and_any(my_easy_bitset_graph[from]))
            continue;
          bool node_val_ok = true;
          int graph_idx = 0;
          for (; graph_idx < (int)graph[from].size(); graph_idx++)
          {
            const int to = graph[from][graph_idx];
            if (st.node_val[to] == -1)
              continue;
            const int diff = std::abs(node_val_cur - st.node_val[to]);
            if (st.used_edge_val[diff])
            {
              bad_node.set(to, true);
              node_val_ok = false;
              break;
            }
            st.used_edge_val[diff] = true;
          }
          if (node_val_ok)
          {
            st.node_val[from] = node_val_cur;
            st.used_node_val_count += 1;
            npq.emplace(st);
            if (st.used_node_val_count == n)
            {
              ret = st.node_val;
              for (int i = 0; i < n; ++i)
              {
                cout << ret[i] << " \n"[i + 1 == n];
              }
              return 0;
            }
            if ((int)npq.size() > beam_width)
              npq.pop();
            st.node_val[from] = -1;
            st.used_node_val_count -= 1;
            remain_transition -= 1;
            if (remain_transition == 0)
              break;
          }
          for (int i = 0; i < graph_idx; ++i)
          {
            const int to = graph[from][i];
            if (st.node_val[to] == -1)
              continue;
            const int diff = std::abs(node_val_cur - st.node_val[to]);
            st.used_edge_val[diff] = false;
          }
        }
        if (remain_transition == transition)
          npq.emplace(st);
      }
      std::swap(pq, npq);
      while ((int)pq.size() > beam_width)
        pq.pop();
    }
  }

  std::cerr << exe_simulate_function << endl;
  int simulate_count = 0;
  for (;; ++simulate_count)
  {
    const auto end = std::chrono::system_clock::now();
    const double time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    if (time + one_exe_time * 2 > 9000)
      break;
    const int node1 = xor64() % n;
    const int node2 = xor64() % n;
    std::swap(node_list[node1], node_list[node2]);
    if (node1 == node2)
      continue;
    erase_node_val(node_list, node_val);
    if (exe_simulate_function == 2)
    {
      bad_nodes.assign(max_val_min + 1, MyEasyBitset());
    }
    const int max_val = exe_simulate_function == 1 ? simulate1(node_list, node_val) : simulate2(node_list, node_val);
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