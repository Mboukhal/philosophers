/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukhal <mboukhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 13:00:21 by mboukhal          #+#    #+#             */
/*   Updated: 2022/05/23 22:55:17 by mboukhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	w_error(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	write(STDERR_FILENO, "Error\n", 7);
	return (EXIT_FAILURE);
}

unsigned int	psleep(int s)
{
	long long	t;

	t = now_time();
	while ((now_time() - t) < s)
		usleep(50);
	return ((unsigned int)(now_time() - t));
}

long long	now_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	clean_exit(t_rule *r)
{
	int	i;

	i = -1;
	while (++i < r->philo_nb)
		pthread_mutex_destroy(&(r->fork[i]));
	pthread_mutex_destroy(&(r->done_lock));
	pthread_mutex_destroy(&(r->stdout_lock));
}
