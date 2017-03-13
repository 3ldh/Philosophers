/*
** philo.h for  in /home/sauvau_m/rendu/PSU_2016_philo
**
** Made by Sauvau Mathieu
** Login   <mathieu.sauvau@epitech.eu>
**
** Started on  Mon Mar  6 10:12:21 2017 Sauvau Mathieu
** Last update Mon Mar 13 13:33:15 2017 Sauvau Mathieu
*/

#ifndef PHILO_H_

# define PHILO_H_

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
  bool		need_to_eat;
  int		pos;
}		t_philo;

/* GLOBAL */
extern int		g_nb_philo;
extern t_philo		*g_philo;
extern pthread_mutex_t	*g_chopsticks;
extern pthread_mutex_t	g_logic;

/* functions */
void		init_philo(int nb_eat);
void		wait_threads();
void		*philo_logic(void *arg);

#endif /* ! PHILO_H_ */
