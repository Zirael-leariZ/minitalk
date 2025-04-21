/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oishchen <oishchen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:49:51 by oishchen          #+#    #+#             */
/*   Updated: 2025/04/21 16:21:54 by oishchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_full_message = 0;

void	ft_sig_handler(int signum, siginfo_t *info, void *contxt)
{
	static int	bit;

	(void)contxt;
	(void)info;
	if (signum == SIGUSR1)
		bit++;
	else if (signum == SIGUSR2)
	{
		bit++;
		ft_printf("%d, overall bits were received\n", bit);
		g_full_message = 1;
	}
}

void	send_message(unsigned char c, int srv_pid)
{
	int	bit;
	int	kill_sts;

	bit = 0;
	while (bit < 8 && g_full_message != -1)
	{
		if ((c >> bit) & 1)
			kill_sts = kill(srv_pid, SIGUSR1);
		else
			kill_sts = kill(srv_pid, SIGUSR2);
		if (kill_sts == -1)
		{
			ft_printf("ERROR: the kill function failed\n");
			g_full_message = -1;
			return ;
		}
		pause();
		usleep(100);
		bit++;
	}
}

int	main(int ac, char *av[])
{
	struct sigaction	sa;
	pid_t				srv_pid;
	int					i;

	if (ac != 3)
		return (ft_printf("Try <binary> <pid> <message>\n"), 1);
	srv_pid = ft_atoi(av[1]);
	ft_bzero(&sa, sizeof(sa));
	sa.sa_sigaction = ft_sig_handler;
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		return (ft_printf("The SIG mapping failed\n"), 1);
	i = 0;
	while (av[2][i] && g_full_message != -1)
		send_message(av[2][i++], srv_pid);
	send_message('\0', srv_pid);
	if (g_full_message == -1)
		return (1);
	while (!g_full_message)
		pause();
	return (ft_printf("The whole message was received!\n"), 0);
}
