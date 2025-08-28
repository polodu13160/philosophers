/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 04:16:01 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/28 05:04:27 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	*exit_thread(t_philosopher_attributes *philo);
static void	think(t_philosopher_attributes *philo, long int time_mili_start);
static void	sleep(t_philosopher_attributes *philo, long int time_mili_start,
				int *a);

void	*while_action_philo(long int time_mili_start,
		t_philosopher_attributes *philo)
{
	int	a;

	a = 0;
	if (philo->id % 2 == 0)
		usleep_cut(philo, philo->time_to_eat);
	while (1)
	{
		pthread_mutex_lock(philo->action->lock_action);
		if (philo->action->action_type == STOP)
			return (exit_thread(philo));
		if (a == 0)
			think(philo, time_mili_start);
		if (a == 1)
		{
			take_forks(philo, time_mili_start);
			if (philo->action->action_type != STOP)
				philo->action->action_type = EAT;
			ft_new_message(philo, time_mili_start);
		}
		if (a == 2)
			sleep(philo, time_mili_start, &a);
		pthread_mutex_unlock(philo->action->lock_action);
		a++;
	}
}

static void	*exit_thread(t_philosopher_attributes *philo)
{
	if (philo->have_forks == 1)
	{
		pthread_mutex_unlock(philo->attr_left_fork->lock_fork);
		pthread_mutex_unlock(philo->attr_right_fork->lock_fork);
	}
	pthread_mutex_unlock(philo->action->lock_action);
	pthread_mutex_lock(philo->lock_print_action);
	philo->finish = 1;
	pthread_mutex_unlock(philo->lock_print_action);
	return (NULL);
}

static void	think(t_philosopher_attributes *philo, long int time_mili_start)
{
	if (philo->action->action_type != STOP)
		philo->action->action_type = THINK;
	ft_new_message(philo, time_mili_start);
	pthread_mutex_unlock(philo->action->lock_action);
	if (philo->number_of_philos % 2 != 0)
		usleep_cut(philo, philo->time_to_eat);
	pthread_mutex_lock(philo->action->lock_action);
}

static void	sleep(t_philosopher_attributes *philo, long int time_mili_start,
		int *a)
{
	if (philo->action->action_type != STOP)
		philo->action->action_type = SLEEP;
	ft_new_message(philo, time_mili_start);
	*a = -1;
}
