/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 17:31:17 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/21 16:17:18 by pgeeser          ###   ########.fr       */
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
	if (!maindata->philos)
		return ;
	i = 0;
	while (i < maindata->amount)
		pthread_join(maindata->philos[i++].thread, NULL);
	pthread_mutex_destroy(&maindata->write_mutex);
	pthread_mutex_destroy(&maindata->deadcheck_mutex);
	pthread_mutex_destroy(&maindata->finishcheck_mutex);
	if (maindata->philos)
		free(maindata->philos);
	if (!maindata->forks)
		return ;
	while (i++ < maindata->amount)
	{
		if (maindata->forks)
			pthread_mutex_destroy(&maindata->forks[i - 1]);
	}
	free(maindata->forks);
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

void	timesleep(long milliseconds, t_philo *philo)
{
	long	start;

	start = timenow();
	while ((timenow() - start) < milliseconds
		&& !check_death(philo->maindata, 0))
		usleep(200);
}

void	write_thread_msg(char *str, t_philo *philo)
{
	pthread_mutex_lock(&(philo->maindata->write_mutex));
	if (!check_death(philo->maindata, 0))
		printf("%ld %d %s\n", timenow()
			- philo->maindata->starttime, philo->id, str);
	pthread_mutex_unlock(&(philo->maindata->write_mutex));
}
