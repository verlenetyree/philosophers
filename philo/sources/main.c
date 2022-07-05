/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtyree <vtyree@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 21:34:00 by vtyree            #+#    #+#             */
/*   Updated: 2022/07/04 15:00:53 by vtyree           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

static t_bool initMutexes(t_philoAll *philoAll)
{
	int 	i;
	int 	num;

	i = 0;
	num = philoAll->number_of_philo;
	while (i < num)
	{
		if (pthread_mutex_init(&philoAll->forks[i], NULL))
			return (false);
		i++;
	}
	if (pthread_mutex_init(&philoAll->mprint, NULL))
		return (false);
	if (pthread_mutex_init(&philoAll->mdead, NULL))
		return (false);
	if (pthread_mutex_init(&philoAll->mtest, NULL))
		return (false);
	if (pthread_mutex_init(&philoAll->mfed, NULL))
		return (false);
	return (true);
}

static t_bool	initPhiloAllStructure(int argc, char **argv, t_philoAll *philoAll)
{
	philoAll->number_of_philo = ft_atoi(argv[1]);
	philoAll->time_to_die = ft_atoi(argv[2]);
	philoAll->time_to_eat = ft_atoi(argv[3]);
	philoAll->time_to_sleep = ft_atoi(argv[4]);
	philoAll->number_must_eat = -1;
	if (argc == 6)
		philoAll->number_must_eat = ft_atoi(argv[5]);
	if (!validateArguments(philoAll))
		return (false);
	philoAll->timestamp = getTimestamp();
	philoAll->is_still_alive = true;
	philoAll->is_well_fed = false;
	philoAll->forks = malloc(sizeof(t_mutex) * philoAll->number_of_philo);
	if (!philoAll->forks)
		return (false);
	if (!initMutexes(philoAll))
		return (false);
	return (true);
}

static t_bool	initPhiloEachStructure(t_philoAll *philoAll, t_philoEach *philoEach, int i)
{
	philoEach->id = i + 1;
	philoEach->right_fork = &philoAll->forks[i];
	philoEach->left_fork = &philoAll->forks[(i + 1) % philoAll->number_of_philo];
	philoEach->eat_count = 0;
	philoEach->life_status = true;
	philoEach->status = false;
	philoEach->timestamp = philoAll->timestamp;
	if (pthread_mutex_init(&philoEach->mute, NULL))
		return (false);
	philoEach->all = philoAll;
	return (true);
}


static t_bool createThreads(t_philoAll *philoAll, t_philoEach *philoEach, pthread_t *philoThreads)
{
	int	i;
	int	num;

	i = -1;
	num = philoAll->number_of_philo;
	while (++i < num)
	{
		if (!initPhiloEachStructure(philoAll, &philoEach[i], i))
			return(false);
		if (pthread_create(&philoThreads[i], NULL, philoLife, &philoEach[i]))
			return(false);
	}
	handleState(philoAll, philoEach);
	i = -1;
	while (++i < num)
	{
		if (pthread_detach(philoThreads[i]))
			return (false);
	}
	return (true);
}

int	main(int argc, char **argv)
{
	t_philoAll	philoAll;
	t_philoEach	*philoEach;
	pthread_t 	*philoThreads;

	if (!checkArguments(argc, argv))
		return (0);
	if(!initPhiloAllStructure(argc, argv, &philoAll))
		return (1);
	philoEach = malloc(sizeof(t_philoEach) * philoAll.number_of_philo);
	if (!philoEach)
		return (1);
	philoThreads = malloc(sizeof(pthread_t) * philoAll.number_of_philo);
	if (!philoThreads)
		return (1);
	if (!createThreads(&philoAll, philoEach, philoThreads))
		return (1);
	ft_free(&philoAll, philoEach, philoThreads);
	return (0);
}
