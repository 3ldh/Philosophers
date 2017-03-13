/*
** main.c for  in /home/sauvau_m/rendu/PSU_2016_philo
**
** Made by Sauvau Mathieu
** Login   <mathieu.sauvau@epitech.eu>
**
** Started on  Mon Mar  6 10:22:43 2017 Sauvau Mathieu
** Last update Mon Mar 13 12:51:17 2017 Sauvau Mathieu
*/

#include <stdio.h>
#include <stdlib.h>
#include "extern.h"
#include "philo.h"

int		g_nb_philo;
t_philo		*g_philo;
bool		*g_chopsticks;
pthread_mutex_t	*g_mutex;

void		eat(t_philo *philo)
{
  philo->state = EAT;
  lphilo_take_chopstick(g_chopsticks[philo->pos]);
  lphilo_take_chopstick(g_chopsticks[(philo->pos + 1) % g_nb_philo]);
  pthread_mutex_lock(g_chopsticks[philo->pos]);
  pthread_mutex_lock(g_chopsticks[(philo->pos + 1) % g_nb_philo]);
  --philo->nb_eat;
  pthread_mutex_unlock(&g_mutex);
  philo->need_to_eat = false;
  lphilo_eat();
  sleep(10);
  lphilo_release_chopstick(g_chopsticks[philo->pos]);
  lphilo_release_chopstick(g_chopsticks[(philo->pos + 1) % g_nb_philo]);
  pthread_mutex_unlock(g_chopsticks[philo->pos]);
  pthread_mutex_unlock(g_chopsticks[(philo->pos + 1) % g_nb_philo]);
}

void		rest(t_philo *philo)
{
  philo->state = REST;
  lphilo_sleep();
}

void		think(t_philo *philo)
{
  philo->state = THINK;
  lphilo_think();
  philo->need_to_eat = true;
}

void		*philo_logic(void *arg)
{
  t_philo	*philo;
  bool		chopstrick_r;
  bool		chopstrick_l;

  philo = (t_philo *)arg;
  while (philo->nb_eat > 0)
    {
      pthread_mutex_lock(g_chopsticks[philo->pos]);
      pthread_mutex_lock(g_chopsticks[(philo->pos + 1) % g_nb_philo]);

      if (philo->state != EAT && chopstrick_l && chopstrick_r)
	eat(philo);
      else if (philo->state != THINK && philo->state != EAT && chopstrick_l)
	think(philo);
      else if (!philo->need_to_eat)
	rest(philo);

    }
  return (NULL);
}


void		init_philo(int nb_eat)
{
  int		i;

  i = -1;
  while (++i < g_nb_philo)
    {
      g_philo[i].state = REST;
      g_philo[i].nb_eat = nb_eat;
      g_philo[i].pos = i;
      g_chopsticks[i] = true;
      pthread_create(&(g_philo[i].thread_ref), NULL, philo_logic, &g_philo[i]);
    }
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

  //TODO -p nb_philo -e max_eat
  if (ac != 5)
    {
      printf("Usage : ./philo -p [nb_philo] -e [max_eat]\n");
      return (1);
    }
  RCFStartup(ac, av);
  g_nb_philo = atoi(av[1]);
  if (!(g_philo = malloc(sizeof(t_philo) * g_nb_philo)) ||
      !(g_chopsticks = malloc(sizeof(int) * g_nb_philo)))
    return (1);
  init_philo(atoi(av[2]));
  wait_threads();
  RCFCleanup();
  return (0);
}
