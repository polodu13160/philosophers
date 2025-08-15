/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pdepetrini <pdepetrini@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:39:39 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/15 18:55:08 by pdepetrini       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	error_erno(char *message)
{
	perror(message);
	return (errno);
}

long int	calc_time(int time_mili_start)
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

void	*to_be_philosopher(void *philo)
{
	long int	time_mili_start;
	int			time_now_in_mili;

	time_mili_start = (((t_philo *)philo)->time_start.tv_sec * 1000
			+ ((t_philo *)philo)->time_start.tv_usec / 1000);
	time_now_in_mili = calc_time(time_mili_start);
	if (time_now_in_mili <= -1)
		error_erno("error Time in thread");
	ft_message(FORK, time_now_in_mili,
		((t_philo *)philo)->philosophers->philo_number);
	return (NULL);
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
			list->number_of_times_eahc_philosopher_must_eat = value;
	}
	if (i < 3 || i > 4)
		return (1);
	return (0);
}

void	free_philo(t_pnj *philo)
{
	t_pnj	*temp;

	while (philo->next != NULL)
	{
		temp = philo->next;
		free(philo);
		philo = temp;
	}
	free(philo);
}

int	main(int argc, char **argv)
{
	t_philo list;
	list.number_of_philosophers = 0;
	list.philosophers = NULL;
	list.number_of_times_eahc_philosopher_must_eat = -1;
	list.time_to_die = 0;
	list.time_to_eat = 0;
	list.time_to_sleep = 0;
	if (gettimeofday(&list.time_start, NULL) == -1)
		return (error_erno("error of the time"));
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
	list.philosophers = malloc(sizeof(t_philo) * 1);
	if (list.philosophers == NULL)
		return (1);

	while (i < list.number_of_philosophers)
	{
		list.philosophers->next = NULL;
		if (i == 0)
			list.philo_uno = list.philosophers;
		list.philosophers->philo_number = i + 1;
		pthread_create(&list.philosophers->value_thread, NULL,
			to_be_philosopher, &list);
		if (i + 1 != list.number_of_philosophers)
		{
			list.philosophers->next = malloc(sizeof(t_philo) * 1);
			if (list.philosophers->next == NULL)
			{
				free_philo(list.philo_uno);
				return (1);
			}
			list.philosophers = list.philosophers->next;
		}
		i++;
	}
	pthread_join(list.philosophers->value_thread, NULL);
	free_philo(list.philo_uno);

	return (0);
}