/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 04:48:24 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/28 10:46:14 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include <unistd.h>

static void	take_fork_left(t_philo_attributes *philo,
				int long time_start);

static int	take_forks_one_philo(t_philo_attributes *philo,
				int long time_start);

static void	take_fork_right(t_philo_attributes *philo,
				int long time_start);

int	take_forks(t_philo_attributes *philo, int long time_start)
{
	if (philo->number_of_philos == 1)
		return (take_forks_one_philo(philo, time_start));
	pthread_mutex_unlock(philo->action->lock_action);
	if (philo->id % 2 == 0)
		take_fork_left(philo, time_start);
	else
		take_fork_right(philo, time_start);
	if (philo->id % 2 == 0)
		take_fork_right(philo, time_start);
	else
		take_fork_left(philo, time_start);
	philo->have_forks = 1;
	pthread_mutex_lock(philo->action->lock_action);
	return (0);
}

static void	take_fork_left(t_philo_attributes *philo, int long time_start)
{
	pthread_mutex_lock(philo->attr_left_fork->lock_fork);
	philo->attr_left_fork->available = 1;
	pthread_mutex_lock(philo->action->lock_action);
	if (check_dead_or_stop(philo) == 0)
		philo->action->action_type = FORK;
	lock_mutex_and_print_message(philo, time_start);
	pthread_mutex_unlock(philo->action->lock_action);
}

static int	take_forks_one_philo(t_philo_attributes *philo,
		int long time_start)
{
	take_fork_left(philo, time_start);
	if (check_dead_or_stop(philo) == 0)
		philo->action->action_type = HUNGER_STRIKE;
	while (1)
	{
		pthread_mutex_unlock(philo->action->lock_action);
		usleep(500);
		pthread_mutex_lock(philo->action->lock_action);
		if (check_dead_or_stop(philo) == 1
			&& philo->action->action_type != HUNGER_STRIKE)
			return (1);
	}
}

static void	take_fork_right(t_philo_attributes *philo,
		int long time_start)
{
	pthread_mutex_lock(philo->attr_right_fork->lock_fork);
	philo->attr_right_fork->available = 1;
	pthread_mutex_lock(philo->action->lock_action);
	if (check_dead_or_stop(philo) == 0)
		philo->action->action_type = FORK;
	lock_mutex_and_print_message(philo, time_start);
	pthread_mutex_unlock(philo->action->lock_action);
}
