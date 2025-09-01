# Build

See the [BUILDING](BUILDING.md) document.

# Develop

See the [HACKING](HACKING.md) document.

# Functional design
Essentially the order matching engine enables us to quickly find best match sell orders for a buy order, or buy orders for a seller order, where the best match is defined by the highest prices for matching a sell order, or the lowest prices for matching a buy order.

Intuitively, heaps should be the go-to solution. We can maintain two heaps, where one stores all buy orders, with the highest price orders on the top, and the other stores all sell orders, with the lowest price orders on the top. In case of the same price, timestamp is the tie breaker.

Whenever there is a new order coming in, e.g., a buy order, as long as it has a match with the sell order heap top, we generate a trade and deduct the trade quantity from both orders. If the seller order heap top's quantity becomes zero, we pop it from the heap. If the buy order's quantity becomes zero or the heap becomes empty, we stop matching. Otherwise, we continue looking for matching orders from the heap.

Once all matched order have been found, if the buy order quantity is still greater than zero, we push it to the buy order heap since we are not able to find any match for it as for now. 

The same process applies for a seller order matching.

# Implementation highlights
## No dynamic memory allocation
Caveat: except for the STL containers default allocators.
## Replace `std::priority_queue` with a `std::vector`
In my very initial implementation, I used a `std::priority_queue` as a heap. However, I realized that the element in the `std::priority_queue` is immutable, which means that I had to pop the order from the heap, modify its quantity and push it back. This is suboptimal in terms of performance. Instead, a `std::vector` is being used together with `std::pop_heap` and `std::push_heap`. This issue could also have been alleviated by using a shared pointer as the `std::priority_queue` element type; however, that approach may increases complexity and reduces readability.
## Minimize `std::string` copies
Most of strings are moved instead of being copied. The performance and memory gain will be more substantial if the order ID and/or the instrument exceeds 22 characters, due to C++'s short string optimization.

# Future work
1. To use a stack memory allocator for the STL containers, e.g., `std::string` and `std::vector`, such that zero dynamic memory allocation is achieved.
1. To add integration tests.
1. To add documentations.
1. To add continuous integration.
1. To utilize static code analysis tool such as `clang-tidy`.
