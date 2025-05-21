/**
Initialization of philosopher structures.
*/

#include "philo.h"

// IN FILE:

int	init_philos(t_sim *sim);

/**
Initializes a philosopher's properties based on its index.

 @param philo 	Pointer to the philosopher to initialize.
 @param sim 	Pointer to the simulation structure.
 @param index 	The index of the philosopher being initialized.

 @return		`0` on success;
				`1` if there is an error initializing the mutex.
*/
static int	initialize_philosopher(t_philo *philo, t_sim *sim, int index,
	int nr_philo)
{
	philo->sim = sim;
	philo->id = index + 1;
	philo->meals_eaten = 0;
	philo->left_fork = &sim->forks[index];
	philo->right_fork = &sim->forks[(index + 1) % nr_philo];
	philo->odd = (philo->id % 2 == 1);
	philo->mtx_last_meal_init = 0;
	if (mtx_action(&philo->mtx_last_meal, INIT, sim))
		return (1);
	philo->mtx_last_meal_init = 1;
	return (0);
}

/**
Initializes the philosophers participating in the simulation by allocating
memory for their structs and setting their initial state, including the
assignment of forks.

 @param sim 	Pointer to the simulation structure containing philosopher data.

 @return		`0` on success;
				`1` if memory allocation or mutex initialization fails.
*/
int	init_philos(t_sim *sim)
{
	int	nr_philo;
	int	i;

	nr_philo = sim->nr_philo;
	sim->philos = malloc(sizeof(t_philo) * nr_philo);
	if (!sim->philos)
	{
		print_err_msg(ERR_MALLOC, sim);
		return (1);
	}
	i = 0;
	while (i < nr_philo)
	{
		if (initialize_philosopher(&sim->philos[i], sim, i, nr_philo))
		{
			while (i > 0)
				(void)mtx_action(&sim->philos[--i].mtx_last_meal, DESTROY, sim);
			free(sim->philos);
			sim->philos = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}
