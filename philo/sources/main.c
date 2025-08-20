/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:39:39 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/21 01:44:33 by pde-petr         ###   ########.fr       */
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

void	ft_message(int action, time_t time, int philo_number)
{
	if (action == SLEEP)
		printf("%lu %d is sleeping\n", time, philo_number);
	if (action == EAT)
		printf("%lu %d is eating\n", time, philo_number);
	if (action == FORK)
		printf("%lu %d has taken a fork\n", time, philo_number);
	if (action == THINK)
		printf("%lu %d is thinking\n", time, philo_number);
}

void	*print_error_return_null(char *message)
{
	perror(message);
	return (NULL);
}

long int	calc_time(long time_mili_start)
{
	long int		time_mili;
	struct timeval	time_now;

	if (gettimeofday(&time_now, NULL) <= -1)
	{
		return (-1);
	}
	time_mili = (time_now.tv_sec * 1000 + time_now.tv_usec / 1000)
		- time_mili_start;
	return (time_mili);
}

void	*while_action_philo(long int time_mili_start, t_pnj *philo)
{
	long int	time_now_in_mili;
	long int	time_eat;
	long int	time_sleep;
	int			a;

	a = 0;
	time_eat = philo->rest_time_to_eat;
	time_sleep = philo->rest_time_to_sleep;
	while (1)
	{
		time_now_in_mili = calc_time(time_mili_start);
		if (time_now_in_mili <= -1)
			return (print_error_return_null("error Time in thread"));
		if (a == 0 && ++a)
			ft_message(THINK, time_now_in_mili, philo->philo_number);
		else if (a == 1 && ++a)
		{
			ft_message(EAT, time_now_in_mili, philo->philo_number);
			philo->rest_time_to_eat = time_eat;
			philo->rest_number_eat--;
			if (philo->rest_number_eat == 0)
				return (NULL);
		}
		else if (a == 2 && ++a)
		{
			ft_message(SLEEP, time_now_in_mili, philo->philo_number);
			a = 0;
		}
	}
}

void	*to_be_philosopher(void *philo)
{
	long int	time_mili_start;

	if (gettimeofday(&((t_pnj *)philo)->time_start, NULL) == -1)
	{
		return (print_error_return_null("error of the time"));
	}
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

int	main(int argc, char **argv)
{
	t_philo list;
	list.number_of_philosophers = 0;
	list.philosophers = NULL;
	list.number_of_times_each_philosopher_must_eat = -1;
	list.time_to_die = 0;
	list.time_to_eat = 0;
	list.time_to_sleep = 0;
	int i;
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
	list.forks = malloc(sizeof(int) * (list.number_of_philosophers
				+ 1));
	if (list.philosophers == NULL || list.forks)
		return (1);
	while (i < list.number_of_philosophers)
	{
		list.philosophers[i].philo_number = i + 1;
		list.philosophers[i].rest_time_to_die = list.time_to_die;
		list.philosophers[i].rest_time_to_eat = list.time_to_eat;
		list.philosophers[i].rest_time_to_sleep = list.time_to_sleep;
		list.philosophers[i].rest_number_eat = list.number_of_times_each_philosopher_must_eat;
		attr_forcetta(); //a faire
		pthread_create(&list.philosophers[i].value_thread, NULL,
			to_be_philosopher, &list.philosophers[i]);
		i++;
	}
	list.philosophers[i].philo_number = -1;
	// if (DEBUG == 1)
	// 	printf_philo(list.philosophers);
	i = 0;
	while (i < list.number_of_philosophers)
	{
		if (pthread_join(list.philosophers[i].value_thread, NULL))
		{
			if (list.philosophers[i].action != EAT)
			{
				free_philo(list.philosophers);
				return (0);
			}
		}
		i++;
	}
	free_philo(list.philosophers);

	return (0);
}