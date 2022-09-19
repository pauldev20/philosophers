/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 17:31:17 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/20 01:43:05 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	errorexit(char *str, t_main *maindata)
{
	write(2, "Error: ", 7);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	cleanup(maindata);
	return (1);
}

void	cleanup(t_main *maindata)
{
	int	i;

	i = 0;
	if (!maindata)
		return ;
	pthread_mutex_unlock(&maindata->write_mutex);
	pthread_mutex_destroy(&maindata->write_mutex);
	pthread_mutex_unlock(&maindata->deadcheck_mutex);
	pthread_mutex_destroy(&maindata->deadcheck_mutex);
	pthread_mutex_unlock(&maindata->finishcheck_mutex);
	pthread_mutex_destroy(&maindata->finishcheck_mutex);
	while (i++ < maindata->amount)
	{
		if (maindata->forks)
		{
			pthread_mutex_unlock(&maindata->forks[i - 1]);
			pthread_mutex_destroy(&maindata->forks[i - 1]);
		}
		if (maindata->philos) 
		{
			pthread_mutex_unlock(&maindata->philos[i - 1].eating);
			pthread_mutex_destroy(&maindata->philos[i - 1].eating);
		}
	}
	if (maindata->philos)
		free(maindata->philos);
	if (maindata->forks)
		free(maindata->forks);
}

void	timesleep(long milliseconds)
{
	long	start;

	start = timenow();
	while ((timenow() - start) < milliseconds)
		usleep(milliseconds / 10);
}

void	write_thread_msg(char *str, t_philo *philo)
{
	long	time;

	pthread_mutex_lock(&(philo->maindata->write_mutex));
	time = timenow() - philo->maindata->starttime;
	if (!(time >= 0 && time <= 2147483647 && !check_death(philo->maindata, 0)))
		return ;
	printf("%ld %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&(philo->maindata->write_mutex));
}

long	timenow(void)
{
	struct timeval	time;
	long			milliseconds;

	gettimeofday(&time, NULL);
	milliseconds = time.tv_sec * 1000;
	milliseconds += time.tv_usec / 1000;
	return (milliseconds);
}
