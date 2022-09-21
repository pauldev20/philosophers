/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 17:03:12 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/21 14:00:11 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	nap(t_philo *philo)
{
	write_thread_msg("is sleeping", philo);
	timesleep(philo->maindata->time_to_sleep);
}

static void	philo_actions(t_philo *philo)
{
	if (philo->id % 2 == 0)
		pthread_mutex_lock(&(philo->maindata->forks[philo->left_fork]));
	else
		pthread_mutex_lock(&(philo->maindata->forks[philo->right_fork]));
	write_thread_msg("has taken a fork", philo);
	if (philo->left_fork == philo->right_fork)
		timesleep(philo->maindata->time_to_die * 2);
	if (philo->left_fork == philo->right_fork)
		return ;
	if (philo->id % 2 == 0)
		pthread_mutex_lock(&(philo->maindata->forks[philo->right_fork]));
	else
		pthread_mutex_lock(&(philo->maindata->forks[philo->left_fork]));
	write_thread_msg("has taken a fork", philo);
	pthread_mutex_lock(&philo->maindata->finishcheck_mutex);
	write_thread_msg("is eating", philo);
	philo->death_time = timenow() + philo->maindata->time_to_die;
	philo->nb_ate++;
	pthread_mutex_unlock(&philo->maindata->finishcheck_mutex);
	timesleep(philo->maindata->time_to_eat);
	pthread_mutex_unlock(&(philo->maindata->forks[philo->left_fork]));
	pthread_mutex_unlock(&(philo->maindata->forks[philo->right_fork]));
	nap(philo);
	write_thread_msg("is thinking", philo);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		usleep(philo->maindata->time_to_eat / 10);
	while (!check_death(philo->maindata, 0))
		philo_actions(philo);
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
		pthread_mutex_lock(&philo->maindata->finishcheck_mutex);
		if (!philo->finished && !check_death(philo->maindata, 0)
			&& (timenow() > philo->death_time))
		{
			check_death(philo->maindata, 1);
			pthread_mutex_lock(&(philo->maindata->write_mutex));
			printf("%ld %d died\n", timenow()
				- philo->maindata->starttime, philo->id);
			pthread_mutex_unlock(&(philo->maindata->write_mutex));
		}
		pthread_mutex_unlock(&philo->maindata->finishcheck_mutex);
		check_must_eat(philo);
		if (i == main->amount)
			i = 0;
		usleep(200);
	}
}
