#include "plru.h"

#include <algorithm>
#include <cassert>

namespace
{
void check_sets_ways_num([[maybe_unused]] long sets, [[maybe_unused]] long ways)
{
  assert(sets > 1);
  assert(ways > 1);
  assert((sets & (sets - 1)) == 0);
  assert((ways & (ways - 1)) == 0);
}

auto exponent2(size_t pow2)
{
  unsigned exp = 0;
  while (pow2 >>= 1) {
    ++exp;
  }
  return exp;
}

} // namespace

plru::plru(CACHE* cache) : plru(cache, cache->NUM_SET, cache->NUM_WAY) {}

plru::plru(CACHE* cache, long sets, long ways) : replacement(cache), direction_tree_(sets, direction_bits(ways - 1)) { check_sets_ways_num(sets, ways); }

long plru::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip,
                       champsim::address full_addr, access_type type)
{
  return direction_tree_[set].find_victim();
}

void plru::replacement_cache_fill(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr,
                                  access_type type)
{
  direction_tree_[set].update_lru(way);
}

void plru::update_replacement_state(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                                    champsim::address victim_addr, access_type type, uint8_t hit)
{
  if (hit && access_type{type} != access_type::WRITE)
    direction_tree_[set].update_lru(way);
}

plru::direction_bits::direction_bits(long bits_num) : bits_(bits_num, 0) {}

long plru::direction_bits::find_victim() const
{
  auto node = 0L;
  auto way = 0L;
  // NOTE: prefer bit_width instead of exponent2
  for (size_t level = 0; level < exponent2(bits_.size() + 1); ++level) {
    auto direction = bits_[node];
    way = (way << 1) | direction;
    node = 2 * node + 1 + direction;
  }
  return way;
}

void plru::direction_bits::update_lru(long way)
{
  auto node = 0L;
  for (size_t level = 0, depth = exponent2(bits_.size() + 1); level < depth; ++level) {
    auto bit_pos = depth - level - 1;
    auto direction = (way >> bit_pos) & 1;
    bits_[node] = !direction;
    node = 2 * node + 1 + direction;
  }
}
