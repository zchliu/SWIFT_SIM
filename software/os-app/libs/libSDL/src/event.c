#include <NDL.h>
#include <SDL.h>
#include <stdio.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
  char tem[101] = {0};
	if (!NDL_PollEvent(tem, 100)){ 
		ev->key.keysym.sym = SDLK_NONE;
    ev->type = SDL_USEREVENT;
		return 0;
	}
	if (strncmp(tem, "kd", 2) == 0) ev->key.type = SDL_KEYDOWN;
	else ev->key.type = SDL_KEYUP;
	char *code = tem + 3;
	for (int i = 0; i < sizeof(keyname); i++) {
    if (code[i] == '\n') code[i] = '\0';
    if (strcmp(keyname[i], code) == 0) {
      ev->key.keysym.sym = i;
      return 1;
    }
  }
  return 0;
}

int SDL_WaitEvent(SDL_Event *event) {
  while (1) {
    if (SDL_PollEvent(event) == 1) break;
  }
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return NULL;
}
