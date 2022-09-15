/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 17:03:12 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/15 18:28:17 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// static void	*isdead_routine(void *data)
// {
// 	t_philo	*philo;
// 	long	time;

// 	philo = (t_philo *)data;
// 	// timesleep(philo->maindata->time_to_die + 1);
// 	while (!check_death(philo->maindata, 0))
// 	{
// 		pthread_mutex_lock(&(philo->eating));
// 		pthread_mutex_lock(&philo->maindata->finishcheck_mutex);
// 		if (!philo->finished && !check_death(philo->maindata, 0) && ((gettimems()
// 					- philo->last_eat) >= (long)(philo->maindata->time_to_die)))
// 		{
// 			time = gettimems() - philo->maindata->starttime;
// 			pthread_mutex_lock(&philo->maindata->deadcheck_mutex);
// 			pthread_mutex_lock(&(philo->maindata->write_mutex));
// 			philo->maindata->someonedied = 1;
// 			printf("%ld %d died\n", time, philo->id);
// 			pthread_mutex_unlock(&philo->maindata->deadcheck_mutex);
// 			pthread_mutex_unlock(&(philo->maindata->write_mutex));
// 		}
// 		pthread_mutex_unlock(&(philo->eating));
// 		pthread_mutex_unlock(&philo->maindata->finishcheck_mutex);
// 	}
// 	return (NULL);
// }

void	check_dead(t_philo *philo)
{
	long	time;

	pthread_mutex_lock(&(philo->eating));
	pthread_mutex_lock(&philo->maindata->finishcheck_mutex);
	if (!philo->finished && !check_death(philo->maindata, 0) && ((gettimems()
				- philo->last_eat) >= (long)(philo->maindata->time_to_die)))
	{
		time = gettimems() - philo->maindata->starttime;
		check_death(philo->maindata, 1);
		pthread_mutex_lock(&(philo->maindata->write_mutex));
		printf("%ld %d died\n", time, philo->id);
		pthread_mutex_unlock(&(philo->maindata->write_mutex));
	}
	pthread_mutex_unlock(&(philo->eating));
	pthread_mutex_unlock(&philo->maindata->finishcheck_mutex);
}

static void	philo_actions(t_philo *philo)
{
	pthread_mutex_lock(&(philo->maindata->forks[philo->left_fork]));
	write_thread_msg("has taken a fork", philo);
	if (philo->left_fork != philo->right_fork)
	{
		pthread_mutex_lock(&(philo->maindata->forks[philo->right_fork]));
		write_thread_msg("has taken a fork", philo);
		write_thread_msg("is eating", philo);
		pthread_mutex_lock(&(philo->eating));
		philo->last_eat = gettimems();
		pthread_mutex_unlock(&(philo->eating));
		timesleep(philo->maindata->time_to_eat);
		pthread_mutex_unlock(&(philo->maindata->forks[philo->left_fork]));
		pthread_mutex_unlock(&(philo->maindata->forks[philo->right_fork]));
		write_thread_msg("is sleeping", philo);
		timesleep(philo->maindata->time_to_sleep);
		write_thread_msg("is thinking", philo);
	}
	if (philo->left_fork == philo->right_fork)
		timesleep(philo->maindata->time_to_die);
	if (philo->left_fork == philo->right_fork)
		pthread_mutex_unlock(&(philo->maindata->forks[philo->left_fork]));
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (!philo->maindata->start)
		usleep(20);
	if (philo->id % 2 == 0)
		timesleep(philo->maindata->time_to_eat / 10);
	while (!check_death(philo->maindata, 0))
	{
		philo_actions(philo);
		philo->nb_ate++;
		check_must_eat(philo);
	}
	return (NULL);
}
