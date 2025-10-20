/**
Defines the core functions involved in a philosopher's life cycle during the
dining simulation. It includes routines for eating, sleeping, and thinking,
as well as checks for stopping the simulation when a philosopher dies or
reaches the maximum number of meals.
*/

#include "philo.h"

// IN FILE:

void	*eat_sleep_think(void *arg);
int		stop_sim(t_philo *philo);

/**
Checks whether a philo thread should stop by accessing the `stop_sim` flag.

 @param sim 	Pointer to the simulation struct containing the `stop_sim` flag.

 @return		The value of the `stop_sim` flag:
				`0` if no philo died (simulation can continue);
				`1` if a philosopher has died (simulation should stop);
				`2` if there is an error in mutex (un)locking.
*/
int	stop_sim(t_philo *philo)
{
	int		dead;

	if (mtx_action(&philo->sim->mtx_stop_sim, LOCK, philo->sim))
		return (2);
	dead = philo->sim->stop_sim;
	if (mtx_action(&philo->sim->mtx_stop_sim, UNLOCK, philo->sim))
		return (2);
	return (dead);
}

/**
Checks if the maximum allowed meals is zero. If so, logs the appropriate
"stuffed" and "thinking" actions.

 @param philo 		A pointer to the philosopher.
 @param max_meals 	The maximum number of meals allowed for the philosopher.

 @return			`0` if the philosopher can start dining;
 					`1` if the philosopher has zero meals to eat and
					logs actions successfully;
					`2` if there is an error in logging the actions.
*/
static int	handle_no_meals(t_philo *philo, int max_meals)
{
	if (max_meals == 0)
	{
		if (FANCY)
			if (print_action(0, philo, STUFFED, 1))
				return (2);
		if (print_action(0, philo, THINK, 1))
			return (2);
		return (1);
	}
	return (0);
}

/**
Logs the philosopher's sleeping action and waits for the specified sleep
duration.

 @param philo 		A pointer to the philosopher.
 @param t_sleep 	The time (in milliseconds) the philosopher spends sleeping.

 @return			`0` on success;
 					`1` if there is an error in logging or waiting.
*/
static int	sleep_and_log(t_philo *philo, int t_sleep)
{
	if (print_action(0, philo, SLEEP, 1))
		return (1);
	if (precise_wait(t_sleep, philo))
		return (1);
	return (0);
}

/**
Logs the philosopher's thinking action and waits for the specified think
duration.

 @param philo 		A pointer to the philosopher.
 @param t_think 	The base time (in millisec) the philosopher spends thinking.

 @return			`0` on success;
 					`1` if there is an error in logging or waiting.
*/
static int	think_and_log(t_philo *philo, int t_think)
{
	if (print_action(0, philo, THINK, 1))
		return (1);
	if (precise_wait(t_think * ALTRUISM_FACTOR, philo))
		return (1);
	return (0);
}

/**
Represents the philosopher's dining routine, which cycles through eating,
sleeping, and thinking actions. The routine terminates if the philosopher dies
or reaches the maximum number of meals.

The function first checks if the philosopher has zero meals to eat.
If not, the philosopher repeatedly attempts to eat, sleep, and think until the
simulation detects a death or the philosopher is full.

Error handling is intentionally omitted, as it falls outside the project's
scope and would be cumbersome due to the project's limitations (cannot use
`pthread_exit()`).

 @param arg 	A void pointer cast to the `t_philo` struct representing
 				the philosopher.

 @return		`NULL` when the philosopher's routine finishes.
*/
void	*eat_sleep_think(void *arg)
{
	t_philo	*philo;
	int		t_eat;
	int		t_sleep;
	int		t_think;
	int		max_meals;

	philo = (t_philo *)arg;
	t_eat = philo->sim->t_eat;
	t_sleep = philo->sim->t_sleep;
	t_think = philo->sim->t_think;
	max_meals = philo->sim->max_meals;
	if (handle_no_meals(philo, max_meals) == 1)
		return (NULL);
	while (stop_sim(philo) == 0)
	{
		(void)pick_forks_and_log(philo);
		(void)eat_and_log(philo, t_eat);
		if (is_philo_full(philo, max_meals) == 1)
			break ;
		(void)sleep_and_log(philo, t_sleep);
		(void)think_and_log(philo, t_think);
	}
	return (NULL);
}
