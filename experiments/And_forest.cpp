#include <caterpillar/caterpillar.hpp>
#include <lorina/verilog.hpp>
#include <mockturtle/io/verilog_reader.hpp>
#include <mockturtle/networks/xag.hpp>
#include <tweedledum/io/write_unicode.hpp>
#include <tweedledum/networks/netlist.hpp>

int main ()
{
  mockturtle::xag_network xag;
  auto const read_stats = lorina::read_verilog( "../experiments/benchmarks/and_forest_4_step.v", mockturtle::verilog_reader( xag ) );
  if ( read_stats != lorina::return_code::success )
  {
    std::cerr << "[e] Failed to read benchmark...\n";
    return -1;
  }
  std::cout << "[m] The benchmark is successfully loaded.\n";

  caterpillar::pebbling_mapping_strategy_params ps;
  ps.progress = true;
  ps.verbose = true;
  ps.search_timeout = 30u;
  ps.max_steps = 100u;
  ps.increment_pebbles_on_failure = false;
  ps.optimize_weight = false;

  for ( uint8_t decr_on_suc{ 0u }; decr_on_suc <= 1u; ++decr_on_suc )
  {
    /* This parameter is used as a solving mode selector */
    /* When configured to false: Find the mininum qubits count corresponding to any solution */
    /* When configured to true: Find the minimum qubit count correspond to the solution(s) involving the minimum steps */
    ps.decrement_pebbles_on_success = static_cast<bool>( decr_on_suc );
    caterpillar::lookup_pebbling_mapping_strategy<mockturtle::xag_network, caterpillar::bsat_pebble_solver<mockturtle::xag_network>> strategy( ps );
    caterpillar::logic_network_synthesis_stats ls_st;
    tweedledum::netlist<caterpillar::stg_gate> rcirc;
    caterpillar::logic_network_synthesis_params ls_ps;
    caterpillar::logic_network_synthesis( rcirc, xag, strategy, {}, ls_ps, &ls_st );
  }

  return 0;
}