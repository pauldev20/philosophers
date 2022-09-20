/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 17:03:12 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/21 00:49:03 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_actions(t_philo *philo)
{
	pthread_mutex_lock(&(philo->maindata->forks[philo->left_fork]));
	write_thread_msg("has taken a fork", philo);
	if (philo->left_fork == philo->right_fork)
		timesleep(philo->maindata->time_to_die * 2);
	if (philo->left_fork == philo->right_fork)
		return ;
	pthread_mutex_lock(&(philo->maindata->forks[philo->right_fork]));
	write_thread_msg("has taken a fork", philo);
	write_thread_msg("is eating", philo);
	pthread_mutex_lock(&(philo->eating));
	philo->last_eat = timenow();
	philo->nb_ate++;
	pthread_mutex_unlock(&(philo->eating));
	timesleep(philo->maindata->time_to_eat);
	pthread_mutex_unlock(&(philo->maindata->forks[philo->left_fork]));
	pthread_mutex_unlock(&(philo->maindata->forks[philo->right_fork]));
	write_thread_msg("is sleeping", philo);
	timesleep(philo->maindata->time_to_sleep);
	write_thread_msg("is thinking", philo);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (!philo->maindata->start)
		usleep(10);
	if (!(philo->id % 2))
		timesleep(philo->maindata->time_to_eat);
	while (!check_death(philo->maindata, 0) && !philo->finished)
	{
		philo_actions(philo);
		if (check_must_eat(philo))
			return (NULL);
	}
	return (NULL);
}

void	check_dead(t_main *main)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (!main->someonedied)
	{
		philo = &main->philos[i++];
		pthread_mutex_lock(&(philo->eating));
		pthread_mutex_lock(&philo->maindata->finishcheck_mutex);
		if (!philo->finished && !check_death(philo->maindata, 0) && ((timenow()
					- philo->last_eat) >= (long)(philo->maindata->time_to_die)))
		{
			check_death(philo->maindata, 1);
			pthread_mutex_lock(&(philo->maindata->write_mutex));
			printf("%ld %d died\n", timenow()
				- philo->maindata->starttime, philo->id);
			pthread_mutex_unlock(&(philo->maindata->write_mutex));
		}
		pthread_mutex_unlock(&(philo->eating));
		pthread_mutex_unlock(&philo->maindata->finishcheck_mutex);
		if (i == main->amount)
			i = 0;
		usleep(25);
	}
}
