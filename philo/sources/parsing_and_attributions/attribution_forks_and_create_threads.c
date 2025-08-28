/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   attribution_forks_and_create_threads.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 08:08:14 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/28 10:47:20 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	*to_be_philosopher(void *philo);

int	pthread_create_philosophers(t_philo_info *list, long int *time_start)
{
	int	i;
	int	temp;

	pthread_mutex_lock(&list->lock_print_action);
	i = 0;
	while (list->philosophers[i].id != -1)
	{
		if (pthread_create(&list->philosophers[i].value_thread, NULL,
				to_be_philosopher, &list->philosophers[i]) == -1)
		{
			temp = i;
			while (--i >= 0)
				list->philosophers[i].finish = 1;
			pthread_mutex_unlock(&list->lock_print_action);
			while (i < temp)
				pthread_join(list->philosophers[i++].value_thread, NULL);
			destroy_and_free_malloc(list);
			pthread_mutex_destroy(&list->lock_print_action);
			return (1);
		}
		i++;
	}
	*time_start = time_now();
	pthread_mutex_unlock(&list->lock_print_action);
	return (0);
}

static void	*to_be_philosopher(void *philo)
{
	t_philo_attributes	*philo_typed;

	philo_typed = philo;
	pthread_mutex_lock(philo_typed->lock_print_action);
	philo_typed->last_time_to_eat = *philo_typed->time_start;
	if (philo_typed->finish == 1)
	{
		pthread_mutex_unlock(philo_typed->lock_print_action);
		return (NULL);
	}
	pthread_mutex_unlock(philo_typed->lock_print_action);
	return (while_action_philo(*philo_typed->time_start, philo));
}

void	attr_forks_philo(t_philo_attributes *philosophers, t_fork *forks,
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
