/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 14:52:16 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/13 14:05:33 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*isdead_routine(void *data)
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

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	if (philo->id % 2 == 0)
		timesleep(philo->maindata->time_to_eat / 10);
	while (!check_death(philo, 0))
	{
		pthread_create(&philo->death_thread, NULL, isdead_routine, data);
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
		pthread_detach(philo->death_thread);
		philo->nb_ate++;
		check_must_eat(philo);
	}
	return (NULL);
}

int	init(t_main *maindata, int argc, char **argv)
{
	int	i;

	maindata->philos = NULL;
	maindata->forks = NULL;
	if (!check_args(argc, argv))
		return (errorexit("invalid arguments", maindata));
	maindata->amount = ft_atoi(argv[1]);
	maindata->time_to_die = ft_atoi(argv[2]);
	maindata->time_to_eat = ft_atoi(argv[3]);
	maindata->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		maindata->must_eat_count = ft_atoi(argv[5]);
	else
		maindata->must_eat_count = 0;
	maindata->someonedied = 0;
	maindata->philos_finished = 0;
	pthread_mutex_init(&maindata->write_mutex, NULL);
	pthread_mutex_init(&maindata->deadcheck_mutex, NULL);
	pthread_mutex_init(&maindata->finishcheck_mutex, NULL);
	maindata->philos = (t_philo *)malloc(sizeof(t_philo) * maindata->amount);
	if (!maindata->philos)
		return (errorexit("failed allocating storage", maindata));
	maindata->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* maindata->amount);
	if (!maindata->forks)
		return (errorexit("failed allocating storage", maindata));
	maindata->starttime = gettimems();
	i = 0;
	while (i < maindata->amount)
	{
		pthread_mutex_init(&maindata->forks[i], NULL);
		pthread_mutex_init(&maindata->philos[i].eating, NULL);
		maindata->philos[i].id = i + 1;
		maindata->philos[i].maindata = maindata;
		maindata->philos[i].last_eat = maindata->starttime;
		maindata->philos[i].nb_ate = 0;
		maindata->philos[i].finished = 0;
		if (i == 0)
			maindata->philos[i].left_fork = 0;
		else
			maindata->philos[i].left_fork = i;
		if (i + 1 == maindata->amount)
			maindata->philos[i].right_fork = 0;
		else
			maindata->philos[i].right_fork = i + 1;
		i++;
	}
	return (0);
}

int	start_threads(t_main *maindata)
{
	int	i;

	i = 0;
	while (i++ < maindata->amount)
		if (pthread_create(&maindata->philos[i - 1].thread, NULL, philo_routine,
				&maindata->philos[i - 1]) != 0)
			return (errorexit("failed creating thread", maindata));
	i = 0;
	while (i < maindata->amount)
		pthread_join(maindata->philos[i++].thread, NULL);
}

int	main(int argc, char **argv)
{
	t_main	maindata;

	if (argc < 5 || argc > 6)
		return (errorexit("invalid arguments", NULL));
	if (init(&maindata, argc, argv) != 0)
		return (1);
	if (start_threads(&maindata))
		return (1);
	cleanup(&maindata);
	return (0);
}
