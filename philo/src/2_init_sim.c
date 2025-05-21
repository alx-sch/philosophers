/**
Creating the fork structures and intializing the simulation state.
*/

#include "philo.h"

// IN FILE:

int	init_sim(t_sim **sim, int argc, char **argv);

/**
Initializes the simulation parameters from command line arguments.

Checks the validity of the arguments, converts them to integers,
and assigns them to the corresponding fields in the simulation structure.

 @param sim 	Pointer to the simulation structure to initialize.
 @param argc 	The argument count from the command line.
 @param argv 	The argument vector from the command line.

 @return		`0` on success;
				`1` if any of the arguments are invalid.
*/
static int	init_args(t_sim *sim, int argc, char **argv)
{
	if (check_args(argc, argv))
		return (1);
	sim->nr_philo = ft_atoi(argv[1]);
	sim->t_die = ft_atoi(argv[2]);
	sim->t_eat = ft_atoi(argv[3]);
	sim->t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		sim->max_meals = ft_atoi(argv[5]);
	else
		sim->max_meals = -1;
	return (0);
}

/**
Sets initial values for the simulation state and initializes mutexes.

 @param sim 	Pointer to the simulation structure to initialize.

 @return		`0` on success;
				`1` if argument initialization or mutex initialization fails.
*/
static int	init_sim_state(t_sim *sim, int argc, char **argv)
{
	sim->full_philos = 0;
	sim->stop_sim = 0;
	sim->forks = NULL;
	sim->philos = NULL;
	sim->mtx_print_init = 0;
	sim->mtx_full_philos_init = 0;
	sim->mtx_stop_sim_init = 0;
	if (init_args(sim, argc, argv))
		return (1);
	sim->t_think = sim->t_die - sim->t_eat - sim->t_sleep;
	if (mtx_action(&sim->mtx_print, INIT, NULL))
		return (1);
	sim->mtx_print_init = 1;
	if (mtx_action(&sim->mtx_full_philos, INIT, sim))
		return (1);
	sim->mtx_full_philos_init = 1;
	if (mtx_action(&sim->mtx_stop_sim, INIT, sim))
		return (1);
	sim->mtx_stop_sim_init = 1;
	return (0);
}

/**
This function allocates memory for the forks used by philosophers in the
simulation, and initializes their mutexes.

 @param sim 	Pointer to the simulation structure containing fork data.

 @return		`0` on success;
				`1` if memory allocation or mutex initialization fails.
*/
static int	init_forks(t_sim *sim)
{
	int	nr_philo;
	int	i;

	nr_philo = sim->nr_philo;
	sim->forks = malloc(sizeof(t_fork) * nr_philo);
	if (!sim->forks)
	{
		print_err_msg(ERR_MALLOC, sim);
		return (1);
	}
	i = 0;
	while (i < nr_philo)
	{
		if (mtx_action(&sim->forks[i].fork, INIT, sim))
		{
			while (i > 0)
				(void)mtx_action(&sim->forks[--i].fork, DESTROY, sim);
			free(sim->forks);
			sim->forks = NULL;
			return (1);
		}
		sim->forks[i].fork_id = i + 1;
		i++;
	}
	return (0);
}

/**
Initializes the simulation by allocating memory for the simulation structure
and setting up the initial state, forks, and philosophers.

 @param sim 	Double pointer to the simulation structure to be initialized.
 @param argc 	The argument count from the command line.
 @param argv 	The argument vector from the command line.

 @return		`0` on success;
				`1` if memory allocation or an initialization step fails.
*/
int	init_sim(t_sim **sim, int argc, char **argv)
{
	*sim = malloc(sizeof(t_sim));
	if (!(*sim))
	{
		print_err_msg(ERR_MALLOC, *sim);
		return (1);
	}
	if (init_sim_state(*sim, argc, argv))
		return (1);
	if (init_forks(*sim))
		return (1);
	if (init_philos(*sim))
		return (1);
	return (0);
}
