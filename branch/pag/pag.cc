#include "pag.h"

bool pag::predict_branch(champsim::address ip)
{
  auto bhr = bhr_table_[get_bhr_idx(ip)].to_ulong();
  auto prediction = pht_[get_pht_idx(ip, bhr)];
  return prediction.value() > (prediction.maximum / 2);
}

void pag::last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type)
{
  auto& bhr = bhr_table_[get_bhr_idx(ip)];
  pht_[get_pht_idx(ip, bhr.to_ulong())] += taken ? 1 : -1;

  bhr <<= 1;
  bhr[0] = taken;
}
