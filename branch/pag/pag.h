#ifndef BRANCH_PAG_H
#define BRANCH_PAG_H

#include <array>
#include <bitset>

#include "address.h"
#include "modules.h"
#include "msl/fwcounter.h"

class pag : champsim::modules::branch_predictor
{
  [[nodiscard]] static constexpr auto get_bhr_idx(champsim::address ip) { return ip.to<unsigned long>() % PRIME; }
  [[nodiscard]] static constexpr auto get_pht_idx(champsim::address ip, uint64_t bhr) { return (ip.to<unsigned long>() ^ bhr) % PRIME; }

  static constexpr std::size_t TABLE_SIZE = 8192;
  static constexpr std::size_t PRIME = 8191;
  static constexpr std::size_t BITS = 2;

  std::array<std::bitset<BITS>, TABLE_SIZE> bhr_table_;
  std::array<champsim::msl::fwcounter<BITS>, TABLE_SIZE> pht_;

public:
  using branch_predictor::branch_predictor;

  // void initialize_branch_predictor();
  bool predict_branch(champsim::address ip);
  void last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type);
};

#endif
