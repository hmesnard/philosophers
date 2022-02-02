#include "philo.h"

int	ft_atoi(const char *nptr)
{
	int					i;
	unsigned long long	nb;
	int					sign;

	i = 0;
	nb = 0;
	sign = 1;
	while ((9 <= nptr[i] && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
		if (nptr[i++] == '-')
			sign = -1;
	while ('0' <= nptr[i] && nptr[i] <= '9')
	{
		nb = nb * 10 + (nptr[i++] - 48);
		if (nb > 9223372036854775807)
		{
			if (sign == 1)
				return (-1);
			return (0);
		}
	}
	return (nb * sign);
}

size_t	ft_strlen(char *str)
{
	size_t	res;

	if (!str)
		return (0);
	res = 0;
	while (str[res])
		res++;
	return (res);
}

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putnbr(long long n)
{
	while (n < 0)
		usleep(10000);
	if (n >= 10)
		ft_putnbr(n / 10);
	ft_putchar(n % 10 + 48);
}

int	finished_eating(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philonb)
	{
		if (!data->finished_eating[i])
			return (0);
		i++;
	}
	return (1);
}
