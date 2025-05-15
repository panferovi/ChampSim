#ifndef BRANCH_PAP_H
#define BRANCH_PAP_H

#include <array>
#include <bitset>

#include "address.h"
#include "modules.h"
#include "msl/fwcounter.h"

class pap : champsim::modules::branch_predictor
{
  [[nodiscard]] static constexpr auto hash(champsim::address ip) { return ip.to<unsigned long>() % PRIME; }

  static constexpr std::size_t TABLE_SIZE = 3276;
  static constexpr std::size_t PRIME = 3271;
  static constexpr std::size_t BITS = 2;

  struct pap_entry {
    static constexpr std::size_t TABLE_SIZE = 4;
    std::bitset<BITS> bhr;
    std::array<champsim::msl::fwcounter<BITS>, TABLE_SIZE> pht;
  };

  std::array<pap_entry, TABLE_SIZE> entry_table_;

public:
  using branch_predictor::branch_predictor;

  // void initialize_branch_predictor();
  bool predict_branch(champsim::address ip);
  void last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type);
};

#endif
