/**
Implements functions for running the simulation by creating and joining
monitor and philosopher threads.
*/

#include "philo.h"

// IN FILE:

int	run_sim(t_sim *sim);

/**
Starts the monitor thread to observe the state of the simulation.

Monitoring is not started when the number of meals to eat is zero, as monitoring
is not needed.

A short delay (`usleep`) is introduced to ensure the monitor is fully
initialized before the philosopher threads begin their dining routines.

 @param	sim 	Pointer to the sim structure, which holds the philosopher data.

 @return		`0` on success;
				`1` if there was an error creating the monitor thread.
*/
static int	start_monitoring(t_sim *sim)
{
	if (sim->max_meals == 0)
		return (0);
	if (pthread_create(&sim->monitor, NULL, &monitor, sim))
	{
		print_err_msg(ERR_TR_CREATE, sim);
		return (1);
	}
	(void)usleep(100);
	return (0);
}

/**
Joins the monitor thread, waiting for it to finish execution.

Monitoring is not ended when the number of meals to eat is zero, as monitoring is
not needed.

 @param	sim 	Pointer to the sim structure, which holds the philosopher data.

 @return		`0` on success;
				`1` if there was an error joining the monitor thread.
*/
static int	end_monitoring(t_sim *sim)
{
	if (sim->max_meals == 0)
		return (0);
	if (pthread_join(sim->monitor, NULL))
	{
		print_err_msg(ERR_TR_JOIN, sim);
		return (1);
	}
	return (0);
}

/**
Starts the dining simulation by creating threads for each philosopher. Each
thread will run the `dining` function that simulates the philosopher's behavior.

If any thread creation fails, the simulation stops, all previously created
threads are joined (cleaned up), and the simulation is terminated.

 @param	sim 	Pointer to the sim structure, which holds the philosopher data.

 @return		`0` on success;
				`1` if there was an error creating any of the threads.
*/
static int	start_dining(t_sim *sim)
{
	int	i;
	int	nr_philo;

	i = 0;
	nr_philo = sim->nr_philo;
	while (i < nr_philo)
	{
		if (pthread_create(&sim->philos[i].thread_id, NULL, &eat_sleep_think,
				&sim->philos[i]))
		{
			(void)mtx_action(&sim->mtx_stop_sim, LOCK, sim);
			sim->stop_sim = 1;
			(void)mtx_action(&sim->mtx_stop_sim, UNLOCK, sim);
			while (i > 0)
				pthread_join(sim->philos[--i].thread_id, NULL);
			end_monitoring(sim);
			(void)usleep(100);
			print_err_msg(ERR_TR_CREATE, sim);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
Ends the dining simulation by joining all philosopher threads, waiting until
all philosophers have completed their execution.

 @param	sim 	Pointer to the sim structure, which holds the philosopher data.

 @return		`0` on success;
				`1` if there was an error joining any of the threads.
*/
static int	end_dining(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nr_philo)
	{
		if (pthread_join(sim->philos[i].thread_id, NULL))
		{
			mtx_action(&sim->mtx_stop_sim, LOCK, sim);
			sim->stop_sim = 1;
			mtx_action(&sim->mtx_stop_sim, UNLOCK, sim);
			end_monitoring(sim);
			(void)usleep(100);
			print_err_msg(ERR_TR_JOIN, sim);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
Runs the entire simulation.

This function orchestrates the simulation by calling the necessary functions
to set the start time, handles the case of a single philosopher, if applicable,
creates threads for the monitor and each philosopher, and joining those threads
after the simulation has finished.

 @param	sim 	Pointer to the sim structure, which holds the philosopher data.

 @return		`0` on success;
				`1` if any of the steps fails (setting start time,
				thread creation, or thread joining).
*/
int	run_sim(t_sim *sim)
{
	if (set_start_time(sim))
		return (1);
	if (sim->nr_philo == 1)
	{
		if (simulate_single_philo(sim))
			return (1);
		return (0);
	}
	if (start_monitoring(sim))
		return (1);
	if (start_dining(sim))
		return (1);
	if (end_dining(sim))
		return (1);
	if (end_monitoring(sim))
		return (1);
	return (0);
}
