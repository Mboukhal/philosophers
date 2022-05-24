/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   emulation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukhal <mboukhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/15 13:03:09 by mboukhal          #+#    #+#             */
/*   Updated: 2022/05/23 23:08:47 by mboukhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    *thread_action(void *arg)
{
    t_philo *ph;
    t_rule *public;

    ph = (t_philo *)arg;
    public = ph->root;
    while(public->status)
    {
        pthread_mutex_lock(&(public->fork[ph->r_fork]));
        pthread_mutex_lock(&(ph->root->stdout_lock));
        print_out(ph, "has taken a fork");
        pthread_mutex_unlock(&(ph->root->stdout_lock));
        pthread_mutex_lock(&(public->fork[ph->l_fork]));
        pthread_mutex_lock(&(ph->root->stdout_lock));
        print_out(ph, "has taken a fork");
        pthread_mutex_unlock(&(ph->root->stdout_lock));
        pthread_mutex_lock(&(ph->root->stdout_lock));
        print_out(ph, "is eating");
        pthread_mutex_unlock(&(ph->root->stdout_lock));
        psleep(public->tt_eat);
        pthread_mutex_unlock(&(public->fork[ph->r_fork]));
        pthread_mutex_unlock(&(public->fork[ph->l_fork]));
        ph->last_meal_time = now_time();
        ph->meal_nb++;
        if (ph->meal_nb == public->max_meal)
            break;
        pthread_mutex_lock(&(ph->root->stdout_lock));
        print_out(ph, "is sleeping");
        pthread_mutex_unlock(&(ph->root->stdout_lock));
        psleep(ph->root->tt_sleep);
        pthread_mutex_lock(&(ph->root->stdout_lock));
        print_out(ph, "is thinking");
        pthread_mutex_unlock(&(ph->root->stdout_lock));
    }
        public->all_done++;
    return (NULL);
}

int    main_cheak_for_death(t_rule *public)
{
    int i;

    if (public->all_done >= public->philo_nb)
    {
        pthread_mutex_lock(&(public->stdout_lock));
        printf("Zrda salat\n");
        return (EXIT_FAILURE);
    }
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
    usleep(300);
    return (EXIT_SUCCESS);
}

int emulation(t_rule *r)
{
    int i;
    int x;
    t_rule *public;

    public = r;
    public->all_done = 0;
    i = -1;
    x = 0;
    r->start_time = now_time();
    while (++i < r->philo_nb)
    {
        if ((i % 2) == 1)
            if (pthread_create(&(r->philo[i].tid), NULL, thread_action, &(r->philo[i])))
                return(EXIT_FAILURE);
    }
    usleep(300);
    i = -1;
    while (++i < r->philo_nb)
    {
        if ((i % 2) == 0)
            if (pthread_create(&(r->philo[i].tid), NULL, thread_action, &(r->philo[i])))
                return(EXIT_FAILURE);
    }
    usleep(50);
    if (r->philo_nb == 1)
    {
        printf("%u ms 1 died\n", psleep(r->tt_die));
        clean_exit(r);
        return(EXIT_SUCCESS);
    }
    while (!main_cheak_for_death(public));
    clean_exit(r);
    return(EXIT_SUCCESS);
}
