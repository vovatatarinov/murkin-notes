#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include "./lib/tomlc99/toml.h"

void createConfig(const char* configPath, const char* HOME)
{
  char customPath[1024];
  printf("Enter path for your notes directory:\n");
  printf("(you can leave input empty and press \"Enter\" for creating \"note\" directory at %s.local/share/mynotes/)\n", HOME);
  printf("%s", HOME);
  fgets(customPath, sizeof(customPath), stdin);
  char notesPath[1024];
  strcpy(notesPath, HOME);

  if (customPath[0] == '\n')
  {
    strcat(notesPath, ".local/share/mynotes/");
    puts(notesPath);
    mkdir(notesPath, 0777);
  }
  else 
  {
    strcat(notesPath, customPath);
    if (mkdir(notesPath, 0777) == -1)
    {
      if (errno == EEXIST)
      {
      }
      else 
      {
        perror("Error while creating directory");
        exit(1);
      }
    }
    else 
    {
      printf("Directory created\n");
    }
  }

  FILE* createConfig = fopen(configPath, "w");
  char configText1[] = "# provides config location\nlocation = ";
  char* configText = malloc(sizeof(configText1) + sizeof(notesPath) + 1);
  strcpy(configText, configText1);
  strcat(configText, notesPath);
  puts(configText);
  fwrite(configText, sizeof(configText[0]), strlen(configText) - 1, createConfig);
  free(configText);
  fclose(createConfig);
  return;

}

void printGreet()
{
  printf("Notes Store\n");
}

void printOptions()
{
  printf("c - create note   o - open   e - edit note   d - delete note   q - exit\n"); }

void mkdirNotes(const char* notesPath)
{
  mkdir(notesPath, 0777);
}

void showNotes(const char* notesPath)
{
  uint16_t filesCount;
  DIR* dir;
  struct dirent* entry;
  char* noteName;

  dir = opendir(notesPath);
  while ((entry = readdir(dir)) != NULL)
  {
    if (entry->d_type == DT_REG)
    {
      noteName = entry->d_name; 
      noteName[strcspn(noteName, ".")] = '\0'; // removing .md 
      printf("--- %s\n", noteName);
      ++filesCount;
    }
  }
  if (filesCount == 0)
    printf("**Notes storage is empty**\n");
  closedir(dir);
}

void createNote(char* noteName, const char* notesPath)
{
  char fullPath[128];
  strcpy(fullPath, notesPath);
  strcat(fullPath, noteName);
  char* md = ".md";
  strcat(fullPath, md);

  FILE* checkNote = fopen(fullPath, "r");
  if (checkNote != NULL)
  {
    printf("Note \"%s\" already exist!\nPress Enter...\n", noteName);
    scanf("%*c");
    printf("\033[2J\033[H");
    fclose(checkNote);
    return;
  }

  FILE* newNote = fopen(fullPath, "w");
  printf("Enter content:\n");
  char content[4096];
  fgets(content, 4096, stdin);
  /* scanf("%*c"); */
  fputs(content, newNote);
  printf("Note \"%s\" sucsessfully created\nPress Enter...\n", noteName);
  scanf("%*c");
  printf("\033[2J\033[H");
  fclose(newNote);
  return;
}

void openNote(char* noteName, const char* notesPath)
{
  char fullPath[128];
  strcpy(fullPath, notesPath);
  strcat(fullPath, noteName);
  char* md = ".md";
  strcat(fullPath, md);
  char content[4096];

  FILE* readNote = fopen(fullPath, "r");
  if (readNote != NULL)
  {
    printf("Content of note \"%s\": \n", noteName);
    fread(content, 1, 4096, readNote);
    printf("%s", content);
    printf("Press Enter...\n");
    scanf("%*c");
    printf("\033[2J\033[H");
    return;
  }
  printf("Note \"%s\" does not exist!\nPress Enter...\n", noteName);
  scanf("%*c");
  printf("\033[2J\033[H");
  return;

}

