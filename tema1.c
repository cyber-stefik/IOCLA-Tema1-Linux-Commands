// Copyright Stefanita Ionita 324CA 2021
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

/*
 * Initializeaza directorul principal, home
 */
Dir* main_init(Dir* parent) {
	char* home = malloc(sizeof(char) * 5);
	assert(home);
	strcpy(home, "home");
	parent->name = malloc(strlen(home) + 1);
	assert(parent->name);
	strcpy(parent->name, home);
	parent->parent = NULL;
	parent->next = NULL;
	parent->head_children_dirs = NULL;
	parent->head_children_files = NULL;
	free(home);

	return parent;
}

/*
 *Parcurge lista de fisiere pana la capat si il returneaza pe ultimul
 */
File* get_last_file(Dir* parent) {
	// Primeste head-ul listei de fisiere
	File* tmp = parent->head_children_files;
	// Parcurge lista pana la NULL
	while (tmp->next) {
		tmp = tmp->next;
	}
	// Returneaza ultimul fisier din directorul parent
	return tmp;
}

/*
 * Parcurge lista de directoare pana la capat si il returneaza pe ultimul
 */
Dir* get_last_dir(Dir* parent) {
	// Primeste head-ul listei de directoare
	Dir* tmp = parent->head_children_dirs;
	// Parcurge lista pana la NULL
	while (tmp->next) {
		tmp = tmp->next;
	}
	// Returneaza ultimul director din directorul parent
	return tmp;
}

/*
 * Verifica daca exista un fisier cu numele name si il returneaza,
 * in caz contrar returneaza NULL
 */
