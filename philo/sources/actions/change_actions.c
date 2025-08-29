/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_actions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 04:16:01 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/29 14:41:16 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	*exit_thread(t_philo_attributes *philo);

static void	think(t_philo_attributes *philo, long int time_mili_start,
				bool init);

static void	sleep(t_philo_attributes *philo, long int time_mili_start, int *a);

void	*while_action_philo(long int time_mili_start, t_philo_attributes *philo)
{
	int	a;

	a = 1;
	think(philo, time_mili_start, 1);
	if (philo->id % 2 == 0)
		usleep_cut(philo, philo->time_to_eat / 2);
	while (1)
	{
		pthread_mutex_lock(philo->action->lock_action);
		if (philo->action->action_type == STOP)
			return (exit_thread(philo));
		if (a == 0)
			think(philo, time_mili_start, 0);
		if (a == 1)
		{
			take_forks(philo, time_mili_start);
			if (philo->action->action_type != STOP)
				philo->action->action_type = EAT;
			lock_mutex_and_print_message(philo, time_mili_start);
		}
		if (a++ == 2)
			sleep(philo, time_mili_start, &a);
		pthread_mutex_unlock(philo->action->lock_action);
	}
}

static void	*exit_thread(t_philo_attributes *philo)
{
	pthread_mutex_unlock(philo->action->lock_action);
	pthread_mutex_lock(philo->lock_print_action);
	philo->finish = 1;
	pthread_mutex_unlock(philo->lock_print_action);
	return (NULL);
}

static void	think(t_philo_attributes *philo, long int time_mili_start,
		bool init)
{
	if (init == 1)
		pthread_mutex_lock(philo->action->lock_action);
	if (philo->action->action_type != STOP)
		philo->action->action_type = THINK;
	lock_mutex_and_print_message(philo, time_mili_start);
	pthread_mutex_unlock(philo->action->lock_action);
	if (philo->number_of_philos != 1 && philo->number_of_philos % 2 != 0)
		usleep_cut(philo, philo->time_to_eat);
	pthread_mutex_lock(philo->action->lock_action);
	if (init == 1)
		pthread_mutex_unlock(philo->action->lock_action);
}

static void	sleep(t_philo_attributes *philo, long int time_mili_start, int *a)
{
	if (philo->action->action_type != STOP)
		philo->action->action_type = SLEEP;
	lock_mutex_and_print_message(philo, time_mili_start);
	*a = -1;
}

int	check_dead_or_stop(t_philo_attributes *philo)
{
	int	return_value;

	return_value = 0;
	if (philo->action->action_type == STOP
		|| philo->action->action_type == HUNGER_STRIKE)
		return_value = 1;
	return (return_value);
}
