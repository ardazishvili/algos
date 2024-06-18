#include <algorithm>
#include <cinttypes>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
#include <vector>

using Partition = std::vector<std::vector<int>>;

class KK {
 public:
  KK(int k) : _k(k) {}

  void merge(Partition& to, Partition& from) {
    auto partition_decreasing = [](std::vector<int>& lhs,
                                   std::vector<int>& rhs) {
      return std::reduce(lhs.begin(), lhs.end()) >
             std::reduce(rhs.begin(), rhs.end());
    };

    std::sort(to.begin(), to.end(), partition_decreasing);
    std::sort(from.begin(), from.end(), std::not_fn(partition_decreasing));

    for (int i = 0; i < _k; ++i) {
      to[i].insert(to[i].end(), from[i].begin(), from[i].end());
    }
  }

  Partition kk(std::vector<int> in) {
    auto compare = [](const Partition& p1, const Partition& p2) -> bool {
      // calculates sums of every bucket in partition and return (max_sum -
      // min_sum)
      auto amplitude = [](const Partition& p) -> int {
        std::vector<int> sums;
        std::transform(p.begin(), p.end(), std::back_inserter(sums),
                       [](const std::vector<int>& vec) {
                         return std::reduce(vec.begin(), vec.end());
                       });
        auto [min_it, max_it] = std::minmax_element(sums.begin(), sums.end());
        return *max_it - *min_it;
      };
      return amplitude(p1) < amplitude(p2);
    };
    std::priority_queue<Partition, std::vector<Partition>, decltype(compare)> q{
        compare};
    int n = in.size();
    for (int i = 0; i < n; ++i) {
      auto p = std::vector<std::vector<int>>(_k);
      p[0] = {in[i]};
      q.push(p);
    }
    while (q.size() > 1) {
      Partition a = q.top();
      q.pop();
      Partition b = q.top();
      q.pop();

      merge(a, b);
      q.push(a);
    }
    return q.top();
  }

 private:
  int _k;
};

int main() {
  {
    KK kk(2);
    auto vecs = kk.kk({8, 7, 6, 5, 4});  // {4, 7, 5} and {8, 6}
    for (auto vec : vecs) {
      for (auto v : vec) {
        std::cout << v << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "_______________________" << std::endl;
    vecs = kk.kk({9, 8, 7, 4, 4});  // {7,9} {4, 4, 8}
    for (auto vec : vecs) {
      for (auto v : vec) {
        std::cout << v << " ";
      }
      std::cout << std::endl;
    }
  }
  std::cout << "_______________________" << std::endl;
  {
    KK kk(3);
    auto vecs = kk.kk({8, 7, 6, 5, 4});  // {5, 6} and {4, 7} and {8}
    for (auto vec : vecs) {
      for (auto v : vec) {
        std::cout << v << " ";
      }
      std::cout << std::endl;
    }
    std::cout << "_______________________" << std::endl;
    vecs =
        kk.kk({5, 5, 5, 4, 4, 3, 3, 1});  // {4, 5} and {1, 4, 5} and {3, 3, 5}
    for (auto vec : vecs) {
      for (auto v : vec) {
        std::cout << v << " ";
      }
      std::cout << std::endl;
    }
  }
}