void deleteNote(char* noteName, const char* notesPath)
{
  char fullPath[128];
  strcpy(fullPath, notesPath);
  strcat(fullPath, noteName);
  char* md = ".md";
  strcat(fullPath, md);

  FILE* checkNote = fopen(fullPath, "r");
  if (checkNote != NULL)
  {
    remove(fullPath);
    printf("Note \"%s\" sucsessfully deleted\nPress Enter...\n", noteName);
    scanf("%*c");
    printf("\033[2J\033[H");
    return;
  }
  printf("Note \"%s\" does not exist!\nPress Enter...\n", noteName);
  scanf("%*c");
  printf("\033[2J\033[H");
  return;
}

void editNote(char* noteName, const char* notesPath)
{
  char fullPath[128];
  strcpy(fullPath, notesPath);
  strcat(fullPath, noteName);
  char* md = ".md";
  strcat(fullPath, md);
  char content[4096];

  FILE* checkNote = fopen(fullPath, "r");
  if (checkNote != NULL)
  {
    FILE* newNote = fopen(fullPath, "w");
    printf("Enter new content for note \"%s\" (all previous content will be deleted): \n", noteName);
    fgets(content, 4096, stdin);
    fputs(content, newNote);
    printf("Content in note \"%s\" sucsessfully edited\nPress Enter...\n", noteName);
    scanf("%*c");
    printf("\033[2J\033[H");
    fclose(newNote);
    fclose(checkNote);
    return;
  }
  printf("Note \"%s\" does not exist!\nPress Enter...\n", noteName);
  scanf("%*c");
  printf("\033[2J\033[H");
  return;
}

int main(void)
{
  printf("\033[2J\033[H");

  char isExit = 1;
  const char* notesPath = "/notes/";
  const char* HOME = getenv("HOME");
  strcat(HOME, "/");
  char configPath[256];
  strcpy(configPath, HOME);
  strcat(configPath, ".config/notes-supa-config/");
  if (mkdir(configPath, 0777) == -1)
  {
    perror("error");
  }
  char* configName = "config.toml";
  strcat(configPath, configName);
  printf("%s\n", configPath);
   
  createConfig(configPath, HOME);


  /* printf("\033[2J\033[H"); */
  /* while (isExit != 0) */
  /* { */
  /*   printGreet(); */
  /*   mkdirNotes(notesPath); */
  /*   showNotes(notesPath); */
  /*   printOptions();  */
  /**/
  /*   char action[2]; */
  /*   if (fgets(action, 2, stdin) == NULL) */
  /*   { */
  /*     printf("Error while reading\nPress Enter...\n"); */
  /*     break; */
  /*   } */
  /**/
  /*   char noteName[64]; */
  /*   if (action[0] == 'c') */
  /*   { */
  /*     printf("Enter name of new note:\n"); */
  /*     fgets(noteName, 64, stdin); */
  /*     noteName[strcspn(noteName, "\n")] = '\0'; //replacing '\n' with '\0' in noteName */
  /*     createNote(noteName, notesPath); */
  /*   } */
  /*   else if (action[0] == 'o') */
  /*   { */
  /*     printf("Enter name of note that you want to open:\n"); */
  /*     fgets(noteName, 64, stdin); */
  /*     noteName[strcspn(noteName, "\n")] = '\0'; //replacing '\n' with '\0' in noteName */
  /*     openNote(noteName, notesPath); */
  /*   } */
  /*   else if (action[0] == 'e')     */
  /*   { */
  /*     printf("Enter name of note that you want to edit:\n"); */
  /*     fgets(noteName, 64, stdin); */
  /*     noteName[strcspn(noteName, "\n")] = '\0'; //replacing '\n' with '\0' in noteName */
  /*     editNote(noteName, notesPath); */
  /*   } */
  /*   else if (action[0] == 'd') */
  /*   { */
  /*     printf("Enter name of note that you want to delete:\n"); */
  /*     fgets(noteName, 64, stdin); */
  /*     noteName[strcspn(noteName, "\n")] = '\0'; //replacing '\n' with '\0' in noteName */
  /*     deleteNote(noteName, notesPath);       */
  /*   } */
  /*   else if (action[0] == 'q') */
  /*   { */
  /*     printf("\033[2J\033[H"); */
  /*     isExit = 0; */
  /*     break; */
  /*   } */
  /* } */

  return 0;
}
