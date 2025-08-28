#ifndef H_PHILOSOPHER
# define H_PHILOSOPHER

# include "stdbool.h"
# include <pthread.h>
# include <sys/time.h>

typedef enum e_action_type
{
	INIT,
	FORK,
	EAT,
	SLEEP,
	THINK,
	STOP,
	HUNGER_STRIKE
}								t_action_type;

typedef struct t_action_mutex
{
	pthread_mutex_t				*lock_action;
	t_action_type				action_type;
}								t_action_mutex;

typedef struct t_fork
{
	pthread_mutex_t				*lock_fork;
	int							fork;
	bool						available;
	pthread_mutex_t				*table_mutex;
}								t_fork;

typedef struct s_philosopher_attributes

{
	pthread_t					value_thread;
	bool						finish;
	long int					last_time_to_eat;
	long int					time_to_eat;
	long int					time_to_sleep;
	long int					rest_number_eat;
	int							id;
	pthread_mutex_t				*lock_print_action;
	t_fork						*attr_left_fork;
	t_fork						*attr_right_fork;
	long int					number_of_philos;
	bool						have_forks;
	t_action_mutex				*action;
	long int					*time_start;
}								t_philosopher_attributes;

typedef struct s_philosopher_info
{
	long int					number_of_philosophers;
	pthread_mutex_t				lock_print_action;
	long int					time_to_die;
	long int					time_to_eat;
	long int					time_to_sleep;
	long int					number_of_times_each_philosopher_must_eat;
	t_action_mutex				*action_mutex;
	t_philosopher_attributes	*philosophers;
	t_fork						*forks;
}								t_philosopher_info;

// times.c

long int						time_now(void);
long int						calc_time(long time_mili_start);

// print.c

void							*print_error_return_null(char *message);
int								ft_message(t_philosopher_attributes *philo,
									long int time_mili_start);
void							ft_new_message(t_philosopher_attributes *philo,
									long int time_start);

// actions.c
int								take_forks(t_philosopher_attributes *philo,
									int long time_start);
void							*while_action_philo(long int time_mili_start,
									t_philosopher_attributes *philo);
// parsing.c
int								parsing_argv(char **argv,
									t_philosopher_info *list, int argc);
// init.c
int	init_list_info(char **argv, int argc,t_philosopher_info *list);
void							attr_forks_philo(t_philosopher_attributes *philosophers,
									t_fork *forks, int number_of_philosophers);
// free_and_destroy_mutex.c
void							free_philo(t_philosopher_attributes *philo);
void	*free_destoy_tab_forks(t_fork *forks, pthread_mutex_t	*lock_mutex, int i);
void *free_destroy_tab_action(t_action_mutex *action, pthread_mutex_t	*malloc_action_mutex,int index_mutex_crash, int nb_philo);

#endif