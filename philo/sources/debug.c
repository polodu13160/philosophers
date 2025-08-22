/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 04:32:52 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/21 23:09:41 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "philosopher.h"
#include <stdio.h>

void	printf_forks(t_fork *forks)
{
	int	i;

	i = 0;
	while (forks[i].fork != -1)
	{
		dprintf(2, "fork value = %d \n", forks[i].fork);
		i++;
	}
}

int	printf_philo(t_pnj *philo)
{
	int	i;

	i = 0;
	while (philo[i].philo_number != -1)
	{
		dprintf(2, "\nphilo number = %d\n", philo[i].philo_number);
		dprintf(2, "philo left = %d\n", philo[i].attr_left_fork->fork);
		if (philo[i].attr_right_fork != NULL)
			dprintf(2, "philo right = %d", philo[i].attr_right_fork->fork);
		// if (i > 0 && philo[i].attr_right_fork == philo[i -1].attr_left_fork)
		// {
		//     dprintf(2,"\nyes\n");
		// }
		i++;
	}
	return (0);
}

void	print_time(long int value)
{
	dprintf(2, "time = %ld\n", value);
}
