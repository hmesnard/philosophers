#include "philo.h"

int	make_mutexes(t_data *data, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
	{
		data->finished_eating[i] = 0;
		data->mutex_time[i] = 0;
		pthread_mutex_init(&data->mutex[i++], NULL);
	}
	pthread_mutex_init(&data->mutex_write, NULL);
	return (1);
}

void	destroy_mutexes(t_data *data, int nb)
{
	int	i;

	i = 0;
	while (i < nb)
		pthread_mutex_destroy(&data->mutex[i++]);
	pthread_mutex_destroy(&data->mutex_write);
}

int	init_malloc(t_data *data, pthread_t **th)
{
	*th = malloc(data->philonb * sizeof(pthread_t));
	if (!*th)
		return (0);
	data->finished_eating = malloc(data->philonb * sizeof(int));
	if (!data->finished_eating)
	{
		free(*th);
		return (0);
	}
	data->mutex_time = malloc(data->philonb * sizeof(int));
	if (!data->mutex_time)
	{
		free(*th);
		free(data->finished_eating);
		return (0);
	}
	data->mutex = malloc(data->philonb * sizeof(pthread_mutex_t));
	if (!data->mutex || !make_mutexes(data, data->philonb))
	{
		free(*th);
		free(data->finished_eating);
		free(data->mutex_time);
		return (0);
	}
	return (1);
}

void	main2(t_data *data, pthread_t *th, int argc)
{
	int	i;

	i = 0;
	while (1)
	{
		if (data->death || (argc == 6 && finished_eating(data)))
			break ;
		usleep(1000);
	}
	i = 0;
	while (i < data->philonb)
		pthread_detach(th[i++]);
	destroy_mutexes(data, data->philonb);
	free(data->finished_eating);
	free(data->mutex);
	free(data->mutex_time);
	free(th);
}

int	main(int argc, char **argv)
{
	t_data		data;
	int			i;
	pthread_t	*th;

	if (!(argc == 5 || argc == 6))
		return (0);
	data.philonb = ft_atoi(argv[1]);
	data.ttd = ft_atoi(argv[2]);
	data.tte = ft_atoi(argv[3]);
	data.tts = ft_atoi(argv[4]);
	if (argc == 6)
		data.eatnb = ft_atoi(argv[5]);
	else
		data.eatnb = -1;
	if (!init_malloc(&data, &th))
		return (0);
	data.death = 0;
	i = 0;
	data.start = gettime_ms();
	while (i < data.philonb)
	{
		pthread_create(th + i, NULL, &routine, &data);
		i++;
	}
	main2(&data, th, argc);
}
