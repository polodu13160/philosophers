/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauldepetrini <pauldepetrini@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:39:39 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/22 17:30:51 by pauldepetri      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "philosopher.h"
#include "stdio.h"
#include "stdlib.h"
#include <errno.h>
#include <limits.h>
#include <sys/time.h>
#include <unistd.h>

long int time_now(void)
{
	struct timeval	time_now;

	if (gettimeofday(&time_now, NULL) <= -1)
		return (-1);
	return (time_now.tv_sec * 1000 + time_now.tv_usec / 1000);
}

long int	calc_time(long time_mili_start)
{
	long int		time_mili;
	long int		v_time_now;

	v_time_now = time_now();
	if (v_time_now == -1)
		return (-1);
	time_mili = v_time_now
		- time_mili_start;
	return (time_mili);
}

void	*print_error_return_null(char *message)
{
	perror(message);
	return (NULL);
}



int	ft_message(t_pnj *philo, long int time_mili_start)
{
	long int	time_now_in_mili_at_start;

	pthread_mutex_lock(philo->lock_print_action);
	// verifier aussi ici si le temps est bon;
	time_now_in_mili_at_start = calc_time(time_mili_start);
	if (time_now_in_mili_at_start <= -1)
	{
		print_error_return_null("error Time in thread");
		return 2;
	}
		
	if (philo->action->action == SLEEP)
		printf("%lu %d is sleeping\n", time_now_in_mili_at_start, philo->philo_number);
	if (philo->action->action == EAT)
	{
		printf("%lu %d is eating\n", time_now_in_mili_at_start, philo->philo_number);
		philo->last_time_to_eat = time_now();
		philo->rest_number_eat--;
		if (philo->rest_number_eat == 0 || philo->last_time_to_eat == -1)
		{
			pthread_mutex_unlock(philo->lock_print_action);
			if  (philo->last_time_to_eat == -1)
			{
				print_error_return_null("error Time in thread");
				return 2;
			}
			return 1;
		}
			
		philo->last_time_to_eat = time_now();
		if (philo->last_time_to_eat == -1)
		{
			print_error_return_null("error Time in thread");
			return 2;
		}
			
	}
	if (philo->action->action == FORK)
		printf("%lu %d has taken a fork\n", time_now_in_mili_at_start,
			philo->philo_number);
	if (philo->action->action == THINK)
		printf("%lu %d is thinking\n", time_now_in_mili_at_start, philo->philo_number);
	pthread_mutex_unlock(philo->lock_print_action);
	return (0);
}

void	*while_action_philo(long int time_mili_start, t_pnj *philo)
{
	// long int	time_max_sleep;
	// long int	time_max_to_die;
	int			a;

	a = 0;
	// time_max_to_die = philo->rest_time_to_die;
	// time_max_sleep = philo->time_to_sleep;
	philo->last_time_to_eat = time_mili_start;
	while (1)
	{
		pthread_mutex_lock(philo->action->lock_action);
		if (a == 0)
			philo->action->action = THINK;
		if (a == 1)
		{
			philo->action->action = EAT;
		}
		if (a == 2)
		{
			philo->action->action = SLEEP;
			a = -1;
		}
		if (ft_message(philo, time_mili_start) > 0)
		{
			pthread_mutex_unlock(philo->action->lock_action);
			return NULL;
		}
		if (philo->action->action == SLEEP)
		{
			pthread_mutex_unlock(philo->action->lock_action);
			usleep(philo->time_to_sleep * 1000);
		}
		else	
			pthread_mutex_unlock(philo->action->lock_action);
		a++;
		
	}
}

void	*to_be_philosopher(void *philo)
{
	long int	time_mili_start;

	time_mili_start = (((t_pnj *)philo)->time_start.tv_sec * 1000)
		+ (((t_pnj *)philo)->time_start.tv_usec / 1000);
	return (while_action_philo(time_mili_start, philo));
}
long int	ft_atol_no_neg(char *argv)
{
	int		neg;
	size_t	number;

	neg = 0;
	number = 0;
	while (*argv == '+' || *argv == '-')
		if (*(argv++) == '-')
			neg++;
	while (*argv >= '0' && *argv <= '9' && (number * 10
			+ *(argv)-48) <= LONG_MAX)
		number = number * 10 + *(argv++) - 48;
	if (*argv != 0)
		return (-1);
	if (neg % 2 == 0)
		return (number);
	else
		return (-1);
}

int	parsing_argv(char **argv, t_philo *list, int argc)
{
	int			i;
	long int	value;

	i = -1;
	while (argc-- != 0)
	{
		value = ft_atol_no_neg(*(argv++));
		if (value == -1)
			return (1);
		if (++i == 0)
			list->number_of_philosophers = value;
		else if (i == 1)
			list->time_to_die = value;
		else if (i == 2)
			list->time_to_eat = value;
		else if (i == 3)
			list->time_to_sleep = value;
		else if (i == 4)
			list->number_of_times_each_philosopher_must_eat = value;
	}
	if (i < 3 || i > 4)
		return (1);
	return (0);
}

void	free_philo(t_pnj *philo)
{
	free(philo);
}

void	mutex_destroy_tab_forks_and_free(t_fork *forks)
{
	int	i;

	i = 0;
	while (forks[i].fork != -1)
	{
		pthread_mutex_destroy(forks[i].lock_fork);
		if (forks[i + 1].fork == -1)
			free(forks[i].table_mutex);
		i++;
	}
	free(forks);
}

