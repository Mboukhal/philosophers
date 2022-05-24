/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emulation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukhal <mboukhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 13:03:09 by mboukhal          #+#    #+#             */
/*   Updated: 2022/05/24 11:22:12 by mboukhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	clean_exit(t_rule *r)
{
	int	i;

	i = -1;
	while (++i < r->philo_nb)
		pthread_mutex_destroy(&(r->fork[i]));
	pthread_mutex_destroy(&(r->done_lock));
	pthread_mutex_destroy(&(r->stdout_lock));
}

static void	*thread_action(void *arg)
{
	t_philo	*ph;

	ph = (t_philo *)arg;
	while (ph->root->status)
	{
		pthread_mutex_lock(&(ph->root->fork[ph->r_fork]));
		print_out(ph, "has taken a fork");
		pthread_mutex_lock(&(ph->root->fork[ph->l_fork]));
		print_out(ph, "has taken a fork");
		print_out(ph, "is eating");
		psleep(ph->root->tt_eat);
		pthread_mutex_unlock(&(ph->root->fork[ph->r_fork]));
		pthread_mutex_unlock(&(ph->root->fork[ph->l_fork]));
		ph->last_meal_time = now_time();
		ph->meal_nb++;
		if (ph->meal_nb == ph->root->max_meal)
			break ;
		print_out(ph, "is sleeping");
		psleep(ph->root->tt_sleep);
		print_out(ph, "is thinking");
	}
	ph->root->all_done++;
	return (NULL);
}

static int	main_cheak_for_death(t_rule *public)
{
	int	i;

	if (public->philo_nb == 1)
		printf("%u ms 0 died\n", psleep(public->tt_die));
	if (public->all_done >= public->philo_nb)
		printf("Zrda salat\n");
	if (public->all_done >= public->philo_nb || public->philo_nb == 1)
		return (EXIT_FAILURE);
	i = 0;
	while (i < public->philo_nb && public->philo[i].last_meal_time != 0)
	{
		if ((now_time() - public->philo[i].last_meal_time) >= public->tt_die)
		{
			pthread_mutex_lock(&(public->stdout_lock));
			public->status = 0;
			printf("%lld ms %d died\n", (now_time() - public->start_time), i);
			return (EXIT_FAILURE);
		}
	i++;
	}
	return (EXIT_SUCCESS);
}

int	emulation(t_rule *r)
{
	int	i;

	i = -1;
	r->start_time = now_time();
	while (++i < r->philo_nb)
	{
		if ((i % 2) == 0)
			if (pthread_create(&(r->philo[i].tid), NULL,
					thread_action, &(r->philo[i])))
				return (EXIT_FAILURE);
	}
	usleep(300);
	i = -1;
	while (++i < r->philo_nb)
	{
		if ((i % 2) == 1)
			if (pthread_create(&(r->philo[i].tid), NULL,
					thread_action, &(r->philo[i])))
				return (EXIT_FAILURE);
	}
	while (!main_cheak_for_death(r))
		usleep(300);
	clean_exit(r);
	return (EXIT_SUCCESS);
}
