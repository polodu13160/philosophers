#ifndef H_PHILOSOPHER
# define H_PHILOSOPHER

# include <pthread.h>
#include <sys/time.h>



typedef enum e_action
{
	FORK,
	EAT,
	SLEEP,
	THINK,
}					t_action;

typedef struct s_pnj

{
	pthread_t		value_thread;
	int				philo_number;
	int				action;
	struct s_pnj	*next;
}					t_pnj;

typedef struct s_philo
{
	long int		number_of_philosophers;
	long int		time_to_die;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		number_of_times_eahc_philosopher_must_eat;
	struct timeval	time_start;
	t_pnj			*philo_uno;
	t_pnj			*philosophers;
}					t_philo;
#endif