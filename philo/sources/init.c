/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 04:21:10 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/28 06:30:34 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static t_fork	*init_fork(int number_philo);
static t_action_mutex	*init_action_mutex(long int nb_philo);


int	init_list_info(char **argv, int argc,t_philosopher_info *list)
{
	pthread_mutex_t		mutex_print;

	start_init_list_info(list);
	if (check_parsing(argv, list, argc) == 1)
		return (1);
	list->philosophers = malloc(sizeof(t_philosopher_attributes)
			* (list->number_of_philosophers + 1));
	list->forks = init_fork(list->number_of_philosophers);
	list->action_mutex = init_action_mutex(list->number_of_philosophers);
	if (list->philosophers == NULL || list->forks == NULL
		|| list->action_mutex == NULL)
		return (destroy_and_free_malloc(list));
	if (pthread_mutex_init(&mutex_print, NULL) == -1)
		return (destroy_and_free_malloc(list));
	list->lock_print_action = mutex_print;
	return (0);
}

static t_fork	*init_fork(int number_philo)
{
	int				i;
	t_fork			*forks;
	pthread_mutex_t	*lock_mutex;

	forks = malloc(sizeof(t_fork) * (number_philo + 1));
	lock_mutex = malloc(sizeof(pthread_mutex_t) * (number_philo));
	if (forks == NULL || lock_mutex == NULL)
		free_destoy_tab_forks(forks, lock_mutex, -1);
	i = 0;
	while (i < number_philo)
	{
		forks[i].fork = -1;
		forks[i].table_mutex = lock_mutex;
		if (pthread_mutex_init(&lock_mutex[i], NULL) == -1)
		{
			free_destoy_tab_forks(forks, NULL, i);
			return (NULL);
		}
		forks[i].lock_fork = &lock_mutex[i];
		forks[i].fork = i;
		forks[i].available = 0;
		i++;
	}
	forks[i].fork = -1;
	return (forks);
}

void	attr_forks_philo(t_philosopher_attributes *philosophers, t_fork *forks,
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
		if (pthread_mutex_init(&malloc_action_mutex[i], NULL) == -1)
			return (free_destroy_tab_action(malloc_action, malloc_action_mutex,
					i, nb_philo));
		malloc_action[i].lock_action = &malloc_action_mutex[i];
		malloc_action[i++].action_type = INIT;
	}
	return (malloc_action);
}
