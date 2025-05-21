/**
This header file defines constants for text formatting and error messages.
*/

#ifndef ERRORS_H
# define ERRORS_H

# define BOLD 			"\033[1m"
# define ERR_COLOR		"\033[91m\033[1m" // red and bold
# define YELLOW			"\033[33m\033[1m" // yellow and bold
# define RESET			"\033[0m"

# define ERR_ARGS_NR	"ERROR: Expected 4 or 5 arguments.\n"
# define ERR_ARGS_VAL_1	"ERROR: Arguments must be integers >= 0 "
# define ERR_ARGS_VAL_2	"(but at least one philosopher).\n\n"
# define ERR_ARGS_MIN_P	"ERROR: At least one philosopher must dine.\n"

# define ERR_MALLOC		"ERROR: malloc() failed."

# define ERR_MTX_INIT	"ERROR: pthread_mutex_init() failed."
# define ERR_MTX_LOCK	"ERROR: pthread_mutex_lock() failed."
# define ERR_MTX_UNLOCK	"ERROR: pthread_mutex_unlock() failed."
# define ERR_MTX_DESTR	"ERROR: pthread_mutex_destroy() failed."

# define ERR_TR_CREATE	"ERROR: pthread_create() failed."
# define ERR_TR_JOIN	"ERROR: pthread_join() failed."

# define ERR_GET_TIME	"ERROR: gettimeofday() failed."

#endif
