/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oishchen <oishchen@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 08:49:44 by oishchen          #+#    #+#             */
/*   Updated: 2025/04/16 20:51:31 by oishchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "libft.h"

void	print_pid(void)
{
	getpid();
}

int main()
{
	pid_t srv_pid;

	srv_pid = getpid();
	printf("Server PID is: %d", srv_pid);
	
}