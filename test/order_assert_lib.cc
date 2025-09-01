#include <memory>
#include <vector>

#include "order.h"

namespace matching_engine {
bool AreOrderPointerVectorSame(const std::vector<std::shared_ptr<Order>> &lhs,
                               const std::vector<std::shared_ptr<Order>> &rhs) {
  if (lhs.size() != rhs.size()) {
    return false;
  }

  std::size_t n = lhs.size();
  for (std::size_t i{0}; i < n; i++) {
    if (*lhs[i] != *rhs[i]) {
      return false;
    }
  }

  return true;
}
} // namespace matching_engine
