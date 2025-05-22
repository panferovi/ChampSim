#include "lfu.h"

#include <algorithm>
#include <cassert>

lfu::lfu(CACHE* cache) : lfu(cache, cache->NUM_SET, cache->NUM_WAY) {}

lfu::lfu(CACHE* cache, long sets, long ways) : replacement(cache), way_num_(ways), freqs_(static_cast<std::size_t>(sets * ways), 0) {}

long lfu::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip,
                      champsim::address full_addr, access_type type)
{
  auto begin = std::next(std::begin(freqs_), set * way_num_);
  auto end = std::next(begin, way_num_);

  // Find the way whose last use cycle is most distant
  auto victim = std::min_element(begin, end);
  assert(begin <= victim);
  assert(victim < end);
  return std::distance(begin, victim);
}

void lfu::replacement_cache_fill(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr,
                                 access_type type)
{
  // Mark the way as being used on the current cycle
  freqs_[set * way_num_ + way] = 0;
}

void lfu::update_replacement_state(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                                   champsim::address victim_addr, access_type type, uint8_t hit)
{
  // Mark the way as being used on the current cycle
  if (hit && access_type{type} != access_type::WRITE) // Skip this for writeback hits
    freqs_[set * way_num_ + way]++;
}
