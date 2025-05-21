/**
This header file defines custom types and data structures.
*/

#ifndef TYPES_H
# define TYPES_H

# include <pthread.h> // Thread functions, like pthread_create() etc.

typedef pthread_mutex_t		t_mtx; // Abbreviation for 'pthread_mutex_t'
typedef unsigned long long	t_ull; // Abbreviation for 'unsigned long long'

/**
Structure representing the overall simulation state:
 - nr_philo:	Total number of philosophers participating in the simulation.
 - t_die:		Time in milliseconds after which a philosopher dies if they
				haven't start eating.
 - t_eat:		Time in milliseconds a philosopher takes to eat.
 - t_sleep:		Time in milliseconds a philosopher sleeps after eating.
 - t_think:		Philosophers are altruistic in this simulation, meaning they
 				delay taking forks and eating for as long as possible without
				starving. This behavior helps keep every philosopher alive as
				long as possible, especially when there's an odd number of
				philosophers: t_think = t_die - t_eat - t_sleep.
 - max_meals:	Max. number of meals a philosopher can eat before they stop
 				dining; `-1` means unlimited meals.
 - full_philos:	The number of philosophers who have eaten their maximum number
 				of meals (if specified) and have stopped eating.
 - stop_sim:	Flag indicating if to stop the simulation.
 - t_start_sim:	Timestamp in milliseconds for when the simulation started.
 - forks:		Array of forks available for the philosophers.
 - philos:		Array of philosophers participating in the simulation.
 - monitor:		Thread handle for the monitoring thread that observes the
				state of the simulation.
 - mtx_print:	Mutex for synchronizing output to the console (printing).
 - mtx_print_init:	Flag checking if 'print' mutex has been initialized.
 - mtx_full_philos:	Mutex for checking / increasing 'full_philos'.
 - mtx_full_philos_init:	Flag checking if 'full_philos' mutex has been
 							initialized.
 - mtx_stop_sim:		Mutex for synchronizing access to the `stop_sim` flag.
 - mtx_stop_sim_init:	Flag checking if 'stop_sim' mutex has been initialized.
*/
typedef struct s_sim		t_sim;

/**
Enumeration for representing various actions that can be performed on a mutex:
 - INIT:	Initializes the mutex.
 - LOCK:	Locks the mutex to ensure exclusive access.
 - UNLOCK:	Unlocks the mutex, allowing access to other threads.
 - DESTROY:	Destroys the mutex and releases associated resources.
*/
typedef enum e_mtx_action
{
	INIT = 1,
	LOCK,
	UNLOCK,
	DESTROY
}	t_mtx_action;

/**
Enumeration for representing the various actions a philosopher can take:
 - FORK_L:	Takes their left fork.
 - FORK_R: 	Takes their right fork.
 - EAT:		Starts eating.
 - SLEEP:	Starts sleeping.
 - THINK:	Starts thinking.
 - DIE:		Has died.
 - STUFFED:	Has eaten all their meals.
*/
typedef enum e_action
{
	FORK_L,
	FORK_R,
	EAT,
	SLEEP,
	THINK,
	DIE,
	STUFFED
}	t_action;

/**
Structure representing a fork in the dining philosophers problem:
 - fork_id:	An identifier for the fork.
 - fork:	A mutex associated with the fork to control access.
*/
typedef struct s_fork
{
	int		fork_id;
	t_mtx	fork;
}	t_fork;

/**
Structure representing a philosopher in the dining philosophers problem:
 - sim:			A pointer to the simulation structure, allowing access
				to shared data.
 - id:			The philosopher's identifier.
 - odd:			A flag indicating if a philo's ID is odd or not.
 - thread_id:	The identifier for the philosopher's thread.
 - meals_eaten:	Counter for the number of meals the philosopher has eaten.
 - t_last_meal:	Timestamp of the philosopher's last meal in milliseconds.
 - mtx_last_meal:		Mutex for synchronizing access to `t_last_meal`.
 - mtx_last_meal_init:	Flag checking if `mtx_last_meal` has been initialized.
 - timestamp_death:		Timestamp in milliseconds for when the philosopher dies.
 - left_fork:	A pointer to the philosopher's left fork.
 - right_fork:	A pointer to the philosopher's right fork.
*/
typedef struct s_philo
{
	t_sim		*sim;
	int			id;
	int			odd;
	pthread_t	thread_id;
	int			meals_eaten;
	t_ull		t_last_meal;
	t_mtx		mtx_last_meal;
	int			mtx_last_meal_init;
	t_ull		timestamp_death;
	t_fork		*left_fork;
	t_fork		*right_fork;
}	t_philo;

/**
Structure representing the overall simulation state:
 - nr_philo:	Total number of philosophers participating in the simulation.
 - t_die:		Time in milliseconds after which a philosopher dies if they
				haven't start eating.
 - t_eat:		Time in milliseconds a philosopher takes to eat.
 - t_sleep:		Time in milliseconds a philosopher sleeps after eating.
 - t_think:		Philosophers are altruistic in this simulation, meaning they
 				delay taking forks and eating for as long as possible without
				starving. This behavior helps keep every philosopher alive as
				long as possible, especially when there's an odd number of
				philosophers: t_think = t_die - t_eat - t_sleep.
 - max_meals:	Max. number of meals a philosopher can eat before they stop
 				dining; `-1` means unlimited meals.
 - full_philos:	The number of philosophers who have eaten their maximum number
 				of meals (if specified) and have stopped eating.
 - stop_sim:	Flag indicating if to stop the simulation.
 - t_start_sim:	Timestamp in milliseconds for when the simulation started.
 - forks:		Array of forks available for the philosophers.
 - philos:		Array of philosophers participating in the simulation.
 - monitor:		Thread handle for the monitoring thread that observes the
				state of the simulation.
 - mtx_print:	Mutex for synchronizing output to the console (printing).
 - mtx_print_init:	Flag checking if 'print' mutex has been initialized.
 - mtx_full_philos:	Mutex for checking / increasing 'full_philos'.
 - mtx_full_philos_init:	Flag checking if 'full_philos' mutex has been
 							initialized.
 - mtx_stop_sim:		Mutex for synchronizing access to the `stop_sim` flag.
 - mtx_stop_sim_init:	Flag checking if 'stop_sim' mutex has been initialized.
*/
typedef struct s_sim
{
	int			nr_philo;
	int			t_die;
	int			t_eat;
	int			t_sleep;
	int			t_think;
	int			max_meals;
	int			full_philos;
	int			stop_sim;
	t_ull		t_start_sim;
	t_fork		*forks;
	t_philo		*philos;
	pthread_t	monitor;
	t_mtx		mtx_print;
	int			mtx_print_init;
	t_mtx		mtx_full_philos;
	int			mtx_full_philos_init;
	t_mtx		mtx_stop_sim;
	int			mtx_stop_sim_init;
}	t_sim;

#endif
