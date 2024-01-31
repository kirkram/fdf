/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_get_colors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: klukiano <klukiano@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 17:07:41 by klukiano          #+#    #+#             */
/*   Updated: 2024/01/31 12:32:01 by klukiano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	get_r(unsigned int rgba)
{
	unsigned int	newcolor;

	newcolor = (rgba >> 24) & 0xFF;
	return (newcolor);
}

int	get_g(unsigned int rgba)
{
	return ((rgba >> 16) & 0xFF);
}

int	get_b(unsigned int rgba)
{
	return ((rgba >> 8) & 0xFF);
}

int	get_a(unsigned int rgba)
{
	return (rgba & 0xFF);
}

unsigned int	add_full_alpha(unsigned int color)
{
	color = ((color << 8) | 0xFF);
	return (color);
}
