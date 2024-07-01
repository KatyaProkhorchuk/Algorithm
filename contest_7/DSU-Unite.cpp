//
//Createdbykaterinaon07.03.23.
//
/*
С.любитобучатьнейросети.Дляихрегуляризации(регулированиясложностимодели)естьтакоепонятиекакdropout.Наинтуитивномуровнеэтоудалениесвязимеждунейронамивграфенейросети.
А.тожелюбитнейросети,нобольшеонлюбитспамитьвличкуС.посамымразнымповодам(наиболеепопулярныйвариант—«Говциву»,кстатизовитеА.вциву).Онпрознал,чтоС.
прямосейчаснастраиваетdropoutипостоянноспрашивает,лежатлидванейронаводнойкомпонентесвязностивегонейросети.
Действиягероевзаписанывследующемформате:
операцияС.cutu,v—вdropoutрешеноубратьребромеждунейронамиuиv;
операцияА.asku,v—проверить,лежатнейроныuиvводнойкомпонентесвязности.
Известно,чтопослевыполнениявсехоперацийтипаcutрёбервграфенейросетинеосталось.Найдитерезультатвыполнениякаждойизоперацийтипаask.
Гарантируется,чтографнейросетинесодержитпетельикратныхрёбер.
*/
#include<iostream>
#include<vector>
using Edge = std::pair<int32_t, int32_t>;
class DSU {
 public:
  DSU(size_t n) {
    ancestors_ = std::vector<int32_t>(n + 1, 0);
    for (size_t i = 1; i <= n; ++i) {
      ancestors_[i] = i;
    }
    ranks_ = std::vector<int32_t>(n, 0);
  }
  bool AreSame(int32_t u, int32_t v) {
    return FindSet(u) == FindSet(v);
  }
  void Unite(int32_t u, int32_t v) {
    u = FindSet(u);
    v = FindSet(v);
    if (u != v) {
      if (ranks_[u] < ranks_[v]) {
        std::swap(u, v);
      }
      ancestors_[v] = u;
      if (ranks_[v] == ranks_[u]) {
        ++ranks_[u];
      }
    }
  }
  int32_t FindSet(int32_t elem) {
    if (elem == ancestors_[elem]) {
      return elem;
    }
    return ancestors_[elem] = FindSet(ancestors_[elem]);
  }
 private:
  std::vector<int32_t> ancestors_;
  std::vector<int32_t> ranks_;
};
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  size_t n = 0, m = 0, k = 0;
  std::cin >> n >> m >> k;
  std::vector<Edge> graph;
  for (size_t i = 0; i < m; ++i) {
    int32_t from;
    int32_t to;
    std::cin >> from >> to;
  }
  DSU dsu(n);
  std::vector<std::string> answer;
  answer.reserve(k);
  std::vector<std::vector<int32_t>> neuron;
  neuron.reserve(k);
  for (size_t i = 0; i < k; ++i) {
    std::string command;
    int32_t u = 0, v = 0;
    std::cin >> command >> u >> v;
    if (command == "ask") {
      neuron.push_back({1, u, v});
    } else {
      neuron.push_back({0, u, v});
    }
  }
  for (int32_t i = k - 1; i >= 0; --i) {
    if (neuron[i][0]) {
      if (dsu.AreSame(neuron[i][1], neuron[i][2])) {
        answer.emplace_back("YES");
      } else {
        answer.emplace_back("NO");
      }
    } else {
      dsu.Unite(neuron[i][1], neuron[i][2]);
    }
  }
  for (int32_t i = answer.size() - 1; i >= 0; --i) {
    std::cout << answer[i] << '\n';
  }
  return 0;
}