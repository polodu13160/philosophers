/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_destroy_mutex.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 04:24:49 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/28 05:51:12 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	free_philo(t_philosopher_attributes *philo)
{
	free(philo);
}

void *free_destroy_tab_action(t_action_mutex *action, pthread_mutex_t	*malloc_action_mutex,int index_mutex_crash, int nb_philo)
{
	int i;

	i = 0;
    if (index_mutex_crash >= 0)
    {
		while (nb_philo > i)
		{
			if (index_mutex_crash > i)
				pthread_mutex_destroy(action[i].lock_action);
			if (i + 1 == nb_philo)
				free(malloc_action_mutex);
			i++;
		}
    }
    else if (malloc_action_mutex != NULL)
        free(malloc_action_mutex);
    if (action != NULL)
			free(action);
    return NULL;
}

void	*free_destoy_tab_forks(t_fork *forks, pthread_mutex_t	*lock_mutex, int index_mutex_crash)
{
	int i;

	i = 0;
	if (index_mutex_crash >= 0)
	{
		while (forks[i].fork != -1)
		{
			if (index_mutex_crash > i)
				pthread_mutex_destroy(forks[i].lock_fork);
			if (forks[i + 1].fork == -1)
				free(forks[i].table_mutex);
			i++;
		}
	}
	else if (lock_mutex != NULL)
		free(lock_mutex);
	if (forks != NULL)
		free(forks);
	return NULL;
}
