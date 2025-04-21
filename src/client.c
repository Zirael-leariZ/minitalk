/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oishchen <oishchen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:49:51 by oishchen          #+#    #+#             */
/*   Updated: 2025/04/21 14:46:32 by oishchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_full_message;

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
		ft_printf("%d, bits were received\n", bit);
		g_full_message = 1;
	}
}

void	send_message(unsigned char c, int srv_pid, int *kill_sts)
{
	int	bit;

	bit = 0;
	while (bit < 8)
	{
		if ((c >> bit) & 1)
			*kill_sts = kill(srv_pid, SIGUSR1);
		else
			*kill_sts = kill(srv_pid, SIGUSR2);
		if (*kill_sts == -1)
			return ;
		pause();
		usleep(100);
		bit++;
	}
}

int	main(int ac, char *av[])
{
	struct sigaction	sa;
	pid_t				srv_pid;
	int					kill_status;
	int					i;

	if (ac != 3)
	{
		ft_printf("Try <binary> <pid> <message>\n");
		return (1);
	}
	srv_pid = ft_atoi(av[1]);
	ft_bzero(&sa, sizeof(sa));
	sa.sa_sigaction = ft_sig_handler;
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigaddset(&sa.sa_mask, SIGUSR2);
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGUSR1, &sa, NULL) == -1
		|| sigaction(SIGUSR2, &sa, NULL) == -1)
		return (ft_printf("The SIG mapping failed\n"), 0);
	g_full_message = 0;
	kill_status = 0;
	i = 0;
	while (av[2][i] && !kill_status)
	{
		send_message(av[2][i], srv_pid, &kill_status);
		i++;
	}
	if (!kill_status)
		send_message('\0', srv_pid, &kill_status);
	while (!g_full_message)
	{
		if (kill_status == -1)
			return (ft_printf("The kill function failed!\n"), 0);
		pause();
	}
	ft_printf("The whole message was received!\n");
	return (0);
}
