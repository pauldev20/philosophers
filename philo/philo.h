/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 14:57:25 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/21 16:17:38 by pgeeser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>

typedef struct s_philo {
	struct s_main	*maindata;
	int				id;
	pthread_t		thread;
	long			death_time;
	int				left_fork;
	int				right_fork;
	int				nb_ate;
	int				finished;
}	t_philo;

typedef struct s_main {
	int					amount;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					must_eat_count;
	long				starttime;
	pthread_mutex_t		write_mutex;
	pthread_mutex_t		deadcheck_mutex;
	pthread_mutex_t		finishcheck_mutex;
	t_philo				*philos;
	pthread_mutex_t		*forks;
	int					someonedied;
	int					philos_finished;
}	t_main;

// checkers
int		check_death(t_main *maindata, int i);
void	check_must_eat(t_philo *philo);
int		check_args(int argc, char **argv);

// lib
int		errorexit(char *str, t_main *maindata);
void	cleanup(t_main *maindata);
void	timesleep(long milliseconds, t_philo *philo);
void	write_thread_msg(char *str, t_philo *philo);
long	timenow(void);

// philo
void	*philo_routine(void *data);
void	check_dead(t_main *main);

// utils
int		ft_atoi(const char *str);
size_t	ft_strlen(const char *str);

#endif