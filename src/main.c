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

/*void pn (node *n)
{
	node *level[1024] = {0};
	level[0] = n;

	while (level[0])
	{
		for (int i = 0; level[i]; i++)
		{

		}
	}
}
*/
node *swap (node *n)
{
//	if (1) return n;
//
	node *newhead = n->r;
	n->r = n->r->l;
	newhead->l = n;

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
			puts ("ADD");
			a = eval_tree (n->l);
			b = eval_tree (n->r);
			printf (">> return %i + %i\n", a, b);
			return a+b;//eval_tree (n->l) + eval_tree (n->r);
		case MUL:
			puts ("MUL");
			a = eval_tree (n->l);
			b = eval_tree (n->r);
			printf (">> return %i * %i\n", a, b);
			return a*b;//eval_tree (n->l) * eval_tree (n->r);
		case VAL:
			puts ("VAL");
			printf (">> return %i\n", n->val);
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
		int result = eval_tree (tree);
		printf ("%i\n", result);
		destroy_tree (tree);

		return 0;
	}

	return 1;
}
