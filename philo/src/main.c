/**
Main entry point for the dining philosophers simulation.
The main() initializes the simulation, runs it, and handles cleanup after the
simulation has finished.
*/

#include "philo.h"

// IN FILE:

int	main(int argc, char **argv);

/**
Main function for running the philosopher dining simulation.

This function initializes the simulation, runs it, and then
performs cleanup before exiting.

 @param	argc 	The number of command-line arguments.
 @param argv 	An array of command-line argument strings.

 @return		Returns `0` on successful completion;
				`1` if there was an error during initialization or running the
				simulation.
*/
int	main(int argc, char **argv)
{
	t_sim	*sim;

	sim = NULL;
	if (init_sim(&sim, argc, argv))
	{
		cleanup_sim(&sim);
		return (1);
	}
	if (run_sim(sim))
	{
		cleanup_sim(&sim);
		return (1);
	}
	cleanup_sim(&sim);
	return (0);
}
