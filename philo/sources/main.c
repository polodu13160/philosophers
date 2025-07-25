/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:39:39 by pde-petr          #+#    #+#             */
/*   Updated: 2025/07/22 22:41:50 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "stdlib.h"
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

int	main(int argc, char **argv)
{
	t_philo list;
	list.number_of_philosophers = 0;
	list.number_of_times_eahc_philosopher_must_eat = -1;
	list.time_to_die = 0;
	list.time_to_eat = 0;
	list.time_to_sleep = 0;
	if (parsing_argv(++argv, &list, argc - 1) > 0)
	{
		printf("error parsing for overflow or negatif value or too much parameters\n");
		return (1);
	}

	return (0);
}