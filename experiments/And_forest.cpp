#include <caterpillar/caterpillar.hpp>
#include <lorina/verilog.hpp>
#include <mockturtle/io/verilog_reader.hpp>
#include <mockturtle/networks/xag.hpp>
#include <tweedledum/io/write_unicode.hpp>
#include <tweedledum/networks/netlist.hpp>

int main ()
{
  uint8_t num_vars_max{ 7u };
  std::string prefix{ "../experiments/benchmarks/" };
  for ( size_t step_oriented{ 0u }; step_oriented <= 1u; ++step_oriented )
  {
    for ( uint8_t num_vars{ 7u }; num_vars <= num_vars_max; ++num_vars )
    {
      std::string benchmark = fmt::format( "heuristic_and_forest_{}_{}", num_vars, ( step_oriented ? "step" : "qubit" ) );
      std::string benchmark_names = fmt::format( "{}{}.v", prefix, benchmark );
      mockturtle::xag_network xag;
      auto const read_stats = lorina::read_verilog( benchmark_names, mockturtle::verilog_reader( xag ) );
      if ( read_stats != lorina::return_code::success )
      {
          std::cerr << fmt::format( "[e] Failed to read benchmark {}\n", benchmark_names );
          return -1;
      }
      std::cout << "[m] The benchmark is successfully loaded.\n";
      fmt::print( "[m] Playing the reversible pebbling game on {} with the min-pebble strategy...\n", benchmark );
      // const auto  in1 = xag.create_pi();
      // const auto  in2 = xag.create_pi();
      // const auto  in3 = xag.create_pi();
      // const auto   n4 = xag.create_and( in1, in2 );
      // const auto   n5 = xag.create_and( in2, in3 );
      // const auto out1 = xag.create_and( n4, n5 );
      // const auto out2 = xag.create_and( in1, in3 );
      // xag.create_po( out1 );
      // xag.create_po( out2 );

      caterpillar::pebbling_mapping_strategy_params ps;
      ps.progress = true;
      ps.verbose = true;
      ps.search_timeout = 3600u;
      ps.max_steps = 300u;
      ps.increment_pebbles_on_failure = false;
      ps.decrement_pebbles_on_success = false;
      ps.optimize_weight = false;
      caterpillar::lookup_pebbling_mapping_strategy<mockturtle::xag_network, caterpillar::bsat_pebble_solver<mockturtle::xag_network>> strategy( ps );
      // caterpillar::pebbling_mapping_strategy<mockturtle::xag_network, caterpillar::bsat_pebble_solver<mockturtle::xag_network>> strategy( ps );

      caterpillar::logic_network_synthesis_stats ls_st;
      tweedledum::netlist<caterpillar::stg_gate> rcirc;
      caterpillar::logic_network_synthesis_params ls_ps;
      // ls_ps.verbose = true;
      caterpillar::logic_network_synthesis( rcirc, xag, strategy, {}, ls_ps, &ls_st );
    }
  }

  return 0;
}
