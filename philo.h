#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data
{
	int				philonb;
	int				*finished_eating;
	int				ttd;
	int				tte;
	int				tts;
	int				eatnb;
	pthread_mutex_t	*mutex;
	int				*mutex_time;
	pthread_mutex_t	mutex_write;
	long long		start;
	int				death;
}					t_data;

typedef struct s_routine_data
{
	int			i;
	int			next;
	long long	lastmeal;
	int			eatnb;
	int			first;
}				t_routine_data;

long long	gettime_ms(void);
void		*routine(void *arg);

int			ft_atoi(const char *nptr);
size_t		ft_strlen(char *str);
void		ft_putchar(char c);
void		ft_putnbr(long long n);
int			finished_eating(t_data *data);

#endif
