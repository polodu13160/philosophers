/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 07:23:20 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/28 22:14:03 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "stdio.h"
#include <unistd.h>

static bool	philo_check_dead(t_philo_info *list, long int i,
				long int time_start);

void	monitoring(t_philo_info *list, long int time_start)
{
	long		count_nb_finish;
	long int	i;

	count_nb_finish = 0;
	i = 0;
	while (count_nb_finish != list->number_of_philosophers)
	{
		if (i < 0)
			break ;
		i = 0;
		count_nb_finish = 0;
		while (i < list->number_of_philosophers)
		{
			pthread_mutex_lock(&list->lock_print_action);
			if (list->philosophers[i].finish == 1)
				count_nb_finish++;
			if (philo_check_dead(list, i, time_start) > 0)
			{
				i = -1000;
				break ;
			}
			pthread_mutex_unlock(&list->lock_print_action);
			i++;
		}
	}
}

static bool	error_time_listed_monitoring(t_philo_info *list, long int i,
		long int var_time_now)
{
	if (var_time_now == -1)
		print_error_time("Error Time");
	i = 0;
	while (i < list->number_of_philosophers)
	{
		pthread_mutex_lock(list->philosophers[i].action->lock_action);
		list->philosophers[i].action->action_type = STOP;
		list->philosophers[i].error_time = 1;
		pthread_mutex_unlock(list->philosophers[i].action->lock_action);
		i++;
	}
	return (1);
}

static void	philo_is_dead(t_philo_info *list, long int i, long int time_start)
{
	printf("%lu %ld died\n", calc_time(time_start), list->philosophers[i].id);
	i = 0;
	while (i < list->number_of_philosophers)
	{
		pthread_mutex_lock(list->philosophers[i].action->lock_action);
		list->philosophers[i].action->action_type = STOP;
		pthread_mutex_unlock(list->philosophers[i].action->lock_action);
		i++;
	}
}

static bool	philo_check_dead(t_philo_info *list, long int i,
		long int time_start)
{
	long int	v_time_now;

	v_time_now = time_now();
	if (v_time_now == -1 || list->philosophers[i].error_time == 1)
		return (error_time_listed_monitoring(list, i, v_time_now));
	if (list->philosophers[i].finish != 1 && (time_now()
			- list->philosophers[i].last_time_to_eat) >= list->time_to_die)
	{
		pthread_mutex_unlock(&list->lock_print_action);
		usleep(500);
		pthread_mutex_lock(&list->lock_print_action);
		v_time_now = time_now();
		if (v_time_now == -1)
			return (error_time_listed_monitoring(list, i, v_time_now));
		if (list->philosophers[i].finish != 1
			&& list->philosophers[i].last_time_to_eat != -2 && (time_now()
				- list->philosophers[i].last_time_to_eat) >= list->time_to_die)
		{
			philo_is_dead(list, i, time_start);
			pthread_mutex_unlock(&list->lock_print_action);
			return (1);
		}
	}
	return (0);
}
