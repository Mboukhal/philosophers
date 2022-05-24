/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukhal <mboukhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 11:38:04 by mboukhal          #+#    #+#             */
/*   Updated: 2022/05/24 10:50:20 by mboukhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi(char *s)
{
	unsigned long int	result;
	int					i;

	i = 0;
	result = 0;
	if (s[0] == '+')
		i++;
	while (s[i])
		result = (result * 10) + (s[i++] - 48);
	if (result > INT_MAX)
		return (-1);
	return (result);
}

static void	set(t_rule *rules, char **av, int ac)
{
	if (ac == 6)
		rules->max_meal = ft_atoi(av[5]);
	else
		rules->max_meal = -2;
	rules->all_done = 0;
	rules->philo_nb = ft_atoi(av[1]);
	rules->tt_die = ft_atoi(av[2]);
	rules->tt_eat = ft_atoi(av[3]);
	rules->tt_sleep = ft_atoi(av[4]);
}

int	init_rules(t_rule *rules, char **av, int ac)
{
	int	i;
	int	j;

	i = 0;
	while (++i < ac)
	{
		j = -1;
		while (av[i][++j])
			if (!((av[i][j] >= 48 && av[i][j] <= 57) || av[i][0] == '+'))
				return (w_error());
	}
	set(rules, av, ac);
	if (rules->max_meal <= 0 && ac == 6)
		return (w_error());
	if (rules->philo_nb == -1 || rules->tt_die == -1 || rules->tt_eat == -1
		|| rules->tt_sleep == -1 || rules->max_meal == -1)
		return (w_error());
	if (rules->philo_nb < 1 || rules->tt_die < 0
		|| rules->tt_eat < 0 || rules->tt_sleep < 0)
		return (w_error());
	rules->all_done = 0;
	return (EXIT_SUCCESS);
}

static int	init_locks(t_rule *lock)
{
	int	i;

	i = -1;
	while (++i < lock->philo_nb)
	{
		if (pthread_mutex_init(&(lock->fork[i]), NULL))
			break ;
		lock->philo[i].id = i;
		lock->philo[i].l_fork = i;
		lock->philo[i].r_fork = (i + 1) % lock->philo_nb;
		lock->philo[i].root = lock;
		lock->philo[i].meal_nb = 0;
		lock->philo[i].last_meal_time = 0;
	}
	lock->status = 1;
	if (i == lock->philo_nb - 1)
		while (--i >= 0)
			pthread_mutex_destroy(&(lock->fork[i]));
	if (i == 0)
		return (w_error());
	if (pthread_mutex_init(&(lock->done_lock), NULL))
		return (w_error());
	if (pthread_mutex_init(&(lock->stdout_lock), NULL))
		return (w_error());
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av)
{
	t_rule	philo_rules;

	if (ac < 5 || ac > 6)
		return (w_error());
	if (init_rules(&philo_rules, av, ac) || init_locks(&philo_rules))
		return (EXIT_FAILURE);
	else
		return (emulation(&philo_rules));
}
