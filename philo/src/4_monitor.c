#include "philo.h"

/**
Checks if all philosophers are full.

 @param sim 		A pointer to the simulation structure.
 @param nr_philos 	The total number of philosophers in the simulation.

 @return		`1` if all philosophers are full;
				`0` otherwise;
				`2` if there is an error in mutex locking/unlocking.
*/
static int	all_philos_full(t_sim *sim, int nr_philos)
{
	int	full;

	if (mtx_action(&sim->mtx_full_philos, LOCK, sim))
		return (2);
	full = 0;
	if (sim->full_philos == nr_philos)
		full = 1;
	if (mtx_action(&sim->mtx_full_philos, UNLOCK, sim))
		return (2);
	return (full);
}

/**
Handles the death of a philosopher by recording the time of death,
printing the death action, and updating the simulation state to indicate
that a philosopher has died.

 @param sim 	A pointer to the simulation structure containing global state.
 @param philo 	A pointer to the philosopher who has died.

 @return		`1` if the philosopher's death was successfully handled;
				`2` if there was an error in time retrieval, logging,
				or mutex operations.
*/
static int	handle_death(t_sim *sim, t_philo *philo)
{
	if (mtx_action(&philo->mtx_last_meal, UNLOCK, sim)
		|| record_time_of_death(philo)
		|| mtx_action(&sim->mtx_stop_sim, LOCK, sim))
		return (2);
	sim->stop_sim = 1;
	if (mtx_action(&sim->mtx_stop_sim, UNLOCK, sim))
		return (2);
	(void)usleep(100);
	if (print_action(philo->timestamp_death, philo, DIE, 0))
		return (2);
	return (1);
}

/**
Checks if the time since the philosopher's last meal exceeds the allowed
duration (t_die). If so, it handles the philosopher's starvation by
logging their death and updating the simulation state.

 @param sim				A pointer to the simulation structure.
 @param philo 			A pointer to the philosopher to check.
 @param current_time 	The current time to compare against the last meal time.
 @param t_die 			The maximum allowable time (in ms) since the last meal.

 @return				`0` if the time difference is within the allowed limit;
						`1` if the philosopher has starved and the starvation
						was handled succesfully;
						`2` if there is an error in mutex locking/unlocking etc.
*/
static int	check_starvation_and_log(t_sim *sim, t_philo *philo,
	t_ull current_time, t_ull t_die)
{
	if (mtx_action(&philo->mtx_last_meal, LOCK, sim))
		return (2);
	if (current_time - philo->t_last_meal > t_die)
		return (handle_death(sim, philo));
	if (mtx_action(&philo->mtx_last_meal, UNLOCK, sim))
		return (2);
	return (0);
}

/**
Monitors the state of the philosophers in the simulation to check for starvation
and whether all philosophers are full. It runs in an infinite loop until all
philosophers have eaten their fill or a philosopher dies.

Error handling is intentionally omitted, as it falls outside the project's
scope and would be cumbersome due to the project's limitations (cannot use
`pthread_exit()`).

 @param arg 	A pointer to the simulation structure containing the global state
				of the simulation, including the list of philosophers and their
				current status.

 @return		Returns `NULL` to indicate that the monitoring has stopped,
 				either because all philosophers are full or due to handling a
				philosopher's starvation.
*/
void	*monitor(void *arg)
{
	t_sim	*sim;
	t_ull	t_die;
	int		nr_philos;
	int		i;
	t_ull	current_time;

	sim = (t_sim *)arg;
	t_die = (t_ull)sim->t_die;
	nr_philos = sim->nr_philo;
	while (1)
	{
		if (all_philos_full(sim, nr_philos) == 1)
			return (NULL);
		i = 0;
		current_time = get_time();
		while (i < sim->nr_philo)
		{
			if (check_starvation_and_log(sim, &sim->philos[i],
					current_time, t_die) == 1)
				return (NULL);
			i++;
		}
		(void)usleep(100);
	}
	return (NULL);
}
