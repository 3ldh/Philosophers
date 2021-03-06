/*
** philo.h for  in /home/sauvau_m/rendu/PSU_2016_philo
**
** Made by Sauvau Mathieu
** Login   <mathieu.sauvau@epitech.eu>
**
** Started on  Mon Mar  6 10:12:21 2017 Sauvau Mathieu
** Last update Fri Mar 17 10:57:26 2017 Sauvau Mathieu
*/

#ifndef PHILO_H_

# define PHILO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

typedef enum	s_state
  {
    THINK,
    EAT,
    REST,
    DEFAULT,
  }		t_state;

typedef struct	s_philo
{
  t_state	state;
  pthread_t	thread_ref;
  int		nb_eat;
  int		do_break;
  int		pos;
}		t_philo;

typedef struct	s_args
{
  int		philo;
  int		occur;
}		t_args;

/* GLOBAL */
extern int		g_nb_philo;
extern t_philo		*g_philo;
extern pthread_mutex_t	*g_chopsticks;

/* functions */
void		init_philo(int nb_eat);
void		wait_threads();
void		*philo_logic(void *arg);
void		usage(char **av);
int		check_args(char **av, t_args *args);
void		check_unlock(t_philo *philo, int chopstick_l,
			     int chopstick_r, bool record);
void		check_lock(t_philo *philo, int chopstick_l, int chopstick_r);
bool		cancel_thread();

#endif /* ! PHILO_H_ */
