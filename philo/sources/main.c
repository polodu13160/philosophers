/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:39:39 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/28 05:38:18 by pde-petr         ###   ########.fr       */
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


int	check_dead_or_stop(t_philosopher_attributes *philo)
{
	int	return_value;

	return_value = 0;
	if (philo->action->action_type == STOP
		|| philo->action->action_type == HUNGER_STRIKE)
		return_value = 1;
	return (return_value);
}

int	usleep_cut(t_philosopher_attributes *philo, long int time_limit)
{
	long int	now;
	long int	time_now_first_function;

	time_now_first_function = time_now();
	if (time_now_first_function == -1)
		return (2);
	while (1)
	{
		usleep(100);
		pthread_mutex_lock(philo->action->lock_action);
		if (philo->action->action_type == STOP)
		{
			pthread_mutex_unlock(philo->action->lock_action);
			return (1);
		}
		pthread_mutex_unlock(philo->action->lock_action);
		now = time_now();
		if (now == -1)
			return (2);
		if ((time_now_first_function + time_limit) < now)
			return (0);
	}
}

void	*to_be_philosopher(void *philo)
{
	pthread_mutex_lock(((t_philosopher_attributes *)philo)->lock_print_action);
	((t_philosopher_attributes *)philo)->last_time_to_eat = *((t_philosopher_attributes *)philo)->time_start;
	if (((t_philosopher_attributes *)philo)->finish == 1)
	{
		pthread_mutex_unlock(((t_philosopher_attributes *)philo)->lock_print_action);
		return (NULL);
	}
	pthread_mutex_unlock(((t_philosopher_attributes *)philo)->lock_print_action);
	return (while_action_philo(*((t_philosopher_attributes *)philo)->time_start,
			philo));
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


void init_list_info(t_philosopher_info *list)
{
	list->number_of_philosophers = 0;
	list->philosophers = NULL;
	list->number_of_times_each_philosopher_must_eat = -1;
	list->time_to_die = 0;
	list->time_to_eat = 0;
	list->time_to_sleep = 0;
}

int check_parsing(char **argv, t_philosopher_info *list, int argc)
{
	if (parsing_argv(++argv, list, argc - 1) > 0)
	{
		printf("error parsing for overflow or negatif value or too much parameters\n");
		return (1);
	}
	if (list->number_of_philosophers == 0)
	{
		printf("are you serious ?\n");
		return (1);
	}
}

int	main(int argc, char **argv)
{
	t_philosopher_info	list;
	pthread_mutex_t		mutex_print;
	int					i;
	int					temp;
	long int			time_start;
	int					count_nb_finish;
	
	init_list_info(&list);
	if (check_parsing(argv, &list, argc) == 1)
		return 1;
	list.philosophers = malloc(sizeof(t_philosopher_attributes)
			* (list.number_of_philosophers + 1));
	list.forks = init_fork(list.number_of_philosophers);
	list.action_mutex = init_action_mutex(list.number_of_philosophers);
	
	
	i = 0;
	
	
	
	
	
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
		list.philosophers[i].id = i + 1;
		list.philosophers[i].number_of_philos = list.number_of_philosophers;
		list.philosophers[i].time_to_sleep = list.time_to_sleep;
		list.philosophers[i].time_to_eat = list.time_to_eat;
		list.philosophers[i].rest_number_eat = list.number_of_times_each_philosopher_must_eat;
		list.philosophers[i].lock_print_action = &mutex_print;
		list.philosophers[i].action = &list.action_mutex[i];
		list.philosophers[i].have_forks = 0;
		list.philosophers[i].finish = 0;
		list.philosophers[i].time_start = &time_start;
		list.philosophers[i].last_time_to_eat = -2;
		i++;
	}
	list.philosophers[i].id = -1;
	attr_forks_philo(list.philosophers, list.forks,
		list.number_of_philosophers);
	// printf_philo(list.philosophers);
	i = 0;
	pthread_mutex_lock(&mutex_print);
	while (list.philosophers[i].id != -1)
	{
		if (pthread_create(&list.philosophers[i].value_thread, NULL,
				to_be_philosopher, &list.philosophers[i]) == -1)
		{
			temp = i;
			while (--i >= 0)
				list.philosophers[i].finish = 1;
			pthread_mutex_unlock(&mutex_print);
			while (i < temp)
				pthread_join(list.philosophers[i++].value_thread, NULL);
			free_philo(list.philosophers);
			free(list.action_mutex->lock_action);
			free(list.action_mutex);
			free_destoy_tab_forks(list.forks, NULL, -1);
			pthread_mutex_destroy(&mutex_print);
			return (1);
		}
		i++;
	}
	time_start = time_now();
	pthread_mutex_unlock(&mutex_print);
	usleep(1000);
	// return (0);
	i = 0;
	count_nb_finish = 0;
	while (count_nb_finish != list.number_of_philosophers)
	{
		if (i < 0)
			break ;
		i = 0;
		count_nb_finish = 0;
		while (i < list.number_of_philosophers)
		{
			pthread_mutex_lock(&mutex_print);
			if (list.philosophers[i].finish == 1)
				count_nb_finish++;
			if (list.philosophers[i].finish != 1 && (time_now()
					- list.philosophers[i].last_time_to_eat) > list.time_to_die)
			{
				pthread_mutex_unlock(&mutex_print);
				usleep(1000);
				pthread_mutex_lock(&mutex_print);
				if (list.philosophers[i].finish != 1
					&& list.philosophers[i].last_time_to_eat != -2
					&& (time_now()
						- list.philosophers[i].last_time_to_eat) > list.time_to_die)
				{
					printf("%lu %d died\n", calc_time(time_start),
						list.philosophers[i].id);
					i = 0;
					while (i < list.number_of_philosophers)
					{
						pthread_mutex_lock(list.philosophers[i].action->lock_action);
						list.philosophers[i].action->action_type = STOP;
						pthread_mutex_unlock(list.philosophers[i].action->lock_action);
						i++;
					}
					i = -1000;
					pthread_mutex_unlock(&mutex_print);
					break ;
				}
			}
			pthread_mutex_unlock(&mutex_print);
			i++;
		}
		//
	}
	i = 0;
	while (list.philosophers[i].id != -1)
	{
		pthread_join(list.philosophers[i].value_thread, NULL);
		i++;
	}
	free_philo(list.philosophers);
	free(list.action_mutex->lock_action);
	free(list.action_mutex);
	free_destoy_tab_forks(list.forks, NULL, -1);
	pthread_mutex_destroy(&mutex_print);
	return (0);
}
