/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oishchen <oishchen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:49:51 by oishchen          #+#    #+#             */
/*   Updated: 2025/04/16 20:53:36 by oishchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include "libft.h"

int	fetch_srv_pid(char *str)
{
	int	srv_pid = ft_atoi(str);
	return (srv_pid);
}

int	main(int argc, char *argv[])
{
	if (argc > 3)
	{
		ft_printf("ERROR MESSAGE");
		return (0);
	}
	int	srv_pid = fetch_srv_pid(argv[2]);
	
	kill(serv_pid, SIGUSR1);
}