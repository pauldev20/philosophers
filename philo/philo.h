/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgeeser <pgeeser@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 14:57:25 by pgeeser           #+#    #+#             */
/*   Updated: 2022/09/13 13:42:09 by pgeeser          ###   ########.fr       */
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
	pthread_t		death_thread;
	long			last_eat;
	int				left_fork;
	int				right_fork;
	pthread_mutex_t	eating;
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
	struct s_philo		*philos;
	pthread_mutex_t		*forks;
	int					someonedied;
	int					philos_finished;
}	t_main;

void	ft_putstr_fd(char *s, int fd);
int		errorexit(char *str, t_main *maindata);
long	gettimems(void);
int		ft_atoi(const char *str);
void	timesleep(long milliseconds);
void	write_thread_msg(char *str, t_philo *philo);
void	cleanup(t_main *maindata);
size_t	ft_strlen(const char *str);

int		check_death(t_philo *philo, int i);

#endif