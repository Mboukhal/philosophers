/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukhal <mboukhal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/17 14:01:02 by mboukhal          #+#    #+#             */
/*   Updated: 2022/05/23 22:57:46 by mboukhal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <limits.h>
/*

	4 410 200 200

	*[number_of_philo] 				philo_nb	4
	*[time_to_die] 					tt_die		410
	*[time_to_eat] 					tt_eat		200
	*[time_to_sleep] 				tt_sleep	200
	
	 [number_of_times_each_max_meal	max_meal	-1
	 philosopher_must_eat] 

	◦ timestamp_in_ms X has taken a fork
	◦ timestamp_in_ms X is eating
	◦ timestamp_in_ms X is sleeping
	◦ timestamp_in_ms X is thinking
	◦ timestamp_in_ms X died

*/

struct	s_rules;
//			privet
typedef struct philo
{
	pthread_t		tid;
	int				id;
	int				l_fork;
	int				r_fork;
	int				meal_nb;
	long long		last_meal_time;
	long long		t1;
	struct s_rules	*root;

}			t_philo;

//			public
typedef struct s_rules
{
	int				philo_nb;
	int				tt_die;
	int				tt_sleep;
	int				tt_eat;
	int				max_meal;
	long long		start_time;
	int				status;
	int				all_done;
	pthread_mutex_t	fork[250];
	pthread_mutex_t	stdout_lock;
	pthread_mutex_t	time_lock;
	pthread_mutex_t	done_lock;
	t_philo			philo[250];		

}				t_rule;

void			print_out(t_philo *p, char *message);
int				w_error(char *str);
int				emulation(t_rule *rule);
int				init_rules(t_rule *rules, char **av, int ac);
long long		now_time(void);
void			clean_exit(t_rule *r);
int				cheak_for_death(t_rule *public);
unsigned int	psleep(int s);

#endif // !PHILO_H
