#pragma once

#include <chrono>
#include <mockturtle/utils/progress_bar.hpp>
#include <mockturtle/views/depth_view.hpp>
#include <caterpillar/synthesis/strategies/action.hpp>
#include <caterpillar/solvers/z3_solver.hpp>
#include <type_traits>
#include <limits>


using namespace std::chrono;

namespace caterpillar
{

struct pebbling_mapping_strategy_params
{
  /*! \brief Show progress bar. */
  bool progress{false};

  /*! \brief Print solution. */
  bool verbose{false};

  /*! \brief Maximum number of pebbles to use, if supported by mapping strategy (0 means no limit). */
  uint32_t pebble_limit{0};

  /*! \brief Maximum number of steps */
  uint32_t max_steps{std::numeric_limits<uint32_t>::max()};

  /*! \brief Conflict limit for the SAT solver (0 means no limit). */
  uint32_t conflict_limit{0};

  /*! \brief Timeout for the solver in milliseconds. */
  uint32_t solver_timeout{0};

  /*! \brief Timeout for the iterative quests in seconds. */
  uint32_t search_timeout{30};

  /*! \brief Increment pebble numbers, if a failure occurs. */
  bool increment_pebbles_on_failure{false};

  /*! \brief Decrement pebble numbers, if satisfiable. */
  bool decrement_pebbles_on_success{false};

  /*! \brief Decrement max weight, if satisfiable. */
  bool optimize_weight{false};

};

template<typename Ntk>
using Steps = std::vector<std::pair<typename Ntk::node, mapping_strategy_action>>;

template <typename Solver, typename Ntk>
inline Steps<Ntk> pebble (Ntk ntk, pebbling_mapping_strategy_params const& ps = {})
{
  assert( !ps.decrement_pebbles_on_success || !ps.increment_pebbles_on_failure );
  assert( !ps.decrement_pebbles_on_success || !ps.optimize_weight );
  assert( !ps.increment_pebbles_on_failure || !ps.optimize_weight );

  auto limit = ps.pebble_limit;
  
  auto start = high_resolution_clock::now(); 

  Steps<Ntk> steps;
  while ( true )
  {
    Solver solver( ntk, limit, ps.conflict_limit, ps.solver_timeout);
    typename Solver::result result;

    solver.init();

    mockturtle::progress_bar bar( 100, "|{0}| current step = {1}", ps.progress );

    do
    {
      if ( solver.current_step() >= ps.max_steps )
      {
        result = solver.unknown();
        break;
      }

      bar( std::min<uint32_t>( solver.current_step(), 100 ), solver.current_step() );

      solver.add_step();
      result = solver.solve(); 

    } while ( result == solver.unsat() && 
        duration_cast<seconds>(high_resolution_clock::now() - start).count() <= ps.search_timeout);

    if ( result == solver.unknown() || result == solver.unsat() )
    {
      if ( ps.increment_pebbles_on_failure )
      {
        limit++;
        continue;
      }
    }
    else if ( result == solver.sat() )
    {
      solver.save_model();
      #ifdef USE_Z3
             
      if(ps.optimize_weight)
      {
        if constexpr (std::is_same_v<Solver, z3_pebble_solver<Ntk>>)
        { 
          solver.optimize_solution();
        }
      }

      #endif

      steps = solver.extract_result();

      if ( ps.decrement_pebbles_on_success && limit > 1)
      {
        limit--;
        continue;
      }

    }

    return steps;
  }

}

template<typename Solver, typename Ntk>
Steps<Ntk> lookup_pebble( Ntk const& ntk, pebbling_mapping_strategy_params const& ps = {} )
{
  Steps<Ntk> steps;
  uint32_t pebble_limit = ps.pebble_limit ? ps.pebble_limit : ntk.num_gates();
  // std::cout << "[m] The network has " << ntk.num_gates() << " gates.\n";
  mockturtle::progress_bar bar( pebble_limit, "|{0}| number of exploitable pebbles = {1}", ps.progress );

  if ( !ps.decrement_pebbles_on_success )
  {
    /* find the minimum number of pebbles without constraints on the resulting number of steps */
    for ( uint32_t num_pebbles{ 3u }; num_pebbles <= pebble_limit; ++num_pebbles )
    {
      bar( num_pebbles, num_pebbles );
      auto start = high_resolution_clock::now();

      for ( uint32_t num_steps{ 51u }; num_steps <= ps.max_steps; ++num_steps )
      {
        Solver solver( ntk, num_pebbles, ps.conflict_limit, ps.solver_timeout );
        solver.lookup_init( num_steps );
        solver.base_constraints( num_steps );
        solver.all_reached_constraint();
        typename Solver::result result = solver.lookup_solve();

        if ( result == solver.sat() )
        {
          std::cout << fmt::format( "\n[i] Found a valid solution!\n" );
          solver.save_model();
          steps = solver.extract_result();
          return steps;
        }

        else
        {
          if ( duration_cast<seconds>( high_resolution_clock::now() - start ).count() > ps.search_timeout )
          {
            std::cout << fmt::format( "\n[i] Met search time upper bound...\n" );
            break;
          }
        }
      }
    }
  }
  else
  {
    /* given a fixed number of steps, find the minimum number of pebbles required */
    // uint32_t num_steps = mockturtle::depth_view<Ntk>{ ntk }.depth() * 2;
    for ( uint32_t num_steps = mockturtle::depth_view<Ntk>{ ntk }.depth() * 2; num_steps <= ps.max_steps; ++num_steps )
    {
      for ( uint32_t num_pebbles{ 42u }; num_pebbles <= pebble_limit; ++num_pebbles )
      {
        bar( num_pebbles, num_pebbles );
        auto start = high_resolution_clock::now();
        Solver solver( ntk, num_pebbles, ps.conflict_limit, ps.solver_timeout );
        solver.lookup_init( num_steps );
        solver.base_constraints( num_steps );
        solver.all_reached_constraint();
        typename Solver::result result = solver.lookup_solve();

        if ( result == solver.sat() )
        {
          std::cout << fmt::format( "\n[i] Found a valid solution!\n" );
          solver.save_model();
          steps = solver.extract_result();
          return steps;
        }

        else
        {
          if ( duration_cast<seconds>( high_resolution_clock::now() - start ).count() > ps.search_timeout )
          {
            std::cout << fmt::format( "\n[i] Met search time upper bound...\n" );
            break;
          }
        }
      }
    }
  }
  std::cout << fmt::format( "\n[e] No solution found. Something went wrong...\n" );
  return steps;
}

}//caterpillar