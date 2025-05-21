/**
Contains functions that implement the "eat" phase of a philosopher's
dining simulation (eat, sleep, think).
*/

#include "philo.h"

// IN FILE:

int	pick_forks_and_log(t_philo *philo);
int	eat_and_log(t_philo *philo, int t_eat);
int	is_philo_full(t_philo *philo, int max_meals);

/**
Attempts to acquire both forks for the philosopher while logging the action.

If a fork cannot be picked up, the philosopher will spin in a loop,
continuously attempting to acquire the forks. This behavior ensures that the
philosopher can pick up the forks as soon as they are available.

The order of locking is differentiated based on whether the philosopher
is odd or even to prevent lock-order violations (while not necessarily resulting
in deadlocks here, Helgrind / `-fsanitize=thread` checks for this).

 @param philo 	A pointer to the philosopher attempting to pick up the forks.

 @return		`0` on success;
				`1` if there is an error (un)locking a mutex or logging.
*/
static int	pick_forks(t_philo *philo)
{
	if (philo->odd)
	{
		if (mtx_action(&philo->left_fork->fork, LOCK, philo->sim))
			return (1);
		if (print_action(0, philo, FORK_L, 1))
			return (1);
		if (mtx_action(&philo->right_fork->fork, LOCK, philo->sim))
			return (1);
		if (print_action(0, philo, FORK_R, 1))
			return (1);
	}
	else
	{
		if (mtx_action(&philo->right_fork->fork, LOCK, philo->sim))
			return (1);
		if (print_action(0, philo, FORK_R, 1))
			return (1);
		if (mtx_action(&philo->left_fork->fork, LOCK, philo->sim))
			return (1);
		if (print_action(0, philo, FORK_L, 1))
			return (1);
	}
	return (0);
}

/**
Attempts to acquire both forks for the philosopher while logging the action.

If the philosopher is odd, it takes the forks immediately. If the philosopher
is even, it waits briefly before attempting to pick up the forks, ensuring
that all odd philosophers have picked up their forks before the even ones
start trying to acquire theirs.

 @param philo 	A pointer to the philosopher attempting to pick up the forks.

 @return		`0` on success;
				`1` if there is an error (un)locking a mutex or logging.
*/
int	pick_forks_and_log(t_philo *philo)
{
	if (philo->odd)
		return (pick_forks(philo));
	else
	{
		(void)usleep(900);
		return (pick_forks(philo));
	}
}

/**
Releases the left and right forks held by the philosopher.

The order of unlocking is differentiated based on whether the philosopher
is odd or even to prevent lock-order violations (while not necessarily resulting
in deadlocks here, Helgrind / `-fsanitize=thread` checks for this).

 @param philo 	A pointer to the philosopher structure releasing the forks.

 @return		`0` on success;
				`1` if there is an error unlocking a mutex.
*/
static int	drop_forks(t_philo *philo)
{
	if (philo->odd)
	{
		if (mtx_action(&philo->right_fork->fork, UNLOCK, philo->sim))
			return (1);
		if (mtx_action(&philo->left_fork->fork, UNLOCK, philo->sim))
			return (1);
	}
	else
	{
		if (mtx_action(&philo->left_fork->fork, UNLOCK, philo->sim))
			return (1);
		if (mtx_action(&philo->right_fork->fork, UNLOCK, philo->sim))
			return (1);
	}
	return (0);
}

/**
Increments the count of meals eaten by the philosopher, records the
time of the last meal, and logs the eating action. The philosopher
then waits for the specified eating duration before dropping the forks.

In case of an error (whether locking/unlocking the mutex, printing, or
waiting), the forks are always dropped before returning the error code,
ensuring that resources are properly released.

 @param philo 	A pointer to the philosopher eating.
 @param t_eat 	The time (in milliseconds) the philosopher spends eating.

 @return		`0` on success;
				`1` if there is an error (un)locking a mutex, logging,
				or retrieving the time
*/
int	eat_and_log(t_philo *philo, int t_eat)
{
	philo->meals_eaten++;
	if (mtx_action(&philo->mtx_last_meal, LOCK, philo->sim))
	{
		drop_forks(philo);
		return (1);
	}
	philo->t_last_meal = get_time();
	if (philo->t_last_meal == 0)
	{
		(void)mtx_action(&philo->mtx_last_meal, UNLOCK, philo->sim);
		drop_forks(philo);
		return (1);
	}
	if (mtx_action(&philo->mtx_last_meal, UNLOCK, philo->sim)
		|| print_action(0, philo, EAT, 1) || precise_wait(t_eat))
	{
		drop_forks(philo);
		return (1);
	}
	if (drop_forks(philo))
		return (1);
	return (0);
}

/**
Checks if the philosopher has reached the maximum number of meals allowed.
If so, it increments the count of full philosophers and, if FANCY mode is
enabled, logs a message indicating the philosopher is stuffed.

 @param philo 		A pointer to the philosopher eating.
 @param max_meals 	The maximum number of meals allowed for philos.


 @return			`0` if the philosopher is not full yet;
 					`1` if the philosopher is full and has finished eating;
					`2` if there is an error in mutex (un)locking or logging.
*/
int	is_philo_full(t_philo *philo, int max_meals)
{
	if (philo->meals_eaten != max_meals)
		return (0);
	if (mtx_action(&philo->sim->mtx_full_philos, LOCK, philo->sim))
		return (2);
	philo->sim->full_philos++;
	if (mtx_action(&philo->sim->mtx_full_philos, UNLOCK, philo->sim))
		return (2);
	if (FANCY && print_action(0, philo, STUFFED, 1))
		return (2);
	if (print_action(0, philo, THINK, 1))
		return (2);
	return (1);
}
