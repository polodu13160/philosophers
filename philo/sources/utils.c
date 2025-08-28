/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 07:58:38 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/28 10:56:56 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include <limits.h>
#include <unistd.h>

static int	usleep_message_error(t_philo_attributes *philo)
{
	pthread_mutex_lock(philo->action->lock_action);
	philo->action->action_type = STOP;
	pthread_mutex_unlock(philo->action->lock_action);
	pthread_mutex_lock(philo->lock_print_action);
	print_error_time("Time Error");
	philo->error_time = 1;
	pthread_mutex_unlock(philo->lock_print_action);
	return (2);
}

int	usleep_cut(t_philo_attributes *philo, long int time_limit)
{
	long int	now;
	long int	time_now_first_function;

	time_now_first_function = time_now();
	if (time_now_first_function == -1)
		return (usleep_message_error(philo));
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
			return (usleep_message_error(philo));
		if ((time_now_first_function + time_limit) < now)
			return (0);
	}
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
	if (neg % 2 == 0 || number == 0)
		return (number);
	else
		return (-1);
}
