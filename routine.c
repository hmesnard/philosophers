#include "philo.h"

long long	gettime_ms(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return (tp.tv_sec * 1000 + tp.tv_usec / 1000);
}

int	print_philo(int i, char *str, t_data *data)
{
	pthread_mutex_lock(&data->mutex_write);
	while (data->death)
		usleep(10000);
	if (str[1] == 'd')
		data->death = 1;
	ft_putnbr(gettime_ms() - data->start);
	ft_putchar(' ');
	ft_putnbr(i + 1);
	write(1, str, ft_strlen(str));
	pthread_mutex_unlock(&data->mutex_write);
	return (0);
}

void	my_wait(int i, int time, long long lastmeal, t_data *data)
{
	long long	start;

	start = gettime_ms() - data->start;
	if (data->philonb == 1)
	{
		data->philonb = 0;
		print_philo(i, " has taken a fork\n", data);
		my_wait(i, data->ttd * 2, lastmeal, data);
	}
	while (1)
	{
		if (gettime_ms() >= lastmeal + data->ttd)
		{
			print_philo(i, " died\n", data);
			while (1)
				usleep(10000);
		}
		if (gettime_ms() - data->start >= start + time)
			break ;
		usleep(500);
	}
}

int	eating_loop(t_data *data, t_routine_data *rd)
{
	print_philo(rd->i, " is thinking\n", data);
	if ((rd->first && rd->i % 2 == 1) || data->philonb == 1)
		my_wait(rd->i, data->tte / 2, rd->lastmeal, data);
	if (data->mutex_time[rd->i] > rd->lastmeal - data->start + data->ttd)
		my_wait(rd->i, data->ttd, rd->lastmeal, data);
	else
		pthread_mutex_lock(data->mutex + rd->i);
	print_philo(rd->i, " has taken a fork\n", data);
	if (data->mutex_time[rd->next] > rd->lastmeal - data->start + data->ttd)
		my_wait(rd->i, data->ttd, rd->lastmeal, data);
	else
		pthread_mutex_lock(data->mutex + rd->next);
	data->mutex_time[rd->i] = gettime_ms() - data->start + data->tte;
	data->mutex_time[rd->next] = gettime_ms() - data->start + data->tte;
	print_philo(rd->i, " has taken a fork\n", data);
	if (gettime_ms() - rd->lastmeal >= data->ttd)
		return (print_philo(rd->i, " died\n", data));
	rd->lastmeal = gettime_ms();
	print_philo(rd->i, " is eating\n", data);
	my_wait(rd->i, data->tte, rd->lastmeal, data);
	pthread_mutex_unlock(data->mutex + rd->i);
	pthread_mutex_unlock(data->mutex + rd->next);
	print_philo(rd->i, " is sleeping\n", data);
	my_wait(rd->i, data->tts, rd->lastmeal, data);
	return (1);
}

void	*routine(void *arg)
{
	static int		sta;
	t_data			*data;
	t_routine_data	rd;

	data = arg;
	pthread_mutex_lock(&data->mutex_write);
	rd.i = sta;
	sta++;
	pthread_mutex_unlock(&data->mutex_write);
	rd.next = (rd.i + 1) % data->philonb;
	rd.eatnb = data->eatnb;
	rd.lastmeal = data->start;
	rd.first = 1;
	while (rd.eatnb)
	{
		if (!eating_loop(data, &rd))
			return (NULL);
		if (rd.eatnb > 0)
			rd.eatnb--;
		rd.first = 0;
	}
	data->finished_eating[rd.i] = 1;
	return (NULL);
}
