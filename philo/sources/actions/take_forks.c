/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 04:48:24 by pde-petr          #+#    #+#             */
/*   Updated: 2025/09/06 16:37:29 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include <unistd.h>

static void	take_fork_left(t_philo_attributes *philo, int long time_start,
				int *left);

static int	take_forks_one_philo(t_philo_attributes *philo, int long time_start,
				int *left);

static void	take_fork_right(t_philo_attributes *philo, int long time_start,
				int *right);

int	take_forks(t_philo_attributes *philo, int long time_start)
{
	int	left_right[2];

	left_right[0] = 0;
	left_right[1] = 0;
	while (left_right[0] == 0 || left_right[1] == 0)
	{
		pthread_mutex_unlock(philo->action->lock_action);
		if (philo->number_of_philos == 1)
			return (take_forks_one_philo(philo, time_start, &left_right[0]));
		if (philo->id % 2 == 0 && left_right[0] == 0)
			take_fork_left(philo, time_start, &left_right[0]);
		else if (left_right[1] == 0)
			take_fork_right(philo, time_start, &left_right[1]);
		if (philo->id % 2 == 0 && left_right[1] == 0)
			take_fork_right(philo, time_start, &left_right[1]);
		else if (left_right[0] == 0)
			take_fork_left(philo, time_start, &left_right[0]);
		usleep(10);
		pthread_mutex_lock(philo->action->lock_action);
		if (left_right[1] > 0 && left_right[0] > 0)
			return (0);
		if (check_dead_or_stop(philo) == 1)
			return (1);
	}
	return (0);
}

static void	take_fork_left(t_philo_attributes *philo, int long time_start,
		int *left)
{
	pthread_mutex_lock(philo->attr_left_fork->lock_fork);
	if (philo->attr_left_fork->id_philo_take == -1 || *left > 0)
	{
		philo->attr_left_fork->id_philo_take = philo->id;
		if (*left == 1)
			*left = 2;
		else if (*left == 0)
			*left = 1;
	}
	else
		*left = 0;
	pthread_mutex_unlock(philo->attr_left_fork->lock_fork);
	pthread_mutex_lock(philo->action->lock_action);
	if (check_dead_or_stop(philo) == 0 && *left == 1)
	{
		philo->action->action_type = FORK;
		lock_mutex_and_print_message(philo, time_start);
	}
	pthread_mutex_unlock(philo->action->lock_action);
}

static int	take_forks_one_philo(t_philo_attributes *philo, int long time_start,
		int *left)
{
	take_fork_left(philo, time_start, left);
	pthread_mutex_lock(philo->action->lock_action);
	if (check_dead_or_stop(philo) == 0)
		philo->action->action_type = HUNGER_STRIKE;
	while (1)
	{
		pthread_mutex_unlock(philo->action->lock_action);
		usleep(500);
		pthread_mutex_lock(philo->action->lock_action);
		if ((check_dead_or_stop(philo) == 1
				&& philo->action->action_type != HUNGER_STRIKE)
			|| philo->rest_number_eat == 0)
		{
			philo->action->action_type = STOP;
			return (1);
		}
	}
}

static void	take_fork_right(t_philo_attributes *philo, int long time_start,
		int *right)
{
	pthread_mutex_lock(philo->attr_right_fork->lock_fork);
	if (philo->attr_right_fork->id_philo_take == -1 || *right > 0)
	{
		philo->attr_right_fork->id_philo_take = philo->id;
		if (*right == 1)
			*right = 2;
		else if (*right == 0)
			*right = 1;
	}
	else
		*right = 0;
	pthread_mutex_unlock(philo->attr_right_fork->lock_fork);
	pthread_mutex_lock(philo->action->lock_action);
	if (check_dead_or_stop(philo) == 0 && *right == 1)
	{
		philo->action->action_type = FORK;
		lock_mutex_and_print_message(philo, time_start);
	}
	pthread_mutex_unlock(philo->action->lock_action);
}
