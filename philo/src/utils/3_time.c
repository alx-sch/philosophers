/**
This file provides utility functions for time management in the dining
philosophers simulation.
*/

#include "philo.h"

// IN FILE:

t_ull	get_time(void);
int		set_start_time(t_sim *sim);
int		record_time_of_death(t_philo *philo);
int		precise_wait(int duration_to_wait);

/**
Get the current time in milliseconds since the epoch (January 1, 1970).

 @return	The current time in milliseconds;
			`0` in case of an error (or if it's 00:00:00 on 1/1/1970...)
*/
t_ull	get_time(void)
{
	struct timeval	tv;
	t_ull			time_in_ms;

	if (gettimeofday(&tv, NULL))
	{
		print_err_msg(ERR_GET_TIME, NULL);
		return (0);
	}
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}

/**
Sets the starting time for the simulation in `sim->t_start_sim` by retrieving
the current time right before the philosopher threads are created in `run_sim()`.

This function also initializes each philosopher's last meal time to the
start time.

 @param sim 	Pointer to the simulation structure to store the start time.

 @return		`0` on success;
 				`1` if there was an error retrieving the time.
*/
int	set_start_time(t_sim *sim)
{
	t_ull	time_start_sim;
	int		i;

	time_start_sim = get_time();
	if (time_start_sim == 0)
		return (1);
	sim->t_start_sim = time_start_sim;
	i = 0;
	while (i < sim->nr_philo)
	{
		sim->philos[i].t_last_meal = time_start_sim;
		i++;
	}
	return (0);
}

/**
Records the time of death for a philosopher.

This function retrieves the current time, calculates the philosopher's
time of death relative to the simulation's start time, and stores it in
`philo->timestamp_death`.

 @param philo 	Pointer to the philosopher structure where the time of
				death will be stored.

 @return		`0` on success;
				`1` if there was an error retrieving the current time.
*/
int	record_time_of_death(t_philo *philo)
{
	t_ull	t_death;

	t_death = get_time();
	if (t_death == 0)
		return (1);
	philo->timestamp_death = t_death - philo->sim->t_start_sim;
	return (0);
}

/**
Halts execution for a specified duration with improved precision.

This function utilizes a combination of `get_time()` and short sleeps
to provide a more accurate waiting period. While traditional sleep functions
like `usleep()` may suffer from inaccuracies due to system scheduling, this
implementation checks the current time at regular intervals, effectively
mitigating such limitations.

The `SLEEP_INTERVALS` constant is defined in `philo.h`.

 @param	duration_to_wait 	Duration in milliseconds for which to wait.
 @param	sim 				Pointer to a data struct to be freed
 							in case of error.

 @return					`0` if the wait was successful;
							`1` if there was an error retrieving the time.
*/
int	precise_wait(int duration_to_wait)
{
	t_ull	time_stop_waiting;
	t_ull	current_time;

	time_stop_waiting = get_time();
	if (time_stop_waiting == 0)
		return (1);
	time_stop_waiting += duration_to_wait;
	while (1)
	{
		current_time = get_time();
		if (current_time == 0)
			return (1);
		if (current_time >= time_stop_waiting)
			break ;
		(void)usleep((duration_to_wait * 1000) / SLEEP_INTERVALS);
	}
	return (0);
}
