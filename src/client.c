/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oishchen <oishchen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:49:51 by oishchen          #+#    #+#             */
/*   Updated: 2025/04/20 13:17:56 by oishchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t end;

void ft_sig_handler()
{
	end = 1;
}

int	fetch_srv_pid(char *str)
{
	int	srv_pid = ft_atoi(str);
	return (srv_pid);
}

void	send_char(char c, pid_t srv_pid)
{
	int	j;

	j = 7;
	while (j >= 0)
	{
		if ((c >> j) & 1)
		{
			// printf("the bit we are sinding is: %d\n", (c >> j) & 1);
			if (kill(srv_pid, SIGUSR1) == -1)
				ft_printf("Kill failed\n");
		}
		else
			if (kill(srv_pid, SIGUSR2) == -1)
				ft_printf("Kill failed\n");
		j--;
		usleep(25);
	}
}


int	main(int argc, char *argv[])
{
	
	pid_t				srv_pid;
	int					i;

	if (argc == 3)
	{
		end = 0;
		i = 0;
		signal(SIGUSR1, ft_sig_handler);
		srv_pid = fetch_srv_pid(argv[1]);
		ft_printf("we got the pid correctly: %d\n", srv_pid);
		while (argv[2][i])
		{
			send_char(argv[2][i], srv_pid);
			// printf("we send the %d correctly\n", i);
			i++;
		}
		// printf("we are out of the loop %d\n", srv_pid);
		send_char('\0', srv_pid);
		while (!end)
			pause();
		ft_printf("The whole message was received, terminating the program\n");
	}
	return (0);
}