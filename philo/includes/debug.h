/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 04:37:19 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/21 23:02:55 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#ifndef DEBUG_H
# define DEBUG_H
# ifndef DEBUG
#  define DEBUG 0
# endif

int		printf_philo(t_pnj *philo);
void	print_time(long int value);
void	printf_forks(t_fork *forks);

#endif