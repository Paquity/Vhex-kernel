#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>

//
// parser_get_word()
// Get the word at the current cursor location.
//
static ssize_t parser_get_word(char ***tab, size_t *tab_pos,
char const *str, int *counter)
{
	ssize_t i;

	i = -1;
	while (str[++i] != '\0' && str[i] != '\n' && str[i] != ' ' && str[i] != '\t');
	if (*tab != NULL){
		(*tab)[*tab_pos] = (char*)malloc(i + 1);
		if ((*tab)[*tab_pos] == NULL)
			return (-1);
		memset((*tab)[*tab_pos], 0, i + 1);
		strncpy((*tab)[(*tab_pos)++], str, i);
	}
	(*counter)++;
	return (i);
}

//
// parser_get_inibitor()
// This function will get the content of an inhibitor (and check if the
// inhibitor characteres are alone or not).
//
static ssize_t parser_get_inibitor(char ***tab, size_t *tab_pos,
char const *str, int *counter)
{
	ssize_t i;

	i = 0;
	while (str[++i] != '\0' && str[i] != '\"');
	if (str[i] != '\"')
		return (0);
	if (*tab != NULL){
		(*tab)[*tab_pos] = (char*)malloc(i + 1);
		if ((*tab)[*tab_pos] == NULL)
			return (-1);
		memset((*tab)[*tab_pos], 0, i + 1);
		strncpy((*tab)[(*tab_pos)++], str + 1, i - 1);
	}
	(*counter)++;
	return (i + 1);
}

//
// parser_setup_arg()
// This function removes useless spaces, tabs and handle '\"' inhibitor.
// Return the number of word(s) stored in "str".
//
static int parser_entry(char ***tab, char const *str)
{
	size_t tab_pos;
	ssize_t size;
	int counter;

	str--;
	counter = 0;
	tab_pos = 0;
	while (*(++str) != '\0' && *str != '\n'){
		if (*str == '\"'){
			size = parser_get_inibitor(tab, &tab_pos, str, &counter);
			if (size < 0)
				return (-1);
			str += size;
		}
		if (*str != ' ' && *str != '\t'){
			size = parser_get_word(tab, &tab_pos, str, &counter) - 1;
			if (size < 0)
				return (-1);
			str += size;
		}
	}
	return (counter);
}

//
// strtotab()
// Generate word table and indicated the number of word find in the string.
//
int strtotab(int *argc, char ***argv, char const *str)
{
	int i;

	// Check memory fault.
	if (argc == NULL || argv == NULL || str == NULL)
		return (EFAULT);

	// Get the number of word.
	*argv = NULL;
	*argc = parser_entry(argv, str);
	if (*argc <= 0)
		return (EINVAL);

	// Alloc tab.
	*argv = (char **)malloc(sizeof(char *) * (*argc + 1));
	if (*argv == NULL)
		return (ENOMEM);
	i = *argc;
	while (--i >= 0)
		(*argv)[i] = NULL;

	// Get word.
	if (parser_entry(argv, str) != *argc){
		strtotab_quit(argc, argv);
		return (ENOMEM);
	}
	(*argv)[*argc] = NULL;
	return (0);
}

//
// strtotab() 
// Free all allocated memory generated by "strtotab()"
//
void strtotab_quit(int *argc, char ***argv)
{
	// Check error
	if (argc == NULL || argv == NULL)
		return;

	// Check useless actions
	if (*argv == NULL) {
		*argc = 0;
		return;
	}

	// Free all list
	while (--(*argc) >= 0)
		free((*argv)[*argc]);
	free(*argv);

	// Secure
	*argv = NULL;
	*argc = 0;
}
