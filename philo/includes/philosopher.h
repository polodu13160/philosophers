/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pde-petr <pde-petr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 10:39:56 by pde-petr          #+#    #+#             */
/*   Updated: 2025/08/31 14:22:48 by pde-petr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include "stdbool.h"
# include <pthread.h>

typedef enum e_action_type
{
	INIT,
	FORK,
	EAT,
	SLEEP,
	THINK,
	STOP,
	HUNGER_STRIKE
}						t_action_type;

typedef struct t_action_mutex
{
	pthread_mutex_t		*lock_action;
	t_action_type		action_type;
	pthread_mutex_t		*tab_lock_action;
}						t_action_mutex;

typedef struct t_fork
{
	pthread_mutex_t		*lock_fork;
	long int			fork;
	long int			id_philo_take;
	pthread_mutex_t		*table_mutex;
}						t_fork;

typedef struct s_philosopher_attributes
{
	pthread_t			value_thread;
	bool				finish;
	long int			last_time_to_eat;
	long int			time_to_eat;
	long int			time_to_sleep;
	long int			rest_number_eat;
	long int			number_of_philos;
	long int			id;
	pthread_mutex_t		*lock_print_action;
	t_fork				*attr_left_fork;
	t_fork				*attr_right_fork;
	t_action_mutex		*action;
	long int			*time_start;
	bool				error_time;
}						t_philo_attributes;

typedef struct s_philosopher_info
{
	long int			number_of_philosophers;
	pthread_mutex_t		lock_print_action;
	long int			time_to_die;
	long int			time_to_eat;
	long int			time_to_sleep;
	long int			number_of_times_each_philosopher_must_eat;
	t_action_mutex		*action_mutex;
	t_philo_attributes	*philosophers;
	t_fork				*forks;
}						t_philo_info;

// times.c

long int				time_now(void);
long int				calc_time(long time_mili_start);

// print.c

void					*print_error_time(char *message,
							t_philo_attributes *philo);
void					lock_mutex_and_print_message(t_philo_attributes *philo,
							long int time_start);

// take_forks.c
int						take_forks(t_philo_attributes *philo,
							int long time_start);
// change_actions.c
void					*while_action_philo(long int time_mili_start,
							t_philo_attributes *philo);
int						check_dead_or_stop(t_philo_attributes *philo);
// parsing.c
int						check_parsing(char **argv, t_philo_info *list,
							int argc);
// init.c
int						init_list_info(char **argv, int argc,
							t_philo_info *list);
void					attr_forks_philo(t_philo_attributes *philosophers,
							t_fork *forks, int number_of_philosophers);
void					init_philos_attributes(t_philo_info *list,
							long int *time_start);
// free_and_destroy_mutex.c
void					free_philo(t_philo_attributes *philo);
void					*free_destoy_tab_forks(t_fork *forks,
							pthread_mutex_t *lock_mutex, int i);
void					*free_destroy_tab_action(t_action_mutex *action,
							pthread_mutex_t *malloc_action_mutex,
							int index_mutex_crash, int nb_philo);
int						destroy_and_free_malloc(t_philo_info *list);
// monitoring.c
void					monitoring(t_philo_info *list, long int time_start);
// utils.c
long int				ft_atol_no_neg(char *argv);
int						usleep_cut(t_philo_attributes *philo,
							long int time_limit);
// attribution_forks_and_create_threads.c
void					attr_forks_philo(t_philo_attributes *philosophers,
							t_fork *forks, int number_of_philosophers);
int						pthread_create_philosophers(t_philo_info *list,
							long int *time_start);
#endif