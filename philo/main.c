/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 14:52:16 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/21 14:59:44 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_philo(t_main *maindata, int nb)
{
	maindata->philos[nb].nb_ate = 0;
	maindata->philos[nb].id = nb + 1;
	maindata->philos[nb].finished = 0;
	maindata->philos[nb].maindata = maindata;
	if (nb == 0)
		maindata->philos[nb].left_fork = 0;
	else
		maindata->philos[nb].left_fork = nb;
	if (nb + 1 == maindata->amount)
		maindata->philos[nb].right_fork = 0;
	else
		maindata->philos[nb].right_fork = nb + 1;
	if (pthread_mutex_init(&maindata->forks[nb], NULL))
		return (errorexit("failed initializing mutex", maindata));
	return (0);
}

static int	init_main(t_main *maindata)
{
	maindata->someonedied = 0;
	maindata->philos_finished = 0;
	if (pthread_mutex_init(&maindata->write_mutex, NULL))
		return (errorexit("failed initializing mutex", maindata));
	if (pthread_mutex_init(&maindata->deadcheck_mutex, NULL))
		return (errorexit("failed initializing mutex", maindata));
	if (pthread_mutex_init(&maindata->finishcheck_mutex, NULL))
		return (errorexit("failed initializing mutex", maindata));
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
		maindata->must_eat_count = -1;
	if (maindata->amount < 1 || maindata->time_to_die < 0
		|| maindata->time_to_eat < 0 || maindata->time_to_sleep < 0)
		return (errorexit("invalid arguments", maindata));
	if (init_main(maindata))
		return (1);
	i = 0;
	while (i < maindata->amount)
		if (init_philo(maindata, i++))
			return (1);
	return (0);
}

int	start_threads(t_main *maindata)
{
	int	i;

	i = 0;
	maindata->starttime = timenow();
	while (i < maindata->amount)
		maindata->philos[i++].death_time = maindata->starttime
			+ maindata->time_to_die;
	i = 0;
	while (i++ < maindata->amount)
	{
		if (pthread_create(&maindata->philos[i - 1].thread, NULL, philo_routine,
				&maindata->philos[i - 1]) != 0)
			return (errorexit("failed creating thread", maindata));
	}
	check_dead(maindata);
	return (0);
}

int	main(int argc, char **argv)
{
	t_main	maindata;

	if (argc < 5 || argc > 6)
		return (errorexit("invalid arguments", NULL));
	if (init(&maindata, argc, argv))
		return (1);
	if (start_threads(&maindata))
		return (1);
	cleanup(&maindata);
	return (0);
}
