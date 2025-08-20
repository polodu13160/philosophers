/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 04:32:52 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/20 23:03:41 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "philosopher.h"
#include <stdio.h>

int	printf_philo(t_pnj *philo)
{
    int i;
    
    i = 0;
    while(philo[i].philo_number != -1)
    {
        dprintf(2,"\nphilo number = %d\n",philo[i++].philo_number);
    }
    return 0;
}

void print_time(long int value)
{
    dprintf(2, "time = %ld\n", value);
}
