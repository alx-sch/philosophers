/**
Functions for cleaning up simulation resources. It includes routines to free
the memory allocated for philosophers and forks and to destroy any initialized
mutexes.
*/

#include "philo.h"

// IN FILE:

void	cleanup_sim(t_sim **sim_ptr);

/**
Cleans up the fork resources for the simulation.

Destroys all fork (mutex lock). After that, it frees the memory allocated for
the fork array and sets the pointer to NULL to avoid dangling references.

 @param sim 	Pointer to the simulation structure containing fork resources.
*/
static void	clean_forks(t_sim *sim)
{
	int	nr_philo;
	int	i;

	i = 0;
	nr_philo = sim->nr_philo;
	while (i < nr_philo)
	{
		(void)mtx_action(&sim->forks[i].fork, DESTROY, sim);
		i++;
	}
	free(sim->forks);
	sim->forks = NULL;
}

/**
Cleans up the philosopher resources for the simulation.

Destroys all mutex locks for accessing 'last_meal'. After that, it frees the
memory allocated for the philo array and sets the pointer to NULL to avoid
dangling reference

 @param sim 	Pointer to the sim structure containing philosopher resources.
*/
static void	clean_philos(t_sim *sim)
{
	int	nr_philo;
	int	i;

	i = 0;
	nr_philo = sim->nr_philo;
	while (i < nr_philo)
	{
		(void)mtx_action(&sim->philos[i].mtx_last_meal, DESTROY, sim);
		i++;
	}
	free(sim->philos);
	sim->philos = NULL;
}

/**
Cleans up all resources associated with the simulation.

This function handles the cleanup of philosopher and fork resources,
as well as mutexes, if initialized. It ensures that all allocated
memory is freed and sets the main simulation pointer to NULL to avoid
dangling references.

 @param sim_ptr 	Double pointer to the simulation structure, allowing for
					modification of the pointer itself to NULL after cleanup.
*/
void	cleanup_sim(t_sim **sim_ptr)
{
	t_sim	*sim;

	sim = *sim_ptr;
	if (!sim)
		return ;
	if (sim->forks)
		clean_forks(sim);
	if (sim->philos)
		clean_philos(sim);
	if (sim->mtx_full_philos_init)
		(void)mtx_action(&sim->mtx_full_philos, DESTROY, sim);
	if (sim->mtx_stop_sim_init)
		(void)mtx_action(&sim->mtx_stop_sim, DESTROY, sim);
	if (sim->mtx_print_init)
		(void)mtx_action(&sim->mtx_print, DESTROY, sim);
	free(sim);
	*sim_ptr = NULL;
}
