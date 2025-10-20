/**
Includes a function for simulating the edge case of a single philosopher
and a single fork in the dining philosophers problem (`nr_philo` = 1).
*/

#include "philo.h"

// IN FILE:

int	simulate_single_philo(t_sim *sim);

/**
Simulates the behavior when only a single philosopher and a single fork are
present. The philosopher takes the fork and waits for the specified time
(`t_die`). After this period, it logs their death.

 @param sim 	Pointer to the simulation structure.

 @return		`0` on success;
				`1` if there was an error during time retrieval.
*/
int	simulate_single_philo(t_sim *sim)
{
	int	t_die;

	t_die = sim->t_die;
	if (FANCY == 0)
	{
		(void)printf("0\t1\thas taken their fork\n");
		if (precise_wait(t_die, NULL))
			return (1);
		(void)printf(ERR_COLOR"%d\t1\tdied\n"RESET, t_die);
	}
	else
	{
		(void)printf("0\t1\t🥄←\thas taken their fork\t(1) left\n");
		if (precise_wait(t_die, NULL))
			return (1);
		(void)printf(ERR_COLOR"%d\t1\t💀\tdied\n"RESET, t_die);
	}
	return (0);
}
