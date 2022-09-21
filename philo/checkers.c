/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/13 14:02:48 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/21 12:21:01 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_main *maindata, int i)
{
	pthread_mutex_lock(&maindata->deadcheck_mutex);
	if (i)
		maindata->someonedied = i;
	i = maindata->someonedied;
	pthread_mutex_unlock(&maindata->deadcheck_mutex);
	return (i);
}

void	check_must_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->maindata->finishcheck_mutex);
	if (philo->nb_ate == philo->maindata->must_eat_count && !philo->finished)
	{
		philo->finished = 1;
		philo->maindata->philos_finished++;
		if (philo->maindata->philos_finished == philo->maindata->amount)
			check_death(philo->maindata, 2);
	}
	pthread_mutex_unlock(&philo->maindata->finishcheck_mutex);
}

int	check_args(int argc, char **argv)
{
	int		i;
	int		j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9' ||
				ft_strlen(argv[i]) > 10)
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
