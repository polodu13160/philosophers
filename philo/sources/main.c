/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 02:39:39 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/31 20:18:46 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include <unistd.h>

int	main(int argc, char **argv)
{
	int				i;
	t_philo_info	list;
	long int		time_start;
	bool			if_error;

	if_error = 0;
	if (init_list_info(argv, argc, &list) == 1)
		return (1);
	init_philos_attributes(&list, &time_start);
	if (pthread_create_philosophers(&list, &time_start) == 1)
		return (1);
	usleep(500);
	monitoring(&list, time_start);
	i = 0;
	while (list.philosophers[i].id != -1)
	{
		pthread_join(list.philosophers[i].value_thread, NULL);
		i++;
	}
	destroy_and_free_malloc(&list);
	pthread_mutex_destroy(&list.lock_print_action);
	return (if_error);
}
