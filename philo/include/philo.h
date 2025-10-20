/**
The primary header file for the dining philosopher simulation.

As it aggregates all necessary headers and declarations, `philo.h` ensures
that each source file within the project has access to all definitions without
redundant includes.
*/

#ifndef PHILO_H
# define PHILO_H

# include "types.h" // All custom types and data structs, e.g. 't_sim'
# include "errors.h" // Error messages and respective formatting

# include <stdio.h> // perror(), printf()
# include <string.h> // memset()
# include <stdlib.h> // malloc(), free()
# include <unistd.h> // write(), usleep()
# include <sys/time.h> // gettimeofday()

//	+++++++++++++++
//	++ CONSTANTS ++
//	+++++++++++++++

/**
Used in `precise_wait()` to divide the total wait time into smaller sleep
intervals.

Precision vs. CPU Load Trade-off:
 -	Larger SLEEP_INTERVALS (smaller sleep durations): Higher precision, but
	increased CPU usage.
 -	Smaller SLEEP_INTERVALS (larger sleep durations): Reduced precision, but
 	lower CPU usage.

If `SLEEP_INTERVALS = 1`, the behavior would be similar to a direct call to
`usleep()`, without the added precision checks.

Value fine-tuned to '100,000' using `test_usleep.c`;
see https://github.com/alx-sch/42_philosophers.
*/
# define SLEEP_INTERVALS	100000

/**
Represents the level of "altruism" or "greed" exhibited by the philosophers in
their decision to eat. It scales the time they wait before attempting to eat,
controlling whether they immediately grab forks or delay their actions in favor
of others.

An ALTRUISM_FACTOR of '0' means the philosopher is purely greedy, always
attempting to eat as soon as possible, which leads to starvation risk for others,
especially with an odd number of philosophers.

An ALTRUISM_FACTOR of '1' means the philosopher is entirely altruistic,
pushing the limits of how long they delay eating (close to starving themselves)
to give others a chance to eat first. This can (and eventually will) lead to
starvation for the thinking philosopher due to the inherent delays during the
simulation.

Choosing an ALTRUISM_FACTOR of 0.9 strikes a balance between self-interest and
altruism, allowing philosophers to delay their eating just enough to minimize
the risk of starvation among others while still prioritizing their own needs.
*/
# define ALTRUISM_FACTOR		0.9

//	+++++++++++++++
//	++ FUNCTIONS ++
//	+++++++++++++++

// 1_check_args.c

int		check_args(int argc, char **argv);

// 2_init_philos.c

int		init_philos(t_sim *sim);

// 2_init_sim.c

int		init_sim(t_sim **sim, int argc, char **argv);

// 3_eat.c

int		pick_forks_and_log(t_philo *philo);
int		eat_and_log(t_philo *philo, int t_eat);
int		is_philo_full(t_philo *philo, int max_meals);

// 3_eat_sleep_think.c

void	*eat_sleep_think(void *arg);
int		stop_sim(t_philo *philo);

// 4_monitor.c

void	*monitor(void *arg);

// 5_run_sim_utils.c

int		simulate_single_philo(t_sim *sim);

// 5_run_sim.c

int		run_sim(t_sim *sim);

// utils/0_libft.c

int		ft_atoi(const char *str);
void	ft_putstr_fd(char *s, int fd);
int		contains_digit(const char *str);

// utils/1_print_action.c

int		print_action(t_ull timestamp, t_philo *philo, t_action action,
			int update_timestamp);

// utils/1_print_error.c

void	print_err_msg(char *msg, t_sim *sim);
void	print_usage(void);

// utils/2_mutex.c

int		mtx_action(t_mtx *mutex, t_mtx_action action, t_sim *sim);

// utils/3_time.c

t_ull	get_time(void);
int		set_start_time(t_sim *sim);
int		record_time_of_death(t_philo *philo);
int		precise_wait(int duration_to_wait, t_philo *philo);

// utils/4_free.c

void	cleanup_sim(t_sim **sim_ptr);

#endif