t_fork	*init_fork(int number_philo)
{
	int				i;
	t_fork			*forks;
	pthread_mutex_t	*lock_mutex;

	forks = malloc(sizeof(t_fork) * (number_philo + 1));
	lock_mutex = malloc(sizeof(pthread_mutex_t) * (number_philo));
	if (forks == NULL || lock_mutex == NULL)
	{
		if (forks != NULL)
			free(forks);
		if (lock_mutex != NULL)
			free(lock_mutex);
		return (NULL);
	}
	i = 0;
	while (i < number_philo)
	{
		forks[i].fork = -1;
		if (pthread_mutex_init(&lock_mutex[i], NULL) == -1)
		{
			mutex_destroy_tab_forks_and_free(forks);
			return (NULL);
		}
		forks[i].lock_fork = &lock_mutex[i];
		forks[i].fork = i;
		forks[i].table_mutex = lock_mutex;
		i++;
	}
	forks[i].fork = -1;
	return (forks);
}

void	attr_forks_philo(t_pnj *philosophers, t_fork *forks,
		int number_of_philosophers)
{
	int	i;

	i = 0;
	while (forks[i].fork != -1)
	{
		philosophers[i].attr_left_fork = &forks[i];
		if (i == 0)
		{
			if (i != number_of_philosophers - 1)
				philosophers[i].attr_right_fork = &forks[number_of_philosophers
					- 1];
			else
			{
				philosophers[i].attr_right_fork = NULL;
				return ;
			}
		}
		else
		{
			if (i != number_of_philosophers)
				philosophers[i].attr_right_fork = philosophers[i
					- 1].attr_left_fork;
		}
		i++;
	}
}

t_action_mutex  *init_action_mutex(long int nb_philo)
{
	t_action_mutex *malloc_action;
	pthread_mutex_t *malloc_action_mutex;
	malloc_action = malloc(sizeof(t_action_mutex) * nb_philo);
	malloc_action_mutex = malloc(sizeof(pthread_mutex_t) * nb_philo);
	
	if (malloc_action == NULL || malloc_action_mutex == NULL)
	{
		if (malloc_action != NULL)
			free(malloc_action);
		if (malloc_action_mutex != NULL)
			free(malloc_action_mutex);
		return (NULL);
	}
	int i;
	i = 0;
	while (i < nb_philo)
	{
		pthread_mutex_init(&malloc_action_mutex[i],NULL);
		malloc_action[i].lock_action = &malloc_action_mutex[i];
		malloc_action[i].action = INIT;
		i++;
	}
	return malloc_action;
}

int	main(int argc, char **argv)
{
	t_philo list;
	pthread_mutex_t mutex_print;
	int i;
	struct timeval time_start;

	list.number_of_philosophers = 0;
	list.philosophers = NULL;
	list.number_of_times_each_philosopher_must_eat = -1;
	list.time_to_die = 0;
	list.time_to_eat = 0;
	list.time_to_sleep = 0;
	i = 0;
	if (parsing_argv(++argv, &list, argc - 1) > 0)
	{
		printf("error parsing for overflow or negatif value or too much parameters\n");
		return (1);
	}
	if (list.number_of_philosophers == 0)
	{
		printf("are you serious ?\n");
		return (1);
	}
	list.philosophers = malloc(sizeof(t_pnj) * (list.number_of_philosophers
				+ 1));
	list.forks = init_fork(list.number_of_philosophers);
	list.action_mutex = init_action_mutex(list.number_of_philosophers);
	
	if (list.philosophers == NULL || list.forks == NULL)
	{
		if (list.philosophers != NULL)
			free(list.philosophers);
		if (list.forks == NULL)
			free(list.forks);
		return (1);
	}
	pthread_mutex_init(&mutex_print, NULL);
	
	while (i < list.number_of_philosophers)
	{
		list.philosophers[i].philo_number = i + 1;
		list.philosophers[i].rest_time_to_die = list.time_to_die;
		list.philosophers[i].time_to_sleep = list.time_to_sleep;
		list.philosophers[i].rest_number_eat = list.number_of_times_each_philosopher_must_eat;
		list.philosophers[i].lock_print_action = &mutex_print;
		list.philosophers[i].action = &list.action_mutex[i];
		i++;
	}
	list.philosophers[i].philo_number = -1;
	
	attr_forks_philo(list.philosophers, list.forks,
		list.number_of_philosophers);
	// printf_philo(list.philosophers);

	if (gettimeofday(&time_start, NULL) <= -1)
	{
		print_error_return_null("error of the time\n");
		free(list.philosophers);
		mutex_destroy_tab_forks_and_free(list.forks);
		pthread_mutex_destroy(&mutex_print);
		return (1);
	}
	i = 0;
	while (list.philosophers[i].philo_number != -1)
	{
		list.philosophers[i].time_start = time_start;
		pthread_create(&list.philosophers[i].value_thread, NULL,
			to_be_philosopher, &list.philosophers[i]);
		i++;
	}
	// if (DEBUG == 1)
	// 	printf_philo(list.philosophers);
	i = 0;
	while (list.philosophers[i].philo_number != -1)
	{
		
		// if (list.philosophers[i].last_time_to_eat )
		
	






	
		if (pthread_join(list.philosophers[i].value_thread, NULL))
		{
			if (list.philosophers[i].action->action != EAT)
			{
				free_philo(list.philosophers);
				mutex_destroy_tab_forks_and_free(list.forks);
				free(list.forks);
				pthread_mutex_destroy(&mutex_print);

				return (0);
			}
		}
		i++;
	}
	free_philo(list.philosophers);
	free(list.action_mutex->lock_action);
	
	free(list.action_mutex);
	mutex_destroy_tab_forks_and_free(list.forks);
	pthread_mutex_destroy(&mutex_print);

	return (0);
}