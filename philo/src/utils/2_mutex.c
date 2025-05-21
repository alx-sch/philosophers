/**
This file provides functions for handling mutex operations (initialization,
locking, unlocking, and destruction) with proper error checking and reporting.
*/

#include "philo.h"

// IN FILE:

int	mtx_action(t_mtx *mutex, t_mtx_action action, t_sim *sim);

/**
Performs an action on a mutex based on the specified action.

 @param mutex 	Pointer to a `pthread_mutex_t` mutex to operate on.
 @param action 	The action to perform on the mutex:
				INIT, LOCK, UNLOCK, DESTROY

 @return		`0` on success;
				INIT`, `LOCK`, `UNLOCK`, or `DESTROY` on failure,
				corresponding to the failed operation.
*/
static int	mtx_perform_action(t_mtx *mutex, t_mtx_action action)
{
	if (action == INIT)
	{
		if (pthread_mutex_init(mutex, NULL) != 0)
			return (INIT);
	}
	else if (action == LOCK)
	{
		if (pthread_mutex_lock(mutex) != 0)
			return (LOCK);
	}
	else if (action == UNLOCK)
	{
		if (pthread_mutex_unlock(mutex) != 0)
			return (UNLOCK);
	}
	else if (action == DESTROY)
	{
		if (pthread_mutex_destroy(mutex) != 0)
			return (DESTROY);
	}
	return (0);
}

/**
Handles mutex operations with error checking and error message printing.

 @param mutex 	Pointer to a `pthread_mutex_t` mutex to operate on.
 @param action 	The action to perform on the mutex:
				INIT, LOCK, UNLOCK, DESTROY.
 @param sim 	Pointer to the data struct containing the mutex for printing
 				(error message).

 @return		`0` on success;
				`1` if the mutex operation failed.
*/
int	mtx_action(t_mtx *mutex, t_mtx_action action, t_sim *sim)
{
	int	error;

	error = mtx_perform_action(mutex, action);
	if (error != 0)
	{
		if (error == INIT)
			print_err_msg(ERR_MTX_INIT, sim);
		else if (error == LOCK)
			print_err_msg(ERR_MTX_LOCK, sim);
		else if (error == UNLOCK)
			print_err_msg(ERR_MTX_UNLOCK, sim);
		else if (error == DESTROY)
			print_err_msg(ERR_MTX_DESTR, sim);
		return (1);
	}
	return (0);
}