File* check_get_file(Dir* parent, char* name) {
	// Primeste head-ul listei de fisiere
	File* tmp = parent->head_children_files;
	// Parcurge lista pana la NULL
	while (tmp) {
		// Verifica daca numele fisierului este acelasi cu name
		if (strcmp(tmp->name, name) == 0) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

/*
 * Verifica daca exista un director cu numele name si il returneaza,
 * in caz contrar returneaza NULL
 */
Dir* check_get_dir(Dir* parent, char* name) {
	// Primeste head-ul listei de directoare
	Dir* tmp = parent->head_children_dirs;
	// Parcurge lista pana la NULL
	while (tmp) {
		// Verifica daca numele directorului este acelasi cu name
		if (strcmp(tmp->name, name) == 0) {
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

/*
 * Creeaza un fisier
 */
void touch (Dir* parent, char* name) {

	// Verifica daca exista fisierul cu numele name
	if (check_get_file(parent, name)) {
		printf("File already exists\n");
		return ;
	}
	// Aloc memorie pentru fisier
	File* file = malloc(sizeof(File));
	// Verific daca am facut alocarea corect
	assert(file);
	file->name = malloc(strlen(name) + 1);
	assert(file->name);
	strcpy(file->name, name);
	file->parent = parent;
	// Verific daca exista un head de fisiere in directorul parent
	if (!parent->head_children_files) {
		// Daca nu exista, file devine head-ul de fisiere
		parent->head_children_files = file;	
		file->next = NULL;
	}
	else {
		// In caz contrar, adaug fisierul la capatul listei de fisiere
		File* tmp = get_last_file(parent);
		tmp->next = file;
		file->next = NULL;
		tmp->parent = parent;
	}
}

/*
 * Creeaza un director
 */
void mkdir (Dir* parent, char* name) {

	// Verific daca exista deja un director cu numele name
	if (check_get_dir(parent, name)) {
		printf("Directory already exists\n");
		return ;
	}
	//Aloc memorie pentru director
	Dir* director = malloc(sizeof(Dir));
	// Verific daca am facut alocarea corect
	assert(director);
	director->name = malloc(strlen(name) + 1);
	assert(director->name);
	strcpy(director->name, name);
	// Adaug parintele directorului
	director->parent = parent;
	// Heads sunt NULL la creearea directorului
	director->head_children_dirs = NULL;
	director->head_children_files = NULL;
	// Verific daca exista un head de directoare in directorul parent
	if (!parent->head_children_dirs) {
		// Daca nu exista, directorul devine head-ul de directoare
		parent->head_children_dirs = director;
		director->next = NULL;
	}
	else {
		// In caz contrar, adaug directorul la capatul listei de directoare
		Dir* tmp = get_last_dir(parent);
		tmp->next = director;
	}
}

/*
 * Afiseaza numele directoarelor si fisierelor din directorul curent
 */
void ls (Dir* parent) {
	// Verific daca exista directoare in directorul parent
	if (parent->head_children_dirs) {
		Dir* tmp = parent->head_children_dirs;
		// Afisez numele directoarelor existente
		while (tmp) {
			printf("%s\n", tmp->name);
			tmp = tmp->next;
		}
	}

	// Verific daca exista fisiere in directorul parent.
	if (parent->head_children_files) {
		File* tmp2 = parent->head_children_files;
		// Afisez numele fisierelor existente
		while (tmp2) {
			printf("%s\n", tmp2->name);
			tmp2 = tmp2->next;
		}
	}
}

/*
 * Returneaza fisierul ce trebuie sters
 */
File*
remove_file(Dir* parent, char* name) {
    File *prev_file;
	File* to_rmv = check_get_file(parent, name);

	// Verific daca fisierul pe care vreau sa il sterg este head-ul listei
	// de fisiere
	if (parent->head_children_files->name == to_rmv->name) {
		// Verifica daca directorul are si alte fisiere
		if (to_rmv->next != NULL) {
			// Mut head-ul catre urmatorul fisier din lista
			parent->head_children_files = to_rmv->next;
			// Returnez head-ul pentru a fi sters
			return to_rmv;
		} else if (!(to_rmv->next)) {
			// In caz contrar, head-ul listei de fisiere devine NULL si returnez
			// head-ul, memorat in to_rmv, pentru a fi sters
			parent->head_children_files = NULL;
			parent->next = NULL;
			return to_rmv;
		}
	} else if (!(to_rmv->next)) {
		// In cazul in care fisierul pe care vreau sa il sterg nu este head-ul
		// si este la finalul listei, penultimul->next trebuie sa pointeze la
		// NULL
		File* tmp = parent->head_children_files;
		while (tmp->next->next != NULL) {
			tmp = tmp->next;
		}
		// prev_file este penultimul fisier din lista
		prev_file = tmp;
		prev_file->next = NULL;
		return to_rmv;
	} else {
		// In cazul in care fisierul se afla intre capetele listei, trebuie sa
		// refac legatura intre fisierul de dinainte de to_rmv si cel de dupa
		File* tmp = parent->head_children_files;
		// Parcurg pana dau de to_rmv si tin minte fisierul de dinainte de el
		while (tmp->next != to_rmv) {
			tmp = tmp->next;
		}
		// Urmatorul element catre care va pointa cel de dinainte de to_rmv
		// va fi urmatorul la care pointeaza to_rmv
		tmp->next = to_rmv->next;
		to_rmv->next = NULL;
		return to_rmv;
	}
}

/*
 * Sterge fisierul cu numele name din directorul parent
 */
void rm (Dir* parent, char* name) {
	// Verific daca fisierul cu numele name exista in lista
	if (!(check_get_file(parent, name))) {
		printf("Could not find the file\n");
	} else {
		// Apelez functia remove_file explicata mai sus si eliberez memoria
		// alocata numelui si fisierului
		File* to_rmv = remove_file(parent, name);
		if (remove_file) {
			free(to_rmv->name);
			free(to_rmv);
		}
	}
}
/*
 * Returneaza directorul ce trebuie sters
 */
Dir* remove_dir(Dir* parent, char* name) {
	Dir *prev_dir;
	Dir* to_rmv = check_get_dir(parent, name);
	// Verific daca directorul pe care vreau sa il sters este head-ul
	// de directoare
	if (parent->head_children_dirs->name == to_rmv->name) {
		// Verific daca directorul parent are si alte directoare
		if (to_rmv->next != NULL) {
			// Mut head-ul catre urmatorul director din lista
			parent->head_children_dirs = to_rmv->next;
			// Returnez head-ul pentru a fi sters
			return to_rmv;
		} else if (!(to_rmv->next)) {
			// In caz contrar, head-ul listei de directoare devine NULL si
			// returnez head-ul, memorat in to_rmv, pentru a fi sters
			parent->head_children_dirs = NULL;
			parent->next = NULL;
			return to_rmv;
		}
	} else if (!(to_rmv->next)) {
		// In cazul in care directorul pe care vreau sa il sterg nu este head-ul
		// si este la finalul listei, penultimul->next trebuie sa pointeze la
		// NULL
		Dir* tmp = parent->head_children_dirs;
		while (tmp->next->next) {
			tmp = tmp->next;
		}
		prev_dir = tmp;
		prev_dir->next = NULL;
		return to_rmv;
	} else {
		// In cazul in care directorul se afla intre capetele listei, trebuie sa
		// refac legatura intre directorul de dinainte de to_rmv si cel de dupa
		Dir* tmp = parent->head_children_dirs;
		// Parcurg pana gasesc to_rmv si tin minte directorul de dinainte de el
		while (tmp->next != to_rmv) {
			tmp = tmp->next;
		}
		// Urmatorul element catre care va pointa cel de dinainte de to_rmv
		// va fi urmatorul la care pointeaza to_rmv
		tmp->next = to_rmv->next;
		to_rmv->next = NULL;
		return to_rmv;
	}
}
/*
 * Elibereaza toata memoria alocata
 */
void free_all(Dir* parent) {
	File* tmp = parent->head_children_files;
	File* tmp2;
	// Parcurg lista de fisiere din directorul parent si eliberez memoria
	if (tmp) {
		while (tmp) {
			tmp2 = tmp;
			tmp = tmp->next;
			free(tmp2->name);
			free(tmp2);
		}
	}
	

	Dir* tmp3 = parent->head_children_dirs;
	Dir* tmp4;
	// Parcurg lista de directoare si apelez recursiv free_all pentru a elibera
	// memoria alocata fisierelor din directoarele respective
	if (tmp3) {
		while (tmp3) {
			tmp4 = tmp3;
			tmp3 = tmp3->next;
			free_all(tmp4);
		}
	}
	free(parent->name);
	free(parent);
}

/*
 * Sterge directorul cu numele name din directorul parent
 */
void rmdir (Dir* parent, char* name) {
	// Verific daca exista directorul pe care vreau sa il sterg
	if (!(check_get_dir(parent, name))) {
		printf("Could not find the dir\n");
	} else {
		Dir* to_rmv = remove_dir(parent, name);
		if (to_rmv) {
			free_all(to_rmv);
		}
	}
}

/*
 * Target va pointa la directorul cu numele name
 */
void cd(Dir** target, char *name) {
		Dir* tmp;
		// Daca name este .., inseamna ca target va pointa catre parintele sau
		if (strcmp(name, "..") == 0) {
			if (!((*target)->parent)) {
				return;
			}
			*target = (*target)->parent;
			return;
		} else if (!(check_get_dir(*target, name))) {
			// Verific daca exista directorul cu numele name
			printf("No directories found!\n");
		} else {
			// Folosesc functia check_get_dir pentru a sti unde va pointa target
			tmp = check_get_dir(*target, name);
			*target = tmp;
		}
}

/*
 * Afiseaza calea catre directorul target
 */
char *pwd (Dir* target) {
	Dir* tmp = target;
	Dir** array = malloc(sizeof(Dir*) * 100); // memoreaza directoarele parinte
	char* aux = malloc(sizeof(char) * 1000); // in aux concatenez numele
											 // directoarelor din array
	assert(array);
	assert(aux);
	int size = 0;
	// Parcurg ierarhia in mod invers, catre parinte si adaug fiecare director
	// intr-un array de directoare
	while (tmp->parent != NULL) {
		array[size] = tmp;
		size++;
		tmp = tmp->parent;
	}
	int i = size - 1;
	strcpy(aux, "/home");
	while (i >= 0) {
		// Concatenez / si numele fiecarui director, in ordine corecta
		strcat(aux, "/");
		strcat(aux, array[i]->name);
		i--;
	}
	free(array);
	return aux;
}

/*
 * Elibereaza toata memoria alocata
 */
void stop (Dir* target) {
	free_all(target);
}

int get_level(Dir* parent) {
	Dir* tmp = parent->head_children_dirs;
	int level = 0;
	while (tmp) {
		level++;
		tmp = tmp->head_children_dirs;
	}
	return level;
}

char* add_spaces(int level) {
	char* aux = malloc(sizeof(char) * 100);
	for (int i = 0; i < level; i++) {
		strcat(aux, "    ");
	}
	return aux;
}

int get_dir_size(Dir* target) {
	Dir* tmp = target->head_children_dirs;
	int size = 0;
	while (tmp) {
		size++;
		tmp = tmp->next;
	}
	return size;
}
/*
 * Afiseaza ierarhia
 */
void tree (Dir* target, int level) {

	if (level == 1) {
		ls(target);
		return;
	}
	// Incercare esuata de a rezolva iterativ, am fost pe aproape :p
	// int size = get_dir_size(target);
	// int count = 0;
	// int aux = level;
	// while (size > 0) {
	// 	int aux_size = size;
	// 	Dir* tmp = target->head_children_dirs;
	// 	int aux_count = count;
	// 	while (aux_count > 0) {
	// 		tmp = tmp->next;
	// 		aux_count--;
	// 	}
	// 	if (!tmp->head_children_dirs || !tmp->head_children_files) {
	// 		printf("%s\n", tmp->name);
	// 	} else {
	// 		for (int i = 0; i < level; i++) {
	// 			char* to_print = malloc(sizeof(char) * 100);
	// 			char* spaces = add_spaces(i);
	// 			strcpy(to_print, spaces);
	// 			strcat(to_print, tmp->name);
	// 			if (tmp->head_children_dirs->head_children_dirs) {
	// 				tmp = tmp->head_children_dirs;
	// 			} else if (tmp->next) {
	// 				while (tmp) {
	// 					tmp = tmp->next;
	// 					strcat(to_print, tmp->name);
	// 				}
	// 			}
	// 			printf("%s\n", to_print);
	// 			free(spaces);
	// 			free(to_print);
	// 		}
	// 		for (int i = level - 1; i > 0; i--) {
	// 			char* to_print = malloc(sizeof(char) * 100);
	// 			char* spaces = add_spaces(i);
	// 			strcpy(to_print, spaces);
	// 			strcat(to_print, tmp->head_children_files->name);
	// 			tmp = tmp->parent;
	// 			printf("%s\n", to_print);
	// 			free(spaces);	
	// 			free(to_print);
	// 		}
	// 	}
	// 	count++;
	// 	size--;
	// }
}

/*
 * Modifica numele unui fisier sau director
 */
void mv(Dir* parent, char *oldname, char *newname) {

	// Verific daca numele in care vrea sa se schimbe fisierul/directorul exista
	// deja, daca numele pe care vrem sa-l atribuim unui fisier
	// apartine unui director sau daca numele pe care vrem sa-l atribuim unui
	// director apartine deja unui fisier
	if ((check_get_dir(parent, oldname) && check_get_file(parent, newname)) ||
		(check_get_dir(parent, newname) && check_get_file(parent, oldname))
		|| (check_get_file(parent, newname) || check_get_dir(parent, newname)))
		{
			printf("File/Director already exists\n");
			return;
		}

	// Cazul in care trebuie schimbat numele fisierului
	if (check_get_file(parent, oldname) && !check_get_file(parent, newname)) {
		File* tmp = check_get_file(parent, oldname);
		// Sterg fisierul pe care vrem sa-l redenumim din lista de fisiere
		rm(parent, tmp->name);
		// Creez fisierul cu noul nume la finalul listei
		touch(parent, newname);
		return;
	}

	// Cazul in care trebuie schimbat numele directorului
	if (check_get_dir(parent, oldname) && !check_get_dir(parent, newname)) {
		Dir* tmp = check_get_dir(parent, oldname);
		// Cazul in care directorul este gol
		if (!tmp->head_children_files) {
			// Sterg directorul pe care vrem sa-l redenumim din lista
			rmdir(parent, oldname);
			// Creez directorul cu noul nume la finalul listei
			mkdir(parent, newname);
		} else { // Cazul in care directorul are fisiere in interior
			// Creez un director nou la finalul listei
			mkdir(tmp->parent, newname);
			File* tmp2 = tmp->head_children_files;
			Dir* tmp3 = check_get_dir(tmp->parent, newname); // Noul director
			// Parcurg lista de fisiere din interiorul directorului vechi
			while (tmp2) {
				// Creez fisierele in noul director
				touch(tmp3, tmp2->name);
				tmp2 = tmp2->next;
			}
			// Sterg, intr-un final, directorul
			rmdir(tmp->parent, tmp->name);
		}
		return;
	}
	
	// Cazul in care nu exista un fisier/director cu numele oldname
	if (!check_get_file(parent, oldname) || !check_get_dir(parent, oldname)) {
		printf("File/Director not found\n");
		return;
	}

}


int main () {
	Dir* target = malloc(sizeof(Dir));
	assert(target);
	Dir* saved_home;
	assert(target);
	target = main_init(target);
	saved_home = target;
	do
	{
		char* command = malloc(sizeof(char) * MAX_INPUT_LINE_SIZE);
		assert(command);
		fgets(command, MAX_INPUT_LINE_SIZE, stdin);
		char* aux = strtok(command, " \n");
		
		if (strcmp(aux, "touch") == 0) {
			aux = strtok(NULL, " \n");
			touch(target, aux);
		}
		if (strcmp(aux, "mkdir") == 0) {
			aux = strtok(NULL, " \n");
			mkdir(target, aux);
		}

		if (strcmp(aux, "ls") == 0) {
			ls(target);
		}

		if (strcmp(command, "rm") == 0) {
			aux = strtok(NULL, " \n");
			rm(target, aux);
		}

		if (strcmp(command, "rmdir") == 0) {
			aux = strtok(NULL, " \n");
			rmdir(target, aux);
		}

		if (strcmp(command, "cd") == 0) {
			aux = strtok(NULL, " \n");
			cd(&target, aux);
		}

		if (strcmp(command, "pwd") == 0) {
			if (strcmp(target->name, "home") == 0) {
				printf("/home\n");
			} else {
				char* result = pwd(target);
				printf("%s\n", result);
				free(result);
			}
		}

		if (strcmp(command, "tree") == 0) {
			int level = get_level(saved_home);
			tree(target, level);
		}

		if (strcmp(command, "mv") == 0) {
			aux = strtok(NULL, " \n");
			char* aux2 = strtok(NULL, " \n");
			mv(target, aux, aux2);
		}

		if (strcmp(aux, "stop") == 0) {
			stop(saved_home);
			free(command);
			break;
		}
		free(command);
		
	} while (1);
	return 0;
}
