/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukhal <mboukhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 13:00:21 by mboukhal          #+#    #+#             */
/*   Updated: 2022/05/24 10:03:34 by mboukhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_out(t_philo *p, char *message)
{
	pthread_mutex_lock(&(p->root->stdout_lock));
	printf("%lld ms %d %s\n", now_time() - p->root->start_time,
		p->id, message);
	pthread_mutex_unlock(&(p->root->stdout_lock));
}

int	w_error(void)
{
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
