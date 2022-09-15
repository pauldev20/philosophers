/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 14:52:16 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/15 18:29:38 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philo(t_main *maindata, int nb)
{
	maindata->philos[nb].nb_ate = 0;
	maindata->philos[nb].id = nb + 1;
	maindata->philos[nb].finished = 0;
	maindata->philos[nb].maindata = maindata;
	maindata->philos[nb].last_eat = maindata->starttime;
	pthread_mutex_init(&maindata->philos[nb].eating, NULL);
	if (nb == 0)
		maindata->philos[nb].left_fork = 0;
	else
		maindata->philos[nb].left_fork = nb;
	if (nb + 1 == maindata->amount)
		maindata->philos[nb].right_fork = 0;
	else
		maindata->philos[nb].right_fork = nb + 1;
	pthread_mutex_init(&maindata->forks[nb], NULL);
}

static int	init_main(t_main *maindata)
{
	maindata->start = 0;
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
	return (0);
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
	i = init_main(maindata);
	if (i != 0)
		return (i);
	maindata->starttime = gettimems();
	i = 0;
	while (i < maindata->amount)
		init_philo(maindata, i++);
	return (0);
}

int	start_threads(t_main *maindata)
{
	int	i;

	i = 0;
	while (i++ < maindata->amount) {
		if (pthread_create(&maindata->philos[i - 1].thread, NULL, philo_routine,
				&maindata->philos[i - 1]) != 0)
			return (errorexit("failed creating thread", maindata));
		usleep(50);
	}
	maindata->starttime = gettimems();
	i = 0;
	while (i < maindata->amount)
		maindata->philos[i++].last_eat = maindata->starttime;
	maindata->start = 1;
	usleep(100);
	i = 0;
	// while (!maindata->someonedied)
	// {
	// 	check_dead(&maindata->philos[i++]);
	// 	if (i == maindata->amount)
	// 		i = 0;
	// 	usleep(50);
	// }
	while (1)
	{
		
	}
	return (0);
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
