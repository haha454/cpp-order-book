#ifndef ORDER_ASSERT_LIB_H_
#define ORDER_ASSERT_LIB_H_

#include <memory>
#include <vector>

#include "order.h"

namespace matching_engine {
bool AreOrderPointerVectorSame(const std::vector<std::shared_ptr<Order>> &lhs,
                               const std::vector<std::shared_ptr<Order>>
                                   &rhs);
}
#endif