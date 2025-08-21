#ifndef H_PHILOSOPHER
# define H_PHILOSOPHER

# include <pthread.h>
# include <sys/time.h>

typedef enum e_action
{
	FORK,
	EAT,
	SLEEP,
	THINK,
}					t_action;

typedef struct fork
{
	pthread_mutex_t	*lock_fork;
	int				fork;
	pthread_mutex_t *table_mutex;
}					t_fork;

typedef struct s_pnj

{
	pthread_t		value_thread;
	long int		rest_time_to_die;
	long int		rest_time_to_eat;
	long int		rest_time_to_sleep;
	long int		rest_number_eat;
	int				philo_number;
	pthread_mutex_t	*lock_print_action;
	t_fork			*attr_left_fork;
	t_fork			*attr_right_fork;
	int				action;
	struct timeval	time_start;
}					t_pnj;

typedef struct s_philo
{
	long int		number_of_philosophers;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		number_of_times_each_philosopher_must_eat;
	t_pnj			*philosophers;
	t_fork			*forks;
}					t_philo;
#endif