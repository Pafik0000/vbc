/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipafnuto <ipafnuto@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 12:08:28 by ipafnuto          #+#    #+#             */
/*   Updated: 2025/12/25 15:37:04 by ipafnuto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum
{
	VAL = '1',
	ADD = '+',
	MUL = '*',
} nodetype;

typedef struct node
{
	nodetype type;
	int val;
	struct node *l;
	struct node *r;
	int afterpar;
} node;

node *new_node (node *value)
{
	node *n = calloc (1, sizeof (node));
	*n = *value;
	return n;
}

void destroy_tree (node *n)
{
	if (!n)
		return;
	destroy_tree (n->l);
	destroy_tree (n->r);
	free (n);
}

void errorat (char c)
{
	if (c)
		printf ("error at %c\n", c);
	else
		printf ("error at the end of line\n");
}

int pn_recoursive_routine (
	node *n, char *lines[1024], int lineN, int x
)
{
	char nodeval =
		n->type == VAL? '0' + n->val : n->type;

	if (n->l)
	{
		x = pn_recoursive_routine (n->l, lines, lineN + 1, x > 0? x - 2 : x);
	}
	if (n->r)
	{
		pn_recoursive_routine (n->r, lines, lineN + 1, x + 2);
	}

	int i = 0;
	while (i < x)
	{
		if (!lines[lineN][i])
		{
			lines[lineN][i] = ' ';
		}
		i++;
	}
	lines[lineN][i] = nodeval;

	return x + 2;
}

void pn (node *n)
{
	char	buf[1024 * 1024] = {0};
	char	*lines[1024];

	for (int i = 0; i < 1024; i++)
		lines[i] = buf + i * 1024;

	pn_recoursive_routine (n, lines, 0, 2);
	for (int i = 0; lines[i][0]; i++)
	{
		puts (lines[i]);
	}
}

node *swap (node *n)
{
//	if (1) return n;
//
	if (n->r->type == VAL)
	{
		printf (">> %s\n", "swap error");
		printf (">> at %c %i\n", n->type, n->r->val);
		return n;
	}
	node *newhead = n->r;
	n->r = newhead->l;

	if (newhead->l->type == VAL)
		newhead->l = n;
	else
		newhead->l = swap (n);

	return newhead;
}

node *parse_expr (char *s, int parcount)
{
	node	tmp;
	char	c;

	c = *(s++);
	switch (c)
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			tmp.type = VAL;
			tmp.val = c - '0';
			tmp.l = NULL;
			tmp.r = NULL;
			tmp.afterpar = 0;
			break;
		case '(':
			return parse_expr (s, parcount + 1);
		default:
			errorat (c);
			return NULL;
	}
	
	while (parcount && *s == ')')
	{
		tmp.afterpar = 1;
		parcount--;
		s++;
	}
	c = *(s++);
	switch (c)
	{
		case '\0':
			if (parcount)
			{
				errorat (c);
				return NULL;
			}
			tmp.afterpar = 0;
			return new_node (&tmp);
		case '+':
		case '*':
			tmp.r = parse_expr (s, parcount);
			if (!tmp.r)
			{
				return NULL;
			}
			tmp.l = new_node (&tmp);
			tmp.l->r = NULL;
			tmp.type = c;

			if (
				(
					*s != '('
					&& tmp.type == MUL
					&& tmp.r->type == ADD
				) || (
					tmp.r
					&& tmp.r->afterpar
				)
			)
				return swap (new_node (&tmp));
			else
				return new_node (&tmp);
		default:
			errorat (c);
			return NULL;
	}
}

int eval_tree (node *n)
{
	int a, b;

	switch (n->type)
	{
		case ADD:
//			puts ("ADD");
			a = eval_tree (n->l);
			b = eval_tree (n->r);
//			printf (">> return %i + %i\n", a, b);
			return a+b;//eval_tree (n->l) + eval_tree (n->r);
		case MUL:
//			puts ("MUL");
			a = eval_tree (n->l);
			b = eval_tree (n->r);
//			printf (">> return %i * %i\n", a, b);
			return a*b;//eval_tree (n->l) * eval_tree (n->r);
		case VAL:
//			puts ("VAL");
//			printf (">> return %i\n", n->val);
			return n->val;
		default:
			printf ("%s\n", "unknown type: %i");
			return 0;
	}
}

int main (int ac, char **av)
{
	if (ac != 2)
		return 1;
	
	node *tree = parse_expr (av[1], 0);
	if (tree)
	{
		pn (tree);
		int result = eval_tree (tree);
		printf ("%i\n", result);
		destroy_tree (tree);

		return 0;
	}

	return 1;
}
