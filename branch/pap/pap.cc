#include "pap.h"

bool pap::predict_branch(champsim::address ip)
{
  auto& entry = entry_table_[hash(ip)];
  auto pht_idx = entry.bhr.to_ulong();
  auto prediction = entry.pht[pht_idx];
  return prediction.value() > (prediction.maximum / 2);
}

void pap::last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type)
{
  auto& entry = entry_table_[hash(ip)];
  auto pht_idx = entry.bhr.to_ulong();
  entry.pht[pht_idx] += taken ? 1 : -1;

  entry.bhr <<= 1;
  entry.bhr[0] = taken;
}
