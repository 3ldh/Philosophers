/*
** main.c for  in /home/sauvau_m/rendu/PSU_2016_philo
**
** Made by Sauvau Mathieu
** Login   <mathieu.sauvau@epitech.eu>
**
** Started on  Mon Mar  6 10:22:43 2017 Sauvau Mathieu
** Last update Tue Mar 14 14:41:18 2017 Sauvau Mathieu
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "extern.h"
#include "philo.h"

int		g_nb_philo;
t_philo		*g_philo;
pthread_mutex_t	*g_chopsticks;

void		eat(t_philo *philo)
{
  printf("philo %d eat\n", philo->pos);
  philo->state = EAT;
  --philo->nb_eat;
  lphilo_eat();
}

void		rest(t_philo *philo)
{
  printf("philo %d rest\n", philo->pos);
  philo->state = REST;
  lphilo_sleep();
}

void		check_unlock(t_philo *philo, int chopstick_l,
			     int chopstick_r, bool record)
{
  if (!chopstick_l)
    {
      pthread_mutex_unlock(&g_chopsticks[philo->pos]);
      if (record)
	lphilo_release_chopstick(&g_chopsticks[philo->pos]);
    }
  if (!chopstick_r)
    {
      pthread_mutex_unlock(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
      if (record)
	lphilo_release_chopstick(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
    }
}

void		check_lock(t_philo *philo, int chopstick_l, int chopstick_r)
{
  if (!chopstick_r)
    lphilo_take_chopstick(&g_chopsticks[philo->pos]);
  if (!chopstick_l)
    lphilo_take_chopstick(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
}

void		think(t_philo *philo, int chopstick_l, int chopstick_r)
{
  bool		b;

  check_lock(philo, chopstick_l, chopstick_r);
  printf("philo %d think\n", philo->pos);
  philo->state = THINK;
  lphilo_think();
  check_unlock(philo, chopstick_l, chopstick_r, false);
  b = false;
  while (philo->state == THINK)
    {
      chopstick_l = pthread_mutex_trylock(&g_chopsticks[philo->pos]);
      chopstick_r = pthread_mutex_trylock(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
      if (!chopstick_l && !chopstick_r)
	{
      	  check_lock(philo, chopstick_l, chopstick_r);
	  eat(philo);
	  check_unlock(philo, chopstick_l, chopstick_r, true);
	  b = true;
	}
      if (!b)
      	check_unlock(philo, chopstick_l, chopstick_r, false);
    }
}

void		cancel_thread(t_philo *philo)
{
  int		i;

  i = -1;
  (void)philo;
  while (++i < g_nb_philo)
    if (g_philo[i].nb_eat == 0)
      pthread_exit(NULL);
}

void		*philo_logic(void *arg)
{
  t_philo	*philo;
  int		chopstick_r;
  int		chopstick_l;

  philo = (t_philo *)arg;
  chopstick_r = 1;
  while (philo->nb_eat > 0)
    {
      cancel_thread(philo);
      chopstick_l = pthread_mutex_trylock(&g_chopsticks[philo->pos]);
      /* chopstick_r = pthread_mutex_trylock(&g_chopsticks[(philo->pos + 1) % g_nb_philo]); */
      /* if (philo->state != EAT && !chopstick_r && !chopstick_l) */
      /* 	{ */
      /* 	  check_lock(philo, chopstick_l, chopstick_r); */
      /* 	  eat(philo); */
      /* 	  check_unlock(philo, chopstick_l, chopstick_r, true); */
      /* 	} */
      /* else */ if (philo->state != EAT && (!chopstick_l))
	think(philo, chopstick_l, chopstick_r);
      else if (philo->state != REST)
	{
	  check_unlock(philo, chopstick_l, chopstick_r, false);
	  rest(philo);
	}
    }
  return (NULL);
}

void		init_philo(int nb_eat)
{
  int		i;

  i = -1;
  while (++i < g_nb_philo)
    {
      g_philo[i].state = DEFAULT;
      g_philo[i].nb_eat = nb_eat;
      g_philo[i].pos = i;
      pthread_mutex_init(&g_chopsticks[i], NULL);
    }
  i = -1;
  while (++i < g_nb_philo)
    pthread_create(&(g_philo[i].thread_ref), NULL, philo_logic, &g_philo[i]);
}

void		wait_threads()
{
  int		i;

  i = -1;
  while (++i < g_nb_philo)
    pthread_join(g_philo[i].thread_ref, NULL);
}

int		main(int ac, char **av)
{
  t_args	args;

  if (ac != 5)
    {
      usage(av);
      return (1);
    }
  if (check_args(av, &args) == 1)
    return (1);
  RCFStartup(ac, av);
  g_nb_philo = args.philo;
  if (!(g_philo = malloc(sizeof(t_philo) * g_nb_philo)) ||
      !(g_chopsticks = malloc(sizeof(pthread_mutex_t) * g_nb_philo)))
    return (1);
  init_philo(args.occur);
  wait_threads();
  RCFCleanup();
  return (0);
}
