/*
** main.c for  in /home/sauvau_m/rendu/PSU_2016_philo
**
** Made by Sauvau Mathieu
** Login   <mathieu.sauvau@epitech.eu>
**
** Started on  Mon Mar  6 10:22:43 2017 Sauvau Mathieu
** Last update Mon Mar 13 13:42:29 2017 Alexandre BLANCHARD
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "extern.h"
#include "philo.h"

int		g_nb_philo;
t_philo		*g_philo;
pthread_mutex_t	*g_chopsticks;
pthread_mutex_t	g_logic = PTHREAD_MUTEX_INITIALIZER;

void		eat(t_philo *philo)
{
  printf("philo %d eat\n", philo->pos);
  philo->state = EAT;
  --philo->nb_eat;
  philo->need_to_eat = false;
  lphilo_eat();
  sleep(1);
  pthread_mutex_unlock(&g_chopsticks[philo->pos]);
  pthread_mutex_unlock(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
  lphilo_release_chopstick(&g_chopsticks[philo->pos]);
  lphilo_release_chopstick(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
}

void		rest(t_philo *philo)
{
  philo->state = REST;
  sleep(1);
  lphilo_sleep();
  printf("philo %d rest\n", philo->pos);
}

void		think(t_philo *philo)
{
  printf("philo %d think\n", philo->pos);
  philo->state = THINK;
  lphilo_think();
    sleep(1);
  philo->need_to_eat = true;
  pthread_mutex_unlock(&g_chopsticks[philo->pos]);
  lphilo_release_chopstick(&g_chopsticks[philo->pos]);
}

void		*philo_logic(void *arg)
{
  t_philo	*philo;

  philo = (t_philo *)arg;
  while (philo->nb_eat > 0)
    {
      //      pthread_mutex_lock(&g_logic);
      if (pthread_mutex_trylock(&g_chopsticks[philo->pos]) == 0)
	{
	  lphilo_take_chopstick(&g_chopsticks[philo->pos]);
	  if (pthread_mutex_trylock(&g_chopsticks[(philo->pos + 1) % g_nb_philo]) == 0 &&
	      philo->state != EAT)
	    {
	      lphilo_take_chopstick(&g_chopsticks[(philo->pos + 1) % g_nb_philo]);
	      eat(philo);
	    }
	  else
	    think(philo);
	}
      else if (!philo->need_to_eat && philo->state != REST)
	rest(philo);
      //      pthread_mutex_unlock(&g_logic);
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
      pthread_mutex_init(&g_chopsticks[i], NULL);
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
