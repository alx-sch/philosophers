/**
Includes general-purpose functions usually included within the libft library.
*/

#include "philo.h"

// IN FILE:

int		ft_atoi(const char *str);
void	ft_putstr_fd(char *s, int fd);
int		contains_digit(const char *str);

/**
Converts a string to its corresponding integer representation (base 10).

 @param str 	Pointer to the string representing the number.

 @return		The integer representation of the number found in the string;
				`0`, if no valid digits are found.
*/
int	ft_atoi(const char *str)
{
	int	sign;
	int	res;

	sign = 1;
	res = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = sign * -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + *str - '0';
		str++;
	}
	return (res * sign);
}

/**
Writes a string to the given file descriptor.

 @param s 	The string to print.
 @param fd 	The file descriptor on which to write.
*/
void	ft_putstr_fd(char *s, int fd)
{
	char	*ptr;

	ptr = s;
	while (*ptr)
	{
		(void)write(fd, ptr, 1);
		ptr++;
	}
}

/**
Checks if the given string contains any digit (0-9).

 @param str 	A pointer to the string to be checked.

 @return		`0` if the string contains at least one digit;
				`1` if no digits are found.
*/
int	contains_digit(const char *str)
{
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
			return (0);
		str++;
	}
	return (1);
}
