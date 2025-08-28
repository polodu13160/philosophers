/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 04:11:53 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/28 10:46:14 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "stdio.h"

static int	print_message(t_philo_attributes *philo,
				long int time_mili_start);
static int	print_message_eat_and_finish_action(t_philo_attributes *philo,
				long int time_now_in_mili_at_start);
static int	print_message_sleep_and_finish_action(t_philo_attributes *philo,
				long int time_now_in_mili_at_start);

void	lock_mutex_and_print_message(t_philo_attributes *philo,
		long int time_start)
{
	pthread_mutex_unlock(philo->action->lock_action);
	pthread_mutex_lock(philo->lock_print_action);
	if (check_dead_or_stop(philo) == 1)
	{
		pthread_mutex_unlock(philo->lock_print_action);
		pthread_mutex_lock(philo->action->lock_action);
		return ;
	}
	if (check_dead_or_stop(philo) != 1 && print_message(philo, time_start) > 0)
		philo->action->action_type = STOP;
	pthread_mutex_unlock(philo->lock_print_action);
	pthread_mutex_lock(philo->action->lock_action);
}

static int	print_message(t_philo_attributes *philo,
		long int time_mili_start)
{
	long int	time_now_in_mili_at_start;
	int			usleep_return_value;

	usleep_return_value = 0;
	time_now_in_mili_at_start = calc_time(time_mili_start);
	if (time_now_in_mili_at_start <= -1)
	{
		print_error_time("error Time in thread");
		philo->error_time = 1;
		return (2);
	}
	if (philo->action->action_type == SLEEP)
		return (print_message_sleep_and_finish_action(philo,
				time_now_in_mili_at_start));
	if (philo->action->action_type == FORK)
	{
		printf("%lu %ld has taken a fork\n", time_now_in_mili_at_start,
			philo->id);
	}
	if (philo->action->action_type == EAT)
		return (print_message_eat_and_finish_action(philo,
				time_now_in_mili_at_start));
	if (philo->action->action_type == THINK)
		printf("%lu %ld is thinking\n", time_now_in_mili_at_start, philo->id);
	return (0);
}

static int	print_message_eat_and_finish_action(t_philo_attributes *philo,
		long int time_now_in_mili_at_start)
{
	if (philo->rest_number_eat <= -1 || philo->rest_number_eat > 0)
	{
		printf("%lu %ld is eating\n", time_now_in_mili_at_start, philo->id);
		philo->last_time_to_eat = time_now();
		pthread_mutex_unlock(philo->lock_print_action);
		usleep_cut(philo, philo->time_to_eat);
		pthread_mutex_lock(philo->lock_print_action);
		philo->rest_number_eat--;
	}
	philo->have_forks = 0;
	philo->attr_left_fork->available = 0;
	philo->attr_right_fork->available = 0;
	pthread_mutex_unlock(philo->attr_left_fork->lock_fork);
	pthread_mutex_unlock(philo->attr_right_fork->lock_fork);
	if (philo->rest_number_eat == 0 || philo->last_time_to_eat == -1)
	{
		philo->finish = 1;
		if (philo->last_time_to_eat == -1)
		{
			print_error_time("error Time in thread");
			return (2);
		}
		return (1);
	}
	return (0);
}

static int	print_message_sleep_and_finish_action(t_philo_attributes *philo,
		long int time_now_in_mili_at_start)
{
	int	usleep_return_value;

	usleep_return_value = 0;
	printf("%lu %ld is sleeping\n", time_now_in_mili_at_start, philo->id);
	pthread_mutex_unlock(philo->lock_print_action);
	usleep_return_value = usleep_cut(philo, philo->time_to_sleep);
	pthread_mutex_lock(philo->lock_print_action);
	return (usleep_return_value);
}

void	*print_error_time(char *message)
{
	perror(message);
	return (NULL);
}
