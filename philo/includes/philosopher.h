#ifndef H_PHILOSOPHER
# define H_PHILOSOPHER

# include <pthread.h>
# include <sys/time.h>

typedef enum e_action
{
	INIT,
	FORK,
	EAT,
	SLEEP,
	THINK,
	STOP,
	HUNGER_STRIKE
}					t_action;

typedef struct t_action_mutex
{
	pthread_mutex_t *lock_action;
	int 			action;
}				t_action_mutex;

typedef struct t_fork
{
	pthread_mutex_t	*lock_fork;
	int				fork;
	pthread_mutex_t *table_mutex;
}					t_fork;

typedef struct s_pnj

{
	pthread_t		value_thread;
	int				finish;
	long int		rest_time_to_die;
	long int		last_time_to_eat;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		rest_number_eat;
	int				philo_number;
	pthread_mutex_t	*lock_print_action;
	t_fork			*attr_left_fork;
	t_fork			*attr_right_fork;
	long int		number_of_philos;
	int				have_forks;
	t_action_mutex	*action;
	long int		*time_start;
}					t_pnj;

typedef struct s_philo
{
	long int		number_of_philosophers;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		number_of_times_each_philosopher_must_eat;
	t_action_mutex  *action_mutex;
	t_pnj			*philosophers;
	t_fork			*forks;
}					t_philo;
#endif