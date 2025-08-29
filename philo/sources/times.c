/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   times.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 18:22:08 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/29 13:54:40 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include <sys/time.h>

long int	time_now(void)
{
	struct timeval	time_now;

	gettimeofday(&time_now, NULL);
	return (time_now.tv_sec * 1000 + time_now.tv_usec / 1000);
}

long int	calc_time(long time_mili_start)
{
	long int	time_mili;
	long int	v_time_now;

	v_time_now = time_now();
	time_mili = v_time_now - time_mili_start;
	return (time_mili);
}
