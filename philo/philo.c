/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 17:03:12 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/13 17:03:40 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*isdead_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	timesleep(philo->maindata->time_to_die + 1);
	pthread_mutex_lock(&(philo->eating));
	pthread_mutex_lock(&philo->maindata->finishcheck_mutex);
	if (!philo->finished && !check_death(philo, 0) && ((gettimems()
				- philo->last_eat) >= (long)(philo->maindata->time_to_die)))
	{
		write_thread_msg("died", philo);
		check_death(philo, 1);
	}
	pthread_mutex_unlock(&(philo->eating));
	pthread_mutex_unlock(&philo->maindata->finishcheck_mutex);
	return (NULL);
}

static void	philo_actions(t_philo *philo)
{
	pthread_mutex_lock(&(philo->maindata->forks[philo->left_fork]));
	write_thread_msg("has taken a fork", philo);
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

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		timesleep(philo->maindata->time_to_eat / 10);
	while (!check_death(philo, 0))
	{
		pthread_create(&philo->death_thread, NULL, isdead_routine, data);
		philo_actions(philo);
		pthread_detach(philo->death_thread);
		philo->nb_ate++;
		check_must_eat(philo);
	}
	return (NULL);
}
