/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtyree <vtyree@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 18:34:22 by vtyree            #+#    #+#             */
/*   Updated: 2022/07/04 14:59:23 by vtyree           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef pthread_mutex_t	t_mutex;

typedef enum bool
{
	false,
	true
}	t_bool;

typedef enum mutex_status
{
	lock,
	unlock
}	t_mstatus;

typedef enum philo_action
{
	has_taken_a_fork,
	is_eating,
	is_sleeping,
	is_thinking,
	is_dead
}	t_action;

typedef struct philoAll
{
	int				number_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_must_eat;
	unsigned long	timestamp;
	t_bool			is_still_alive;
	t_bool			is_well_fed;
	t_mutex			*forks;
	t_mutex			mprint;
	t_mutex			mdead;
	t_mutex			mtest;
	t_mutex			mfed;
}	t_philoAll;

typedef struct philoEach
{
	int				id;
	t_mutex 		*left_fork;
	t_mutex 		*right_fork;
	unsigned long	timestamp;
	int				eat_count;
	t_bool			life_status;
	t_bool			status;
	t_mutex 		mute;
	t_philoAll		*all;
}	t_philoEach;

int				ft_isdigit(int c);
int				ft_atoi(const char *str);
t_bool			checkArguments(int argc, char **argv);
t_bool			validateArguments(t_philoAll *philoAll);

unsigned long	getTimestamp();
void 			ft_usleep(int time);

void 			ft_free(t_philoAll *philoAll, t_philoEach *philoEach, pthread_t *philoThreads);

void			handleState(t_philoAll *philoAll, t_philoEach *philoEach);
void			*philoLife(void *param);

#endif
