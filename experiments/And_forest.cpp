#include <caterpillar/caterpillar.hpp>
#include <lorina/verilog.hpp>
#include <mockturtle/io/verilog_reader.hpp>
#include <mockturtle/networks/xag.hpp>
#include <tweedledum/io/write_unicode.hpp>
#include <tweedledum/networks/netlist.hpp>

int main ()
{
    mockturtle::xag_network xag;
    auto const read_stats = lorina::read_verilog( "../experiments/benchmarks/AND_3_1.v", mockturtle::verilog_reader( xag ) );
    if ( read_stats != lorina::return_code::success )
    {
        std::cerr << "[e] Failed to read benchmark...\n";
        return -1;
    }
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
    ps.search_timeout = 30u;
    ps.increment_pebbles_on_failure = false;
    ps.decrement_pebbles_on_success = false;
    ps.optimize_weight = false;
    caterpillar::lookup_pebbling_mapping_strategy<mockturtle::xag_network, caterpillar::bsat_pebble_solver<mockturtle::xag_network>> strategy( ps );

    // caterpillar::logic_network_synthHowesis_stats st;
    // tweedledum::netlist<caterpillar::stg_gate> rcirc;
    // caterpillar::logic_network_synthesis( rcirc, xag, strategy, {}, {}, &st );

    return 0;
}