/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 04:21:10 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/30 21:02:56 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include "stdlib.h"

static t_fork			*init_fork(int number_philo);
static t_action_mutex	*init_action_mutex(long int nb_philo);
static void				start_init_list_info(t_philo_info *list);

int	init_list_info(char **argv, int argc, t_philo_info *list)
{
	pthread_mutex_t	mutex_print;

	start_init_list_info(list);
	if (check_parsing(argv, list, argc) == 1)
		return (1);
	list->philosophers = malloc(sizeof(t_philo_attributes)
			* (list->number_of_philosophers + 1));
	if (list->philosophers == NULL)
		return (1);
	list->forks = init_fork(list->number_of_philosophers);
	if (list->forks == NULL)
		return (destroy_and_free_malloc(list));
	list->action_mutex = init_action_mutex(list->number_of_philosophers);
	if (list->action_mutex == NULL)
		return (destroy_and_free_malloc(list));
	if (pthread_mutex_init(&mutex_print, NULL) == -1)
		return (destroy_and_free_malloc(list));	
	list->lock_print_action = mutex_print;
	return (0);
}

void	init_philos_attributes(t_philo_info *list, long int *time_start)
{
	int	i;

	i = 0;
	while (i < list->number_of_philosophers)
	{
		list->philosophers[i].id = i + 1;
		list->philosophers[i].time_to_sleep = list->time_to_sleep;
		list->philosophers[i].time_to_eat = list->time_to_eat;
		list->philosophers[i].rest_number_eat = \
list->number_of_times_each_philosopher_must_eat;
		list->philosophers[i].lock_print_action = &list->lock_print_action;
		list->philosophers[i].action = &list->action_mutex[i];
		list->philosophers[i].finish = 0;
		list->philosophers[i].time_start = time_start;
		list->philosophers[i].error_time = 0;
		list->philosophers[i].last_time_to_eat = -2;
		i++;
	}
	list->philosophers[i].id = -1;
	attr_forks_philo(list->philosophers, list->forks,
		list->number_of_philosophers);
}

static t_fork	*init_fork(int number_philo)
{
	int				i;
	t_fork			*forks;
	pthread_mutex_t	*lock_mutex;

	lock_mutex = NULL;
	forks = malloc(sizeof(t_fork) * (number_philo + 1));
	lock_mutex = malloc(sizeof(pthread_mutex_t) * (number_philo));
	if (forks == NULL || lock_mutex == NULL)
		return (free_destoy_tab_forks(forks, lock_mutex, -1));
	i = 0;
	while (i < number_philo)
	{
		forks[i].fork = -1;
		forks[i].table_mutex = lock_mutex;
		if (pthread_mutex_init(&lock_mutex[i], NULL) == -1)
			return (free_destoy_tab_forks(forks, NULL, i));
		forks[i].lock_fork = &lock_mutex[i];
		forks[i].fork = i;
		forks[i].id_philo_take = -1;
		i++;
	}
	forks[i].fork = -1;
	return (forks);
}

static void	start_init_list_info(t_philo_info *list)
{
	list->number_of_philosophers = 0;
	list->philosophers = NULL;
	list->action_mutex = NULL;
	list->forks = NULL;
	list->number_of_times_each_philosopher_must_eat = -1;
	list->time_to_die = 0;
	list->time_to_eat = 0;
	list->time_to_sleep = 0;
}

static t_action_mutex	*init_action_mutex(long int nb_philo)
{
	t_action_mutex	*malloc_action;
	pthread_mutex_t	*malloc_action_mutex;
	int				i;

	malloc_action = malloc(sizeof(t_action_mutex) * nb_philo);
	malloc_action_mutex = malloc(sizeof(pthread_mutex_t) * nb_philo);
	if (malloc_action == NULL || malloc_action_mutex == NULL)
		return (free_destroy_tab_action(malloc_action, malloc_action_mutex, -1,
				nb_philo));
	i = 0;
	while (i < nb_philo)
	{
		malloc_action[i].tab_lock_action = malloc_action_mutex;
		if (pthread_mutex_init(&malloc_action_mutex[i], NULL) == -1)
			return (free_destroy_tab_action(malloc_action, malloc_action_mutex,
					i, nb_philo));
		malloc_action[i].lock_action = &malloc_action_mutex[i];
		malloc_action[i++].action_type = INIT;
	}
	return (malloc_action);
}
