#ifndef ORDER_ASSERT_LIB_H_
#define ORDER_ASSERT_LIB_H_

#include <memory>
#include <vector>

#include "order.h"

namespace matching_engine
{
inline auto AreOrderPointerVectorSame(
    const std::vector<std::shared_ptr<Order>>& lhs,
    const std::vector<std::shared_ptr<Order>>& rhs) -> bool
{
  if (lhs.size() != rhs.size()) {
    return false;
  }

  for (std::size_t i {0}; i < lhs.size(); i++) {
    if (*lhs[i] != *rhs[i]) {
      return false;
    }
  }

  return true;
}
}  // namespace matching_engine
#endif
