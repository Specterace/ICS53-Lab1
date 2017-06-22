/*
 * Oscar Choy
 * ID# 53555169
 * 1/20/2017
 * Lab 1
 *
 * This program is a text-based "adventure game". The user, as the player, will need to load a dungeon
 * file which describes the rooms in the dungeon to be explored, and how they are connected. The player
 * will begin in an initial room, and this will be the first room printed on the screen to the player. The
 * player can then issue any one of the following commands to move to the room in that direction: "north",
 * "south", "east", and "west". When a player moves to a room, that room's description will come up. If there
 * is no room in that direction, the game will print out a message stating "You can't go there.", and will
 * prompt the player to enter another command, not moving from their current room. At any time, the player 
 * can enter the command "quit" to quit the game entirely.
*/

#include <stdio.h>
#include <string.h>
#define numRooms 100

struct dunRoom {
  int roomNum;
  char description[80];
  int nPos;
  int sPos;
  int ePos;
  int wPos;
};

struct dunRoom rooms[numRooms];

int parseFile(char* dunFile);
int loadFile(char* dunFile);
int goToDestination(int destin, int cRoomIdx, int roomTotal);

int main() {
  
  FILE *fp;
  char fileName[1000];
  int rCount;
  int isViable;
  int isLoaded = 0;
  char input[1000];
  
  char command[1000];
  char junk[1000];
  int numParams;
  
  int currRoom;
  int destination;
  int notPlaying = 0;
  char oldFile[1000];

  while(isLoaded == 0) {  
    printf("$ ");
    gets(input);

    if (strcmp(input, "quit") == 0) {
      return 0;
    } else if((strcmp(input, "north") == 0) || (strcmp(input, "south") == 0)
	|| (strcmp(input, "east") == 0) || (strcmp(input, "west") == 0)){
      printf("No dungeon file loaded.\n");
    } else {
      numParams = sscanf(input, "%s %s %s", command, fileName, junk);

      if (numParams != 2) {
        printf("Invalid command. Try again.\n");
      } else if (strcmp(command, "loaddungeon") != 0) {
        printf("Invalid command. Try again.\n");
      } else {
        isViable = parseFile(fileName);
        if (isViable == 1) {
          rCount = loadFile(fileName);
          isLoaded = 1;
        }
      }
    }
  }
  
  currRoom = 0;
  printf("%s\n", rooms[currRoom].description);

  while (notPlaying == 0) {
    printf("$ ");
    gets(input);
    
    if (strcmp(input, "quit") == 0) {
      notPlaying = 1;
    } else if(strcmp(input, "north") == 0) {
      currRoom = goToDestination(rooms[currRoom].nPos, currRoom, rCount);
    } else if(strcmp(input, "south") == 0) {
      currRoom = goToDestination(rooms[currRoom].sPos, currRoom, rCount);
    } else if(strcmp(input, "east") == 0) {
      currRoom = goToDestination(rooms[currRoom].ePos, currRoom, rCount);
    } else if(strcmp(input, "west") == 0) {
      currRoom = goToDestination(rooms[currRoom].wPos, currRoom, rCount);
    } else {
      strcpy(oldFile, fileName);
      numParams = sscanf(input, "%s %s %s", command, fileName, junk);
      if (numParams != 2) {
        printf("Invalid command. Try again.\n");
        strcpy(fileName, oldFile);
      } else if (strcmp(command, "loaddungeon") != 0) {
        printf("Invalid command. Try again.\n");
        strcpy(fileName, oldFile);
      } else {
        isViable = parseFile(fileName);
        if (isViable == 0) {
          strcpy(fileName, oldFile);
        } else {
          printf("Loading new dungeon\n");
          rCount = loadFile(fileName);
          printf("Dungeon is loaded.\n");
          currRoom = 0;
          printf("%s\n", rooms[currRoom].description);
        }
      }
    }
  }
  return 0;
}

int goToDestination(int destin, int cRoomIdx, int roomTotal) {
  int index, currRoomNum, found = 0;
  if (destin <= 0) {
    printf("You can't go there.\n");
    index = cRoomIdx;
  } else {
    index = 0;
    while ((index < roomTotal)&& (found != 1)) {
      currRoomNum = rooms[index].roomNum;
      if(currRoomNum == destin) {
        found = 1;
      } else {
        index++;
      }
    }
    if (found == 1) {
      printf("%s\n", rooms[index].description);
    } else {
        printf("Cant go to that room.\n");
        index = cRoomIdx;
    }  
  }  
  return index;
}

int loadFile(char* dunFile) { 
  FILE *fp;
  fp = fopen(dunFile, "r");

  char* token;
  const char s[] = "$\n";
  int index = 0;
  char str[200];
  char oldLine[200];
  int lineNum, nRoom, sRoom, eRoom, wRoom, roomAmt;

  while(fgets(str, 200, fp) != NULL) {
    strcpy(oldLine, str);
    token = strtok(str, s);
    sscanf(token, "%d", &lineNum);
    rooms[index].roomNum = lineNum;
    token = strtok(NULL, s);
 
    strcpy(rooms[index].description, token);
    token = strtok(NULL, s);
  
    sscanf(token, "%d %d %d %d", &nRoom, &sRoom, &eRoom, &wRoom);
    rooms[index].nPos = nRoom;
    rooms[index].sPos = sRoom;
    rooms[index].ePos = eRoom;
    rooms[index].wPos = wRoom;
    index++;
  }
  roomAmt = index;

  fclose(fp);
  return roomAmt;
}

int parseFile(char* dunFile) {
  int isCorrect = 1;
  FILE *fp;
  fp = fopen(dunFile, "r");
  if (fp == NULL) {
    perror("Error opening file");
    return 0;
  } else {
    char str[200];
    int numTok, roomNum, nPos, sPos, ePos, wPos;
    char dolPres;
    char strDesc[200];
    
    while((fgets(str, 200, fp) != NULL) && (isCorrect == 1)) {
      numTok = sscanf(str, "%d $%[^$]$ %d %d %d %d", &roomNum, strDesc, &nPos, &sPos, &ePos, &wPos);

      if (numTok != 6) {
        printf("File not vaild. Try another one.\n");
        isCorrect = 0;
      } else if (roomNum <= 0) {
        printf("File not vaild. Try another one.\n");
        isCorrect = 0;
      } else if ((nPos < -1) || (sPos < -1) || (ePos < -1) || (wPos < -1)) {
        printf("File not vaild. Try another one.\n");
        isCorrect = 0;
      }
    }
  }
  fclose(fp);
  return isCorrect;
}
