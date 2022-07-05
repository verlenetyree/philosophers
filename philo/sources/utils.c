/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: margaritasoldatkina <margaritasoldatkin    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 18:42:53 by vtyree            #+#    #+#             */
/*   Updated: 2022/07/01 10:26:19 by margaritaso      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	n;

	i = 0;
	sign = 1;
	n = 0;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v' || \
		str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			sign *= -1;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		n = (str[i] - '0') + (n * 10);
		i++;
	}
	return (sign * n);
}

static t_bool checkIfDigit(char *elem)
{
	while(*elem)
	{
		if (*elem == '+' || *elem == '-')
			elem++;
		while(*elem)
		{
			if (!ft_isdigit(*elem))
				return(false);
			elem++;
		}
	}
	return (true);
}

t_bool	checkArguments(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf("5 or 6 arguments are required!\n");
		return (false);
	}
	i = 1;
	while (i < argc)
	{
		if (checkIfDigit(argv[i]) == false)
		{
			printf("Value %s is not a number!\n", argv[i]);
			return(false);
		}
		i++;
	}
	return (true);
}

t_bool	validateArguments(t_philoAll *philoAll)
{
	if (philoAll->number_of_philo < 1)
	{
		printf("There should be at least 1 philosopher.\n");
		return (false);
	}
	if (philoAll->number_of_philo > 200)
	{
		printf("Do not test with more than 200 philosophers.\n");
		return (false);
	}
	if (philoAll->time_to_die < 60 || philoAll->time_to_eat < 60 || \
	philoAll->time_to_sleep < 60)
	{
		printf("Do not test with time values under 60 ms.\n");
		return (false);
	}
	return (true);
}
