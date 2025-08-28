/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 04:18:19 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/28 10:56:26 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "stdio.h"

static int	parsing_argv(char **argv, t_philo_info *list, int argc);

int	check_parsing(char **argv, t_philo_info *list, int argc)
{
	if (parsing_argv(++argv, list, argc - 1) > 0)
	{
		printf(\
"error parsing for overflow or negatif value or too much parameters\n");
		return (1);
	}
	if (list->number_of_philosophers == 0)
	{
		printf("are you serious ?\n");
		return (1);
	}
	return (0);
}

static int	parsing_argv(char **argv, t_philo_info *list, int argc)
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
