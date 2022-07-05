/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: margaritasoldatkina <margaritasoldatkin    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 21:34:30 by vtyree            #+#    #+#             */
/*   Updated: 2022/07/05 20:14:18 by margaritaso      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

//void	change_life_status(t_philoAll *philoAll, t_philoEach *philoEach)
//{
//	int i;

//	i = -1;
//	while (++i < philoAll->number_of_philo)
//		philoEach[i].life_status = false;
//}

t_bool	checkLifeStatus(t_philoEach *philoEach)
{
	t_bool	status;

	//pthread_mutex_lock(&(philoEach->mute));
	pthread_mutex_lock(&(philoEach->all->mdead));
	status = philoEach->all->is_still_alive;				// data race
	//status = philoEach->life_status;						// data race
	pthread_mutex_unlock(&(philoEach->all->mdead));
	//pthread_mutex_unlock(&(philoEach->mute));
	return (status);
}

void printAction(t_philoEach *philoEach, int status)
{
	unsigned long	timestamp;

	timestamp = getTimestamp() - philoEach->all->timestamp;
	pthread_mutex_lock(&(philoEach->all->mprint));
	if (checkLifeStatus(philoEach))
	{
		if (status == has_taken_a_fork)
			printf("\e[1;34m%lu\e[0m %d has taken a fork\n", timestamp, philoEach->id);
		else if (status == is_eating)
			printf("\e[1;34m%lu\e[0m %d is eating\n", timestamp, philoEach->id);
		else if (status == is_sleeping)
			printf("\e[1;34m%lu\e[0m %d is sleeping\n", timestamp, philoEach->id);
		else if (status == is_thinking)
			printf("\e[1;34m%lu\e[0m %d is thinking\n", timestamp, philoEach->id);
	}
	else if (status == is_dead)
		printf("\e[31m%lu %d died\e[0m\n", timestamp, philoEach->id);
	pthread_mutex_unlock(&(philoEach->all->mprint));
}

void philoTakeForks(t_philoEach *philoEach, int status)
{
	if (!status)
	{
		pthread_mutex_lock(philoEach->right_fork);
		printAction(philoEach, has_taken_a_fork);
		pthread_mutex_lock(philoEach->left_fork);
		printAction(philoEach, has_taken_a_fork);
	}
	else
	{
		pthread_mutex_unlock(philoEach->left_fork);
		pthread_mutex_unlock(philoEach->right_fork);
	}
}

t_bool checkIfStillAlive(t_philoEach *philoEach)
{
	int diff;

	diff = getTimestamp() - philoEach->timestamp;
	if (diff > philoEach->all->time_to_die)
		return (false);
	return (true);
}

void	handleState(t_philoAll *philoAll, t_philoEach *philoEach)
{
	int	i;

	while (!philoAll->is_well_fed)
	{
		i = 0;
		while (i < philoAll->number_of_philo && philoAll->is_still_alive)
		{
			//pthread_mutex_lock(&(philoAll->mfed));
			pthread_mutex_lock(&(philoEach[i].mute));
			if (!(checkIfStillAlive(&philoEach[i])))
			{
				pthread_mutex_lock(&(philoAll->mdead));
				philoAll->is_still_alive = false;
				//change_life_status(philoAll, philoEach);
				pthread_mutex_unlock(&(philoAll->mdead));
				printAction(&philoEach[i], is_dead);
			}
			//pthread_mutex_unlock(&(philoAll->mfed));
			pthread_mutex_unlock(&(philoEach[i].mute));
			usleep(1000);
			i++;
		}
		if (!philoAll->is_still_alive)
			return ;
		i = 0;
		pthread_mutex_lock(&(philoEach->all->mtest));
		while (i < philoAll->number_of_philo && philoEach[i].status == true)
				i++;
		if (i == philoAll->number_of_philo)
			philoAll->is_well_fed = true;
		pthread_mutex_unlock(&(philoEach->all->mtest));
	}
}

void *philoLife(void *param)
{
	int			n_to_eat;
	t_philoEach	*philoEach;

	philoEach = (t_philoEach *)param;
	n_to_eat = philoEach->all->number_must_eat;
	if (philoEach->id % 2)
		ft_usleep(philoEach->all->time_to_eat);
	while (checkLifeStatus(philoEach))
	{
		philoTakeForks(philoEach, lock);
		//pthread_mutex_lock(&(philoEach->all->mfed));
		pthread_mutex_lock(&(philoEach->mute));
		printAction(philoEach, is_eating);
		philoEach->timestamp = getTimestamp();
		pthread_mutex_unlock(&(philoEach->mute));
		//pthread_mutex_unlock(&(philoEach->all->mfed));
		ft_usleep(philoEach->all->time_to_eat);
		philoTakeForks(philoEach, unlock);
		pthread_mutex_lock(&(philoEach->all->mtest));
		philoEach->eat_count++;
		if (philoEach->eat_count == n_to_eat)
			philoEach->status = true;
		pthread_mutex_unlock(&(philoEach->all->mtest));
		printAction(philoEach, is_sleeping);
		ft_usleep(philoEach->all->time_to_sleep);
		printAction(philoEach, is_thinking);
	}
	return (NULL);
}
