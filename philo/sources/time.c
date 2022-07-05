
#include "../includes/philosophers.h"

unsigned long	getTimestamp()
{
	unsigned long	timestamp;
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	timestamp = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (timestamp);
}

void ft_usleep(int time)
{
	unsigned long	start_time;
	unsigned long	current_time;
	struct timeval	now;

	start_time = getTimestamp();
	gettimeofday(&now, NULL);
	current_time = (now.tv_sec * 1000) + (now.tv_usec / 1000);
	while (current_time - start_time < (unsigned long)time)
	{
		usleep(50);
		gettimeofday(&now, NULL);
		current_time = (now.tv_sec * 1000) + (now.tv_usec / 1000);
	}
}

void ft_free(t_philoAll *philoAll, t_philoEach *philoEach, pthread_t *philoThreads)
{
	int i;

	i = 0;
	while(i < philoAll->number_of_philo)
	{
		pthread_mutex_destroy(&philoAll->forks[i]);
		i++;
	}
	free(philoAll->forks);
	free(philoThreads);
	free(philoEach);
}
